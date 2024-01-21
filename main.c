/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef volatile uint32_t MyReg;

typedef struct MyGPIO
{
	// Mode - input or output
	MyReg MODER;
	// Push&Pull or open drain
	MyReg OTYPER;
	// I/O speed
	MyReg OSPEEDR;
	// Pull-up or pull-down
	MyReg PUPDR;
	// Read
	MyReg IDR;
	MyReg ODR;
	//Write pin
	MyReg BSRR;
	MyReg DUMMY1[3];
	MyReg BRR;
}MyGPIO;

typedef struct MyPWR{
	MyReg CR1;
	MyReg CR2;
}MyPWR;

typedef struct MyTimer{
	MyReg TIM6_CR1;
	MyReg D[3];
	MyReg TIM6_SR;
	MyReg TIM6_EGR;
	MyReg D1[4];
	MyReg TIM6_PSC;
	MyReg TIM6_ARR;
}MyTimer;

/*
resetting &= ~
setting |=
registers :: page 81
 */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SWITCH_PERIOD_MS 1000
#define FRAME_PERIOD_MS 1

#define PRESCALLER_VALUE 3
#define TIM6_ENABLE (1<<4)
#define TIM6_CONTROL_REG1_CONFIG_MASK (1 << 0 | 1<<7 )




#define INPUT 1
#define OUTPUT 0

#define LOW 0
#define HIGH 1



#define TIM6_FREQUENCY 4000000

#define PWR_CR2_ADR 0x40007000
#define GPIOB_ADR 0x48000400
#define GPIOE_ADR 0x48001000
#define GPIOG_ADR 0x48001800
#define TIM6_ADR 0x40001000

#define DISP_SA (uint8_t)0
#define DISP_SB (uint8_t)1
#define DISP_SC (uint8_t)2
#define DISP_SD (uint8_t)3
#define DISP_SE (uint8_t)4
#define DISP_SF (uint8_t)5
#define DISP_SG (uint8_t)6

#define CATHODE1 2
#define CATHODE2 3
#define CATHODE3 4
#define CATHODE4 5

const uint8_t SEGMENTS[7] = {DISP_SA, DISP_SB, DISP_SC, DISP_SD, DISP_SE, DISP_SF, DISP_SG};

struct MyPWR *Power = (MyPWR*)PWR_CR2_ADR;
struct MyGPIO *MyGPIOB = (MyGPIO*)GPIOB_ADR;
struct MyGPIO *MyGPIOE = (MyGPIO*)GPIOE_ADR;
struct MyGPIO *MyGPIOG = (MyGPIO*)GPIOG_ADR;

struct MyTimer *MyTIM6 = (MyTimer*)TIM6_ADR;


#define JOY_OK 15

