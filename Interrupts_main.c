/*
 * lab5.c
 *
 *  Created on: Jan 21, 2018
 *      Author: domanico
 *      1-6-19  jfr
 */

#include "S32K144.h"
#include "eecs461.h"

#include "pwm.h"
#include "LPIT.h"
#include "adc.h"
#include "gpio.h"
#include "math.h"
#include "lab1header.h"

#define LPIT0_CHANNEL  0

float sineTable[10]; // Ten samples/period
int sineIndex = 0;
const float PI = 3.14159;

volatile uint32_t * const portD_PCR = (uint32_t *)(BASE_PORTD);
volatile uint32_t * const portE_PCR = (uint32_t *)(BASE_PORTE);
GPIO_mem * const gpioD = (GPIO_mem *)(BASE_GPIO + GPIOD_OFFSET);
GPIO_mem * const gpioE = (GPIO_mem *)(BASE_GPIO + GPIOE_OFFSET);


void IsrA(void){

    int dip1, dip2, PWM_freq;
    float dc;

 	/* Turn on LED */
	gpioD->PDOR = 1;

	/* Read sine value */
	sineIndex = read_ADC0_single(0);

	/* Calculate PWM duty cycle */
	dip1 = readGPIO(4,6);
	dip2 = readGPIO(4,7);
	if(dip2)
        dc =  0.8*(float)(sineIndex/5000.0) + 0.1;
    else
        dc =  0.2*(float)(sineIndex/5000.0) + 0.4;

	/* Set PWM frequency based on dipswitch */
	if(dip1)
        PWM_freq = 60000;
    else
        PWM_freq = 20000;

	/* Set PWM duty cycle and frequency */
	setPWM(0, 0, PWM_freq, dc);

	/* Turn off LED */
	gpioD->PDOR = 0;

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);


}

void IsrB(void){

    static int i = 0;
    float theta;
    float dutyCycle;
    uint32_t PWM_freq = 60000;
	/* Turn on LED */
	gpioD->PDOR = 1;

	/* Calculate and set PWM duty cycle */
	theta = 2*PI*i/10; ;
    i = (i+1)%10;
    dutyCycle = 0.5 + 0.4*sinf (theta);
    setPWM(0, 0, PWM_freq, dutyCycle);


	/* Turn off LED */
	gpioD->PDOR = 0;

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);

}

void IsrC(void){

    uint32_t PWM_freq = 60000;
    float dc;
	/* Turn on LED */
	gpioD->PDOR = 1;

	/* Calculate and set PWM duty cycle */
	dc = sineTable[sineIndex];
    sineIndex = (sineIndex+1)%10;
    setPWM(0, 0, PWM_freq, dc);

	/* Turn off LED */
	gpioD->PDOR = 0;

	/* Clear interrupt flag */
	clearFlagLPIT(LPIT0_CHANNEL);

}

void initializeGPIOs()
{
    int index;

      for(index = 0; index < 16; index++)
    {
        /* Configure pin mux to gpio */
        portD_PCR[index] = 0b001 << 8;
        /* Set the direction to output */
        gpioD->PDDR |= 0b1 << index;

    }

      for(index = 6; index < 14; index++)
    {
        /* Configure pin mux to gpio */
        portE_PCR[index] = 0b001 << 8;
        /* Set the direction to input */
        gpioE->PDDR &= ~(1 << index);

    }
}

int main(){

  initEECS461();
  initializeGPIOs();
  enableLPIT();
  init_ADC0_single();

  /* Initialize PWMs  */
  initPWMPCRs();
  initPWM(MOTOR_SUBMODULE,MOTOR_CHANNEL,MOTOR_FREQUENCY,0.5);

  initPWM(FILTER_SUBMODULE,FILTER_CHANNEL,FILTER_FREQUENCY,0.5);

  /* Initialize GPIO  */

  int i;
  for(i = 0; i < 10; i++)
  {
    sineTable[i] = 0.5 + 0.4*sin(2.0*PI*i/10.0);
  }
  int a;


  initLPIT(LPIT0_CHANNEL, 80000, &IsrA, 0xC);

  while(1){
    //a = readGPIO(4,8);
  }
}

