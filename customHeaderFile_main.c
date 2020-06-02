/*
  EECS461
  Lab 1
  Read Chapter 11&12 in S32K144 User's Manual
  Updated: March 17th, 2014 by Scott Kenyon
           14 Jun 2014 by GPCz
           19 Jun 2014 by GPCz
           2017-03 RMC
           2018-03 Paul Domanico
           15 Nov 2018 jfr
*/

#include "eecs461.h"
#include <stdint.h>
#include "lab1header.h"

/* Change section number to match the section (1-3) being tested */
#define SECTION 1

#if SECTION == 1



/******************************************************************************
 * Function:    section1
 * Description: read and write to the GPIO registers using your header file
 ******************************************************************************/
void section1(){
  /* code for section 1 */

  volatile uint32_t * const portD_PCR = (uint32_t *)(BASE_PORTD);
   volatile uint32_t * const portE_PCR = (uint32_t *)(BASE_PORTE);
   GPIO_mem * const gpioD = (GPIO_mem *)(BASE_GPIO + GPIOD_OFFSET);
   GPIO_mem * const gpioE = (GPIO_mem *)(BASE_GPIO + GPIOE_OFFSET);
  uint16_t sum, value1, value2;
  uint32_t regReadVal;
  int index;
  /* Configure the LED pins */
  for(index = 0; index < 16; index++){
   /* Configure pin mux to gpio */
   portD_PCR[index] = 0b001 << 8;
   /* Set the direction to output */
   gpioD->PDDR |= 0b1 << index;
  }
  /* Configure the DIP pins */
  for(index = 6; index < 14; index++){
   /* Configure pin mux to gpio */
   portE_PCR[index] = 0b001 << 8;
   /* Set the direction to input */
   gpioE->PDDR &= ~(1 << index);
  }

  while(1)
  {
   value1 = 0;
   value2 = 0;
   sum = 0;


   /* Read the GPIOE input register, extract the two 4 bit numbers
   set on dipswitches 1-4 and 5-8 */
   value1 = (gpioE->PDIR)&(0b1111000000);
   value2 = (gpioE->PDIR)&(0b11110000000000);
   value1 = value1 >> 6;
   value2 = value2 >> 10;
   sum = value1 + value2;
   gpioD->PDOR = sum;
   //printf("%d",sum);
   /* Calculate the sum and write to the GPIOD output register
   to display the results on LEDs1-5. */

  }

}


#elif SECTION == 2
#include "S32K144.h"
#include "gpio.h"
/******************************************************************************
 * Function:    section2
 * Description: Use the provided S32K144.h header file
 ******************************************************************************/
void section2()
{
  /* code for section 2 */
  initGPDO(3,0);
  initGPDO(3,1);
  initGPDO(3,2);
  initGPDO(3,3);
  initGPDO(3,4);

  initGPDI(4,6);
  initGPDI(4,7);
  initGPDI(4,8);
  initGPDI(4,9);
  initGPDI(4,10);
  initGPDI(4,11);
  initGPDI(4,12);
  initGPDI(4,13);

  int c[5];
  c[0]=0;
  int n = 0;
  int val;
  int i,j, val1, val111, val222, val11, val12, val13, val14, val21, val22, val23, val24;
  while(1)
  {


  val11 = 1*readGPIO(4,6);
  val12 = 2*readGPIO(4,7);
  val13 = 4*readGPIO(4,8);
  val14 = 8*readGPIO(4,9);

  val21 = 1*readGPIO(4,10);
  val22 = 2*readGPIO(4,11);
  val23 = 4*readGPIO(4,12);
  val24 = 8*readGPIO(4,13);

  val111 = val11 + val12 + val13 + val14;
  val222 = val21 + val22 + val23 + val24;
  val = val111 + val222;

  for(i=0;i<5;i++)
  {
    writeGPIO(3,i,(val>>i)&1);
  }

    /*for(i=6,j=10;i<=9,j<=13;i++,j++)
  {
    value += readGPIO(4,i)
    writeGPIO(3,n,(readGPIO(4,i)^readGPIO(4,j)^c[n]));
    c[n+1] = (readGPIO(4,i)&readGPIO(4,j)) | c[n]&(readGPIO(4,i)^readGPIO(4,j));
    n++;
  }
  //writeGPIO(3,n,c[n])
  */
  }

}


#elif SECTION == 3
#include "S32K144.h"
#include "LPUART.h"
#include "gpio.h"
/******************************************************************************
 * Function:    section3
 * Description: Communicate with the computer using the keyboard interface
 ******************************************************************************/
void section3(){
  /* code for section 3 */
  char byte_in, old_byte_in;

  int index;

  volatile uint32_t * const portD_PCR = (uint32_t *)(BASE_PORTD);
   GPIO_mem * const gpioD = (GPIO_mem *)(BASE_GPIO + GPIOD_OFFSET);
   /* Configure the LED pins */
   for(index = 0; index < 16; index++){
    /* Configure pin mux to gpio */
    portD_PCR[index] = 0b001 << 8;
    /* Set the direction to output */
    gpioD->PDDR |= 0b1 << index;
   }

//  writeGPIO(3,0,2);

  int i;
  int first, second;
  int res;

  /* LPUART functions for sending and receiving serial data */
  LPUART1_init();
  LPUART1_transmit_string("\n\rSerial Output Enable...........");
  while(1) {
    old_byte_in = byte_in;
    //byte_in = LPUART1_receive_char(); // returns ASCII representation
    //LPUART1_transmit_char(byte_in);
    int first = LPUART1_receive_char()-'0';
    while(first>=0 && first<=9)
    {
      second = LPUART1_receive_char()-'0';
      if(second>=0 && second<=9)
      {
        res = first + second;
        gpioD->PDOR = res;
        break;
      }
    }

    /*int second = LPUART1_receive_char()-'0';
    if((first>=0 && first<=9)&&(second >=0 && second<=9))
    {
      int res = first + second;
      char res2 = (char)res + '0';
      gpioD->PDOR = res;
      LPUART1_transmit_char(res2);
    }
*/

  }
}
#endif

int main(){
  /* Always call initEECS461() before doing anything else */

  initEECS461();
  #if(SECTION == 1)
  section1();
  #elif(SECTION == 2)
  section2();
  #elif(SECTION == 3)
  section3();
  #endif

  /* Should never reach this point */
  return 0;
}
