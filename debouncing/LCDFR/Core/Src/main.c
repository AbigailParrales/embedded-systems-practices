
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c-lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "string.h"
/* USER CODE END Includes */

/**************************************************************************************/

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/*****MUTEX**************/
osMutexId uartMutexHandle;
osMutexId i2cMutexHandle;
osMutexId tempMutexHandle;

/*****HANDLERS**************/
osThreadId TaskTempHandle;
osThreadId Task_SenseHandle;
osThreadId Task_LCDHandle;
osThreadId Task_DebounceHandle;

/*****QUEUE**************/
xQueueHandle SimpleQueue;

/**************************************************************************************/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
void TaskTemp_init(void const * argument);
void Task_Sense_init(void const * argument);
void Task_LCD_init(void const * argument);
void Task_Debounce_init(void const * argument);
void leftButton(void);
void midButton(void);
void rightButton(void);

void uart_Temp(void);

/**************************************************************************************/
uint32_t value[1];
uint32_t valmax=50;
uint32_t valmin=50;
char button[]="---";
uint8_t data[]="TEMP:     ,LEDMAX:    ,LEDMIN:    ,MAX:     ,MIN:    ---\n\r";

/**************************************************************************************/
int main(void)
{

	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();

	lcd_init();

	HAL_ADC_Start_DMA(&hadc1, value, 1);

	/* Create the mutex(es) */
	/* definition and creation of i2cMutex */
	osMutexDef(i2cMutex);
	i2cMutexHandle = osMutexCreate(osMutex(i2cMutex));

	/* definition and creation of uartMutex */
	osMutexDef(uartMutex);
	uartMutexHandle = osMutexCreate(osMutex(uartMutex));

	/* definition and creation of uartMutex */
	osMutexDef(tempMutex);
	tempMutexHandle = osMutexCreate(osMutex(tempMutex));

	/* Create the thread(s) */

	/*****HIGH PRIORITY**************/
	/* definition and creation of TaskTemp */
	osThreadDef(TaskTemp, TaskTemp_init, osPriorityHigh, 0, 128);
	TaskTempHandle = osThreadCreate(osThread(TaskTemp), NULL);

	/* definition and creation of Task_LCD */
	osThreadDef(Task_LCD, Task_LCD_init, osPriorityHigh, 0, 128);
	Task_LCDHandle = osThreadCreate(osThread(Task_LCD), NULL);

	/*****LOW PRIORITY**************/
	/* definition and creation of Task_Sense */
	osThreadDef(Task_Sense, Task_Sense_init, osPriorityLow, 0, 128);
	Task_SenseHandle = osThreadCreate(osThread(Task_Sense), NULL);

	/* definition and creation of TaskDebounce */
	osThreadDef(Task_Debounce, Task_Debounce_init, osPriorityLow, 0, 128);
	Task_DebounceHandle = osThreadCreate(osThread(Task_Debounce), NULL);


	/* USER CODE BEGIN RTOS_THREADS */


	/* USER CODE END RTOS_THREADS */

	/* Start scheduler */
	osKernelStart();

	vTaskStartScheduler();

	while (1){

	}
}

/**************************************************************************************/

/**
 * @brief  Function implementing the TaskTemp thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_TaskTemp_init */
void TaskTemp_init(void const * argument)
{
	for(;;)
	{
		HAL_ADC_Start_DMA(&hadc1, value, 1);
		//osDelay(500);
		vTaskDelay(500);

		xSemaphoreTake(tempMutexHandle, portMAX_DELAY);
		HAL_UART_Transmit(&huart1, data, 60,1000);
		xSemaphoreGive(tempMutexHandle);
		//osDelay(500);//cada seg
		vTaskDelay(500);
	}
}

/* USER CODE BEGIN Header_Task_Sense_init */
/**
 * @brief Function implementing the Task_Sense thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Task_Sense_init */
void Task_Sense_init(void const * argument)
{
	for(;;)
	{
		xSemaphoreTake(uartMutexHandle, portMAX_DELAY);
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		xSemaphoreGive(uartMutexHandle);
		//osDelay(100);//cada seg
		vTaskDelay(100);
	}
}

