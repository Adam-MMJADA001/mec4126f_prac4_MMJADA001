// Description----------------------------------------------------------------|
/*
 *
 */
// DEFINES AND INCLUDES-------------------------------------------------------|

#define STM32F051												   //COMPULSORY
#include "stm32f0xx.h"											   //COMPULSORY
#include "lcd_stm32f0.h"
#include <stdint.h>
#define UCT_BOARD

// GLOBAL VARIABLES ----------------------------------------------------------|
int interrupt_count = 0;
// FUNCTION DECLARATIONS -----------------------------------------------------|

void main(void);                                                   //COMPULSORY
void display_on_LCD(uint8_t count);
void init_LEDs(void);
void display_on_LEDs(uint8_t count);
void init_switches(void);
void init_external_interrupts(void);
void EXTI2_3_IRQHandler(void);
// MAIN FUNCTION -------------------------------------------------------------|

void main(void)
{
	uint8_t count = 0;
	init_LCD();		//used to initiate the LCD
	init_LEDs();
	init_switches();
	display_on_LCD(0); //displays 0 on reset

	while(1)
	{

		 if((GPIOA->IDR & GPIO_IDR_1)==0) //checks to see if button 1 is pressed
		{
			count++; //increments count variable. if count is 255 it automatically resets to 0 and starts again. Therefore there's no need for an if statement
			display_on_LEDs(count); //Displays count value on LEDs
			display_on_LCD(count); //Displays count value on LEDs
			delay(80000);	//delay to negate switch bounce

		}

		else if ((GPIOA->IDR & GPIO_IDR_2)==0){ //checks to see if button 2 is pressed
			count--; //decrements count variable. if count is 0 it automatically resets to 255. Therefore there's no need for an if statement
			display_on_LEDs(count);//Displays count value on LEDs
			display_on_LCD(count); //Displays count value on LEDs
			delay(80000); //delay to negate switch bounce
			}

	}
	}



// OTHER FUNCTIONS -----------------------------------------------------------|
void display_on_LCD(uint8_t count){

	lcd_command(CLEAR);
	char output[3];
	sprintf(output, "%d", count);
	lcd_putstring(output);

}

void init_LEDs(void){

	RCC->AHBENR|= RCC_AHBENR_GPIOBEN;

	GPIOB->MODER |=(GPIO_MODER_MODER0_0|
					GPIO_MODER_MODER1_0|
					GPIO_MODER_MODER2_0|
					GPIO_MODER_MODER3_0|
					GPIO_MODER_MODER4_0|
					GPIO_MODER_MODER5_0|
					GPIO_MODER_MODER6_0|
					GPIO_MODER_MODER7_0);
}

void display_on_LEDs(uint8_t count){
	GPIOB -> ODR &= ~(GPIOB -> ODR);
	GPIOB-> ODR = count;
}

void init_switches(void){

	RCC->AHBENR|= RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~(GPIO_MODER_MODER1|
					  GPIO_MODER_MODER2);

	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR1_0|
					 GPIO_PUPDR_PUPDR2_0);

}

