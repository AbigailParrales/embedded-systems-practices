
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c-lcd.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* USER CODE END Includes */
#include "string.h"

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/*****HANDLERS**************/
osThreadId TaskTempHandle;
osThreadId Task_SenseHandle;
osMutexId uartMutexHandle;
osThreadId Task_LCDHandle;
osThreadId Task_DebounceHandle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);

/**************** TASK FUNCTIONS ***********************/
void sense_Task (void *argument);		//sender
void buttons_Task (void *argument);		//sender
void display_Task (void *argument);		//receiver

/**************** TASK HANDLERS ***********************/
xTaskHandle Sense_Handler;
xTaskHandle Buttons_Handler;
xTaskHandle Display_Handler;

/**************** QUEUE HANDLER ***********************/
xQueueHandle SimpleQueue;

/********************** BUTTONS ***********************/
void leftButton(void);
void midButton(void);
void rightButton(void);

/********************** UPDATES ***********************/
void update_statusData(void);
void update_tempStr(void);
char update_tempNum(void);
void update_sLEDs(void);
void update_data(void);

/********************* DISPLAYS ***********************/
void display_in_terminal(void);
void display_in_LCD(void);
void display_in_LED(void);

void uart_Temp(void);

uint32_t value[1];

uint8_t data[]="TEMP:     ,LEDMAX:    ,LEDMIN:    ,MAX:     ,MIN:    ---\n\r";

char button[]="---";

struct statusDataStr {
	char tempStr[3];
	char numTemp;

	char *max;		//string max level
	char *min;		//string min level

	char max_umbral;	//numeric max level
	char min_umbral;	//numeric min level

	char *L_max;	//max LED status string
	char *L_min;	//min LED status string
	GPIO_PinState status_Lmax;	//max level LED
	GPIO_PinState status_Lmin;	//min level LED

	char lButton;
	char mButton;
	char rButton;
	GPIO_PinState status_lLED;
	GPIO_PinState status_mLED;
	GPIO_PinState status_rLED;

	char *termSensation;

}statusData;