int16_t counter = 0;



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const uint8_t DIGITS[10][7] = {
		{1, 1, 1, 1, 1, 1, 0},
		{0, 1, 1, 0, 0, 0, 0},
		{1, 1, 0, 1, 1, 0, 1},
		{1, 1, 1, 1, 0, 0, 1},
		{0, 1, 1, 0, 0, 1, 1},
		{1, 0, 1, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 1, 1},
		{1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 0, 1, 1}
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

uint8_t digitalRead(struct MyGPIO *port, uint8_t Pin)
{
	//input data register - read only
	return !((port->IDR & (1<<Pin)) == 0);
}

void digitalWrite(struct MyGPIO *port, uint8_t Pin, uint8_t state)
{
    if (state)
    {
    	port->BSRR |= (1 << Pin);
    }
    //reset pin
    else
    {
    	port->BRR |= (1 << Pin);
    }
}





void timer_6_init(){
	RCC -> APB1ENR1 |= TIM6_ENABLE;
	MyTIM6 -> TIM6_CR1 = TIM6_CONTROL_REG1_CONFIG_MASK;
	MyTIM6 -> TIM6_PSC = PRESCALLER_VALUE;
	MyTIM6 -> TIM6_ARR = 1000;
	MyTIM6 -> TIM6_EGR = (1<<0);
}


void my_delay_ms(uint16_t ms)
{
	//MyTIM6 -> TIM6_ARR = 1000*ms;
	while((MyTIM6->TIM6_SR & (1<<0))==0){}

	MyTIM6->TIM6_SR &= ~(1<<0);
}







void set_digit(const uint8_t digit[]){
	for(uint8_t i = 0; i < 7; i++){
		digitalWrite( MyGPIOG, SEGMENTS[i],digit[i]);
	}
}




void display_counter()
{
	uint32_t i = 0;
	int32_t d1 = (int32_t)((double)counter / 1000) % 10;
	int32_t d2 = (int32_t)((double)counter / 100) % 10;
	int32_t d3 = (int32_t)((double)counter / 10) % 10;
	int32_t d4 = counter % 10;

	uint32_t frames_per_period = SWITCH_PERIOD_MS / (4 * FRAME_PERIOD_MS);

	while(i < frames_per_period)
	{
		set_digit(DIGITS[d4]);
		digitalWrite(MyGPIOB, CATHODE4, HIGH);
		my_delay_ms(FRAME_PERIOD_MS);
		digitalWrite(MyGPIOB, CATHODE4, LOW);

		set_digit(DIGITS[d3]);
		digitalWrite(MyGPIOB, CATHODE3, HIGH);
		my_delay_ms(FRAME_PERIOD_MS);
		digitalWrite(MyGPIOB, CATHODE3, LOW);

		set_digit(DIGITS[d2]);
		digitalWrite(MyGPIOB, CATHODE2, HIGH);
		my_delay_ms(FRAME_PERIOD_MS);
		digitalWrite(MyGPIOB, CATHODE2, LOW);

		set_digit(DIGITS[d1]);
		digitalWrite(MyGPIOB, CATHODE1, HIGH);
		my_delay_ms(FRAME_PERIOD_MS);
		digitalWrite(MyGPIOB, CATHODE1, LOW);
		i++;
	}

}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */








int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* USER CODE BEGIN Init */
	HAL_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  timer_6_init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  while(1){
	  display_counter();

	  counter += 2 * (int32_t)digitalRead(MyGPIOE, JOY_OK) - 1;
	if(counter < 0){counter = 9999;}
	if(counter > 9999){counter = 0;}


  }
  /* USER CODE BEGIN WHILE */
  /* USER CODE END 3 */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

void initGPIO(struct MyGPIO *port, uint32_t rcc_config, uint8_t Pin, int Mode)
{
	// Enable the clock for the given GPIO port
	RCC->AHB2ENR |= rcc_config;

	if(Mode == OUTPUT)
	{
		//output type
		port->MODER |= 1 << (Pin * 2);
		port->MODER &= ~(1 << (Pin * 2 + 1));
		//no pull up
		port->PUPDR &= ~(1 << (Pin * 2));
	}

	if (Mode == INPUT)
	{
		//input type
		port->MODER &= ~(1 << (Pin * 2));
		port->MODER &= ~(1 << (Pin * 2 + 1));
		//with pull up
		port->PUPDR |= (1 << (Pin * 2));
		port->PUPDR &= ~(1 << (Pin * 2 + 1));
	}
	//pp
	port->OTYPER &= ~(1 << Pin);
	//speed
	port->OSPEEDR &= ~(1 << (Pin * 2));
	port->OSPEEDR &= ~(1 << (Pin * 2 + 1));

}

void initVDD()
{
	Power -> CR2 |= (1 << 9);
}

static void MX_GPIO_Init(void)
{

/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */
  for(int i=2; i<6; i++)
  {
	  initGPIO(MyGPIOB, RCC_AHB2ENR_GPIOBEN, i, OUTPUT);
  }
  for(int i=0; i<7; i++)
    {
  	  initGPIO(MyGPIOG, RCC_AHB2ENR_GPIOGEN, i, OUTPUT);
    }
  initGPIO(MyGPIOE, RCC_AHB2ENR_GPIOEEN, JOY_OK, INPUT);

  initVDD();


/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
	while(1);
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
