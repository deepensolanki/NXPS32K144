/*
 * lab3.c
 *
 *  Created on: Feb 3, 2020
 *      Author: dsolanki
 */

#include "eecs461.h"
#include <stdint.h>
#include "S32K144.h"
#include "lab1header.h"
#include "adc.h"
#include "gpio.h"

int main()
{
  uint32_t iAnalog;

  initEECS461();

  volatile uint32_t * const portD_PCR = (uint32_t *)(BASE_PORTD);
  GPIO_mem * const gpioD = (GPIO_mem *)(BASE_GPIO + GPIOD_OFFSET);

  int index;
   /* Configure the LED pins */
  for(index = 0; index < 16; index++)
  {
    /* Configure pin mux to gpio */
    portD_PCR[index] = 0b001 << 8;
    /* Set the direction to output */
    gpioD->PDDR |= 0b1 << index;
   }

  init_ADC0_single();

  while(121)
  {
    gpioD->PDOR |= (1<<2);
    iAnalog = read_ADC0_single(0);
    gpioD->PDOR &=~ (1<<2);
    if(iAnalog>2500)
      gpioD->PDOR = 1;
    else
      gpioD->PDOR = 0;
  }

  return 0;

}
