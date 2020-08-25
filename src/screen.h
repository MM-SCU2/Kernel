/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "stdint.h"
#include "colors.h"
#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    uint8_t c;
    uint8_t a;
} ca;
extern uint8_t relojs [];


void print(const char* text, uint32_t x, uint32_t y, uint16_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr);

void screen_drawBox(uint32_t fInit, uint32_t cInit,
                    uint32_t fSize, uint32_t cSize,
                    uint8_t character, uint8_t attr );


void atender_teclado(uint8_t code);
void print_string(const char* text, uint32_t x, uint32_t y, uint16_t attr);
// ======================================================================
// #####$$$$$$$$$$ FUNCIONES QUE ACTUALIZAN LA PANTALLA SEGUN LOS SUCESOS
// ======================================================================
void screen_init();
void colorearJugadores ( uint32_t x,uint32_t y,uint8_t Tipo_Tarea,uint8_t estado,uint8_t conquistador);
void printear_portal( tarea corriendo , uint32_t pos_columna , uint32_t pos_fila);
void printear_personaje( tarea corriendo , uint32_t pos_columna , uint32_t pos_fila);
void imprimir_estados();
void resetear_pantalla ();
void pantalla_final ();
void imprimir_debugging(uint32_t valor_excepcion, uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi, uint32_t ebp, uint32_t ds, uint32_t es, uint32_t fs, uint32_t gs, uint32_t cr0, uint32_t cr1, uint32_t cr2, uint32_t cr3, uint32_t cr4, int32_t errorCode, uint32_t eip,uint32_t cs,uint32_t eflags,uint32_t esp,uint32_t ss);
char* character(uint8_t selector);
void imprimir_status(uint8_t estado, uint8_t conquistador,uint32_t columna,uint32_t fila);
#endif  /* !__SCREEN_H__ */
