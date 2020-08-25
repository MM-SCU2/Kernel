/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "stdint.h"
#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"
#include "task.h"
#include "isr.h"
#include "tss.h"

typedef enum e_taskType {
    RickC137       = 1,
    MortyC137      = 2,
    RickD248       = 3,
    MortyD248      = 4,
    Cronenberg     = 5,
    CronenbergC137 = 6,
    CronenbergD248 = 7,
} e_taskType_t;



// =====================================================
//########## FUNCIONES DE INICICIO DE JUEGO Y POSICIONES 
// =====================================================
void game_init();
void I_AM_RICK(uint32_t codigo_ID);
void game_update(uint32_t  tarea_a_correr);
uint32_t modulo(int32_t pos_tarea, int32_t pos_nueva);
// ==================================================
// ############# FUNCIONES QUE ATIENDAN A LAS SYSCALL 
// ==================================================


uint32_t armaDisponible (  int32_t columna , int32_t fila , uint32_t cross ,uint32_t with_morty);
void valor_dezplazamiento (tarea morty, int32_t* x,int32_t* y );

// ==================================================
// ############# FUNCIONES DE MAPEO Y PORTALES
// ==================================================

void nuevo_mapeo ( uint32_t cr3, uint32_t dir_virtual , uint32_t dir_fisica, uint32_t attRW, uint32_t attrUS, uint32_t codigo_tarea);
void cruzar_portal ( int32_t x, int32_t y);
void crear_portal( int32_t pos_x, int32_t pos_y);
void desplazamiento( int32_t* x , int32_t* y);
void cruzar_morty ( tarea morty , int32_t columna , int32_t pos_fila);



uint32_t modulo(int32_t pos_tarea, int32_t pos_nueva);
// ==================================================
// ############# FUNCIONES DE RECONOMIENTO  
// ==================================================
uint8_t esRick( uint8_t rick);
uint8_t esMorty ( tarea morty);
uint8_t hayCronenberg ( uint32_t columna, uint32_t fila);
tarea obtener_morty(uint8_t rick);



// ======================================================
// $$$$$$$$$$ FUNCIONES QUE CHEQUEAN EL ESTADO DEL JUEGO
// ======================================================
void aumentar_portales_de_rick(uint8_t rick );
uint8_t numero_dePortales_De_rick(tarea morty);
void matar_mente_poseida(tarea jugador , uint32_t codigo_ID);
uint16_t check_endGame();
#endif  /* !__GAME_H__ */
