
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
	init_external_interrupts(); //instantiates the interrupt 

	while(1)
	{
		if((interrupt_count%2)!=0) //checks to see if interrupt count is odd for functioning
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
		else //checks to see if interrupt count is even for 'off' state
		{
			lcd_command(CLEAR); //clears LCD
			count=0;			//Resets the count
			display_on_LEDs(count); //turns off LEDs
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
					  GPIO_MODER_MODER2|
					  GPIO_MODER_MODER3);

	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR1_0|
					 GPIO_PUPDR_PUPDR2_0|
					 GPIO_PUPDR_PUPDR3_0);

}

void init_external_interrupts(void){

	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; 	//enable clock
	SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA; //PA3 to EXTI1 register
	EXTI -> IMR |= EXTI_IMR_MR3;  					//unmask interrupt
	EXTI -> FTSR |= EXTI_FTSR_TR3;					//falling edge trigger
	NVIC_EnableIRQ(EXTI2_3_IRQn);					//NVIC handler

}

void EXTI2_3_IRQHandler(void){
	interrupt_count++;	//increments the interrupt count 
	delay(40000);		//needed to overcome switch bounce. makes program very stable
	if((interrupt_count%2)!=0){
		display_on_LCD(0);	//when system is in 'on' state, it displays a 0
	}
	EXTI -> PR |= EXTI_PR_PR3; //resets interrupt
	}