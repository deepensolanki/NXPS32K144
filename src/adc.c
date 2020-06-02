/*
 * adc.c
 *
 *  Created on: Feb 3, 2020
 *      Author: dsolanki
 */


#include "adc.h"
void init_ADC0_single(void)
{
 /* Table 27-9 Peripheral module clocking */
 /* 29.6.19 PCC ADC0 Register (PCC_ADC0) */
  //PCC->PCCn[PCC_ADC0_INDEX] &=~ PCC_PCCn_CGC_MASK;
  PCC->PCCn[PCC_ADC0_INDEX] &=~ (1<<30);/* Disable clock to change PCS */
 PCC->PCCn[PCC_ADC0_INDEX] &= 0b000111111111111111111111111;
 PCC->PCCn[PCC_ADC0_INDEX] |= 0b001000000000000000000000000; /* Select clock option 1 */
 PCC->PCCn[PCC_ADC0_INDEX] |= (1<<30); /* Enable clock */

 /*42.4.2 - ADC Status and Control Register 1 (SC1AA - SC1Z)*/

 ADC0->SC1[0] |= 0b11111; /* Disable Module */
 ADC0->SC1[0] &=~ (1<<6); /* Disable interrupts */

 /*42.4.3 - ADC Configuration Register 1: CFG1 */
 ADC0->CFG1 |= 0b00; /* Alternate clock 1 */
 ADC0->CFG1 &=~ 0b00<<5; /* Prescaler=1 */
 ADC0->CFG1 |= ADC_CFG1_MODE(0b01);/* 12-bit conversion */

 /*42.4.4 - ADC Configuration Register 2: CFG2 */
 ADC0->CFG2 |= 0b1100; /* set sample time to 13 ADC clks */


 /*42.4.7 - Status and Control Register 2: SC2 */
 ADC0->SC2 &=~ (1<<6); /* SW trigger */
 ADC0->SC2 |= 0b00; /* use voltage reference pins VREFH & VREEFL */
/*42.4.8 - Status and Control Register 3: SC3 */
 ADC0->SC3 &=~ (1<<7); /* Do not start calibration sequence */
 ADC0->SC3 &=~ (1<<3); /* One conversion performed (single mode) */
 ADC0->SC3 &=~ (1<<2); /* HW average function disabled */
}

uint8_t ADC0_complete(void)
{
 /* 42.4.2.4 - COCO flag */
 return ((ADC0->SC1[0]>>7)&1); /*return COCO flag*/
}

uint32_t read_ADC0_single(uint16_t inputChannel)
{
 uint16_t adc_result=0;
 /* inform students of the mask and functions in header for ADCH */
 ADC0->SC1[0] &=~ 0x1F;

 ADC0->SC1[0] |= (inputChannel & 0b11111); /* Initiate Conversion*/

 while(ADC0_complete() == 0); // wait for completion
 /* 42.4.31.2 - Data Result Register*/

 adc_result = ADC0->R[0] & 0xFFF; /* For SW trigger mode, R[0] is used */

 //return (adc_result);
 return ((adc_result*5000)>>12);
}
