/*
  EECS461
  Lab 4
*/
#include "eecs461.h"
#include "qd.h"
#include "pwm.h"
#include "worlds.h"
#include "adc.h"
#include "gpio.h"
#include "lab1header.h"


int main(void)
{
  int section = 6;
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

  // Initialize QD and ADC
  initQD();
  init_ADC0_single();

  // Initialize PWMs
  initPWMPCRs();
  initPWM(3,0,1000000,0.5);
  initPWM(0,0,20000,0.5);
  float init_angle;
  init_angle = updateAngle();
  while(1)
  { int adcV;
    switch(section)
    {
  case 1: outputTorque(0.0);
    break;
    case 2: adcV = read_ADC0_single(0);
    outputTorque((0.000104*adcV - 0.26)*3162.5);
      break;
    case 3:
      setPWM(0,0, 1000, 0.5);//Change PWM Frequency to 1 kHz torque = 0 N-mm
       /* fill in */
      break;
    case 4: outputTorque(0.0);
       /* fill in */
      break;
    case 5:
      gpioD->PDOR = 2;
      outputTorque(virtualSpring(updateAngle()));
      gpioD->PDOR = 0;
      /* fill in */
      break;
    case 6: // Virtual Wall
      outputTorque(virtualWall(updateAngle()));
      break;
    }
  }
}
