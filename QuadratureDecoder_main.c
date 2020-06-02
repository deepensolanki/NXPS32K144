#include "eecs461.h"
#include <stdint.h>
#include "S32K144.h"
#include "LPUART.h"
#include "qd.h"
#include "lab1header.h"

int main()
{
  initEECS461();

  //set up our GPIO for LED
  volatile uint32_t * const portD_PCR = (uint32_t *)(BASE_PORTD);
  GPIO_mem * const gpioD = (GPIO_mem *)(BASE_GPIO + GPIOD_OFFSET);
  int index;
  /* Configure the LED pins */
  for(index = 0; index < 16; index++){
   /* Configure pin mux to gpio */
   portD_PCR[index] = 0b001 << 8;
   /* Set the direction to output */
   gpioD->PDDR |= 0b1 << index;
  }

  int i;
  //initialize all LEDPINS
  for(i = 0; i< 16; i++)
  {
    initGPDO(3,i);
  }

  initQD();
  int32_t counter;
  float a;

  while(1)
    {
      counter = updateCounter();
      a = updateAngle();
      gpioD->PDOR = (int)a;
    }
  return 0;
}
