 /*
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H
#define INTERRUPS_H

#include <stdint.h>

extern uint64_t _irq00Handler;
extern uint64_t _irq01Handler;
extern uint64_t _irq80Handler;

extern uint64_t _exceptionHandler00;
extern uint64_t _exceptionHandler06;

void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

#define TIMER_PIC_MASTER 0xFE
#define KEYBOARD_PIC_MASTER 0xFD
#define NO_INTERRUPTS 0xFF

#endif
