/*
 * qd.c
 *
 *  Created on: Jan 27, 2020
 *      Author: dsolanki
 */

#include "qd.h"
#include "eecs461.h"
#include <stdint.h>
#include "S32K144.h"
#include "LPUART.h"

/* Quadrature Decoding Parameters */
#define ENCODER_COUNT 0x0
#define QD_PHA_PORT PORTA
#define QD_PHB_PORT PORTA
#define QD_PHA_PIN 13
#define QD_PHB_PIN 12



float angle;
/******************************************************************************
* Function: init_QD
* Description: Initializes eTimer for Quadrature Decoding
******************************************************************************/
void initQD()
{
/* Initialize Phase A and B input PCR */

QD_PHA_PORT->PCR[QD_PHA_PIN] |= PORT_PCR_MUX(0b110);
QD_PHB_PORT->PCR[QD_PHB_PIN] |= PORT_PCR_MUX(0b110);
/* Set up FTM2 for Quadrature Decode */
FTM2->MODE |= FTM_MODE_WPDIS_MASK; /* Disable write protection (should already be
disabled) */
FTM2->MOD = 0xFFFF;
FTM2->CNTIN = 0x0000;
FTM2->QDCTRL = 0x1;
FTM2->CONF |= FTM_CONF_BDMMODE(0b11); /* Optional: enable in debug mode */
return;
}
/******************************************************************************
* Function: updateCounter
* Description: Returns an updated counter value that keeps track of absolute
* wheel position
******************************************************************************/
int32_t updateCounter()
{

  static int32_t total_count;
  static uint16_t last_count;
  uint16_t curr_count;
  curr_count = FTM2->CNT & FTM_CNT_COUNT_MASK;
  //total_count = (total_count + (curr_count - last_count));
  total_count = (total_count + (int16_t)(curr_count - last_count));
  last_count = curr_count;
  return total_count;
}

/******************************************************************************
* Function: updateAngle
* Description: Returns the angle of the wheel
******************************************************************************/
float updateAngle()
{
  int32_t total_count;
  total_count = updateCounter();
  angle = total_count*0.09;
  return angle;
}