/*****************************************************************************/
int main(void) {

	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();

	statusData.min = "20";
	statusData.max = "30";
	statusData.min_umbral = 50;
	statusData.max_umbral = 50;
	statusData.termSensation = "----";
	statusData.tempStr[0] = '-';
	statusData.tempStr[1] = '-';
	statusData.tempStr[2] = '.';
	statusData.tempStr[3] = '-';

	/************************* Create Integer Queue ****************************/
	SimpleQueue = xQueueCreate(3, sizeof (statusData));
	/*if (SimpleQueue == 0)  // Queue not created
	{
		char *str = "Unable to create Integer Queue\n\n";
		HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);
	}
	else
	{
		char *str = "Integer Queue Created successfully\n\n";
		HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);
	}*/

	lcd_init();

	HAL_ADC_Start_DMA(&hadc1, value, 1);

	xTaskCreate(sense_Task, "SEND_sensor", 128, NULL, 3, &Sense_Handler);
	xTaskCreate(buttons_Task, "SEND_buttons", 128, NULL , 3, &Buttons_Handler);

	xTaskCreate(display_Task, "RX_display", 128, NULL, 3, &Display_Handler);

	vTaskStartScheduler();

	while (1){

	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

/** 
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) 
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//SALIDAS
	/*Configure GPIO pins : PC14 */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PA1 PA2 PA3 PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	/**************************/
	//ENTRADAS
	/*Configure GPIO pin : PB0 B10 B4 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);




}

/**************** TASK FUNCTIONS ***********************/
void sense_Task (void *argument) {		//sender
	uint32_t TickDelay = pdMS_TO_TICKS(1000);

	while(1) {
		HAL_ADC_Start_DMA(&hadc1, value, 1);	//hacer una medicion en el ADC
		update_statusData();

		if (xQueueSend(SimpleQueue, &statusData, portMAX_DELAY) == pdPASS) {}
		vTaskDelay(TickDelay);
	}
}
void buttons_Task (void *argument) {		//sender
	uint32_t TickDelay = pdMS_TO_TICKS(1000);

	while(1){
		leftButton();
		midButton();
		rightButton();

		if (xQueueSend(SimpleQueue, &statusData, portMAX_DELAY) == pdPASS) {}
		vTaskDelay(TickDelay);
	}
}
void display_Task (void *argument) {		//receiver
	uint32_t TickDelay = pdMS_TO_TICKS(1000);

	while(1){
		display_in_terminal();
		display_in_LED();
		display_in_LCD();

		if (xQueueReceive(SimpleQueue, &statusData, portMAX_DELAY) != pdTRUE){}
		vTaskDelay(TickDelay);
	}
}

void update_statusData(void) {
	update_tempStr();
	statusData.numTemp = update_tempNum();
	update_sLEDs();
}
void update_tempStr(void){
	statusData.tempStr[0]=((value[0]/1000)+48);			//tens
	statusData.tempStr[1]=(((value[0]%1000)/100)+48);	//units
	statusData.tempStr[2]='.';
	statusData.tempStr[3]=(((value[0]%100)/10)+48);		//decimals
}
char update_tempNum(void){
	char units;
	char tens;
	char tmp;

	tens=((value[0]/1000)+48);
	units=(((value[0]%1000)/100)+48);

	tmp = (tens*10)+units;
	return tmp;
}
void update_sLEDs(void){
	//HIGH umbral
	if(statusData.numTemp>statusData.max_umbral) {
		statusData.min = "OFF";
		statusData.max = "ON ";

		statusData.status_Lmax = GPIO_PIN_RESET;
		statusData.status_Lmin = GPIO_PIN_SET;

		statusData.termSensation = "HOT ";
	}
	//MID umbral
	if((statusData.numTemp<statusData.max_umbral) && (statusData.numTemp>statusData.min_umbral)){
		statusData.min = "OFF";
		statusData.max = "OFF";

		statusData.status_Lmax = GPIO_PIN_SET;
		statusData.status_Lmin = GPIO_PIN_SET;

		statusData.termSensation = "MILD";
	}
	//LOW umbral
	if(statusData.numTemp<statusData.min_umbral) {
		statusData.min = "ON ";
		statusData.max = "OFF";

		statusData.status_Lmax = GPIO_PIN_SET;
		statusData.status_Lmin = GPIO_PIN_RESET;

		statusData.termSensation = "COLD";
	}
}
void update_data(void){
	//TEMP
	data[6] = statusData.tempStr[0]; 	//tens
	data[7] = statusData.tempStr[1]; 	//tens
	data[8]=('C');

	//LEDMAX
	data[18]=statusData.L_max[0];
	data[19]=statusData.L_max[1];
	data[20]=statusData.L_max[2];

	//LEDMIN
	data[30]=statusData.L_min[0];
	data[31]=statusData.L_min[1];
	data[32]=statusData.L_min[2];

	//MAX
	data[40]= statusData.max[0];
	data[41]= statusData.max[1];
	data[42]=('C');

	//MIN
	data[50]= statusData.min[0];
	data[51]= statusData.min[1];
	data[52]=('C');

	//Buttons
	data[53]=statusData.lButton;
	data[54]=statusData.mButton;
	data[55]=statusData.rButton;
}

void rightButton(void){
	//////////////////DERECHA/////////////////////////////////////////////////////////////////////////

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_RESET){  // se presionasdsds
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,GPIO_PIN_SET);          //prende
		statusData.status_rLED = GPIO_PIN_SET;

		statusData.rButton = 'R';
		button[2]='R';
		data[55]='R';
	}
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_SET){
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,GPIO_PIN_RstatusData.rButton = '-';
		button[2]='-';
		data[55]='-';
	}
}
void midButton(void){

	///////////////MEDIO///////////////////////////////////////////////////////////////////////////
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==GPIO_PIN_RESET){  // se presiona
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET);          //prende
		statusData.status_mLED = GPIO_PIN_SET;

		statusData.mButton = 'M';
		button[1]='M';
		data[54]='M';
	}
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==GPIO_PIN_SET){
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET);	      //apagado
		statusData.status_mLED = GPIO_PIN_RESET;

		statusData.mButton = '-';

		button[1]='-';
		data[54]='-';
	}
}
void leftButton(void){

	/////////////////IZQUIERDA//////////////////////////////////////////////////////////////////////////
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==GPIO_PIN_RESET){  // se presiona
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,GPIO_PIN_SET);          //RESET LED APAGA
		statusData.status_lLED = GPIO_PIN_SET;

		statusData.lButton = 'L';
		button[0]='L';
		data[53]='L';
	}
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==GPIO_PIN_SET){
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,GPIO_PIN_RESET);	      //SET LED PRENDE
		statusData.status_lLED = GPIO_PIN_RESET;

		statusData.lButton = '-';
		button[0]='-';
		data[53]='-';
	}
}

void display_in_terminal(void){
	update_data();

	HAL_UART_Transmit(&huart1, data, 60, 1000);
	HAL_Delay(1000);
}
void display_in_LCD(void){
	lcd_send_cmd(0x80);
	lcd_send_string("TEMP:");
	lcd_send_data(statusData.tempStr[0]);//decimal
	lcd_send_data(statusData.tempStr[1]);//unidad
	lcd_send_data(statusData.tempStr[2]);//punto decimal
	lcd_send_data(statusData.tempStr[3]);//punto decimal
	lcd_send_string(" C     ");
	lcd_send_cmd(0xc0); //segunda linea
	lcd_send_string("                ");
	lcd_send_cmd(0x8b);
	lcd_send_data(statusData.lButton);//decimal
	lcd_send_data(statusData.mButton);//unidad
	lcd_send_data(statusData.rButton);//punto decimal

	vTaskDelay(100);

	lcd_send_cmd(0xc0);
	lcd_send_string(statusData.termSensation);
}
void display_in_LED(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, statusData.status_Lmax);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, statusData.status_Lmin);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,statusData.status_rLED);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,statusData.status_mLED);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,statusData.status_lLED);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{ 
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
