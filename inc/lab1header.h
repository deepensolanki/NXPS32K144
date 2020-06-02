/*
 * lab1header.h
 *
 *  Created on: Jan 13, 2020
 *      Author: dsolanki
 */

#ifndef LAB1HEADER_H_
#define LAB1HEADER_H_


#define BASE_GPIO 0x400FF000
#define BASE_PORTA 0x400FF000
#define BASE_PORTB 0x400FF040
#define BASE_PORTC 0x400FF080
#define BASE_PORTD 0x4004C000
#define BASE_PORTE 0x4004D000
#define GPIOA_OFFSET 0x00
#define GPIOB_OFFSET 0x40
#define GPIOC_OFFSET 0x80
#define GPIOD_OFFSET 0xC0
#define GPIOE_OFFSET 0x100
#define PDOR_OFFSET 0x00
#define PSOR_OFFSET 0x4
#define PCOR_OFFSET 0x8
#define PTOR_OFFSET 0xC
#define PDIR_OFFSET 0x10
#define PDDR_OFFSET 0x14
#define PIDR_OFFSET 0x18

typedef struct
{
  volatile uint32_t PDOR;
  volatile uint32_t PSOR;
  volatile uint32_t PCOR;
  volatile uint32_t PTOR;
  volatile uint32_t PDIR;
  volatile uint32_t PDDR;
  volatile uint32_t PIDR;
} GPIO_mem;


#endif /* LAB1HEADER_H_ */