/**
 * @brief  Function implementing the TaskLCD thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_TaskLCD_init */
void Task_LCD_init(void const * argument)
{
	for(;;)
	{
		xSemaphoreTake(i2cMutexHandle, portMAX_DELAY);
		lcd_send_cmd(0x80);
		lcd_send_string("TEMP:");
		lcd_send_data((value[0]/1000)+48);//decimal
		lcd_send_data(((value[0]%1000)/100)+48);//unidad
		lcd_send_string(".");
		lcd_send_data(((value[0]%100)/10)+48);//punto decimal
		lcd_send_string(" C     ");
		lcd_send_cmd(0xc0); //segunda linea
		lcd_send_string("                ");
		lcd_send_cmd(0x8b);
		lcd_send_string(button);
		//HAL_Delay(200);
		vTaskDelay(100);

		//condicionales para indicadores LED
		if(((value[0]/1000)+48)>valmax){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);//PRENDE
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);//APAGAR
			data[18]='O', data[19]='n', data[20]=' ';
			data[30]='O', data[31]='f', data[32]='f';
			lcd_send_cmd(0xc0);
			lcd_send_string("HOT");
		}

		if(((value[0]/1000)+48)<valmin){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);//APAGA
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);//PRENDE
			data[18]='O', data[19]='f', data[20]='f';
			data[30]='O', data[31]='n', data[32]=' ';
			lcd_send_cmd(0xc0);
			lcd_send_string("COLD");
		}
		if(((value[0]/1000)+48) == 50){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);//APAGA
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);//APAGA
			data[18]='O', data[19]='f', data[20]='f';
			data[30]='O', data[31]='f', data[32]='f';
			lcd_send_cmd(0xc0);
			lcd_send_string("MILD");
		}
		xSemaphoreGive(i2cMutexHandle);
		//imprimir temp en uart
		uart_Temp();

		//osDelay(1000);//cada seg
		vTaskDelay(1000);
	}
}

/**
 * @brief  Function implementing the TaskDebounce thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_TaskDebounce_init */
void Task_Debounce_init(void const * argument)
{
	for(;;){
	leftButton();
	midButton();
	rightButton();
	}
}


void rightButton(void){
	//////////////////DERECHA/////////////////////////////////////////////////////////////////////////

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_RESET){  // se presionasdsds
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,GPIO_PIN_SET);          //prende
		button[2]='R';
  		data[55]='R';
	}
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,GPIO_PIN_RESET);	      //apagado
		button[2]='-';
  		data[55]='-';
	}
}
void midButton(void){

	///////////////MEDIO///////////////////////////////////////////////////////////////////////////
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==GPIO_PIN_RESET){  // se presiona
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET);          //prende
		button[1]='M';
	  	  data[54]='M';
	}
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET);	      //apagado
		button[1]='-';
	  	  data[54]='-';
	}
}
void leftButton(void){

	/////////////////IZQUIERDA//////////////////////////////////////////////////////////////////////////
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==GPIO_PIN_RESET){  // se presiona
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,GPIO_PIN_SET);          //RESET LED APAGA
		button[0]='L';
		 data[53]='L';
	}
	else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,GPIO_PIN_RESET);	      //SET LED PRENDE
		button[0]='-';
		  data[53]='-';
	}
}

/**
 * @brief  Function implementing the UART.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END  */
void uart_Temp(void){

	xSemaphoreTake(uartMutexHandle, portMAX_DELAY);

	data[6]=((value[0]/1000)+48);
	data[7]=(((value[0]%1000)/100)+48);
	data[8]=('C');

	data[40]= '3';//valmax-20;
	data[41]= '0';
	data[42]=('C');

	data[50]= '2';//valmin-20;
	data[51]= '0';
	data[52]=('C');

	xSemaphoreGive(uartMutexHandle);
}

/**************************************************************************************/

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

	//ENTRADAS
	/*Configure GPIO pin : PB0 B10 B4 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//SALIDAS
	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
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

}


/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */

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
