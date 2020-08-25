/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "stdint.h"
#include "screen.h"
#include "tss.h"


void sched_init();

uint16_t sched_nextTask();

void matar_del_scheduler( uint16_t selectorDeSegmento);

extern void sched_init();
extern uint16_t sched_nextTask();
extern void matar_del_scheduler( uint16_t selectorDeSegmento);

#endif	/* !__SCHED_H__ */
