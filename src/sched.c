/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "defines.h"
extern uint8_t numero_de_cronenbergs;

uint16_t scheduler [24];

uint16_t tarea_actual = 0; 					// indices del scheduler 

// voy a usar esto para iniciar las tareas que faltan 
// el scheduler por ahora va a tener un arreglo de los indices que le corresponden a las tareas en la gdt 
void sched_init() {
	
	uint16_t selector_tarea_actual = 16;		 	// donde empiezan las tareas en la gdt 

	for (uint16_t i = 0 ; i < 24 ; i++){
		scheduler[i] =(selector_tarea_actual << 3); // selectores de segmentos 
		selector_tarea_actual ++;
	}

}

uint16_t sched_nextTask() {

	uint16_t selector_a_saltar = scheduler[tarea_actual];
	//print_dec((uint32_t)selector_a_saltar >> 3,12,79,40,0xff);
	tarea_actual++;
	if(tarea_actual == 24) tarea_actual = 0;
	return selector_a_saltar;
	
}



void matar_del_Scheduler( uint16_t selectorDeSegmento){

	for (uint16_t i = 0; i< 24 ; i++){
		if(  scheduler[i] ==  selectorDeSegmento ){
			scheduler[i] = (15<<3);
			uint16_t indice_deMuerte =( selectorDeSegmento>>3 ) - 15;
			array_tareas[indice_deMuerte].estado = 0;
			// disminuye el numero total de cronenbergs 
			if ( array_tareas[indice_deMuerte].tipo >= 5){
				numero_de_cronenbergs --;
			}

		}
	}

}