/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "stdint.h"
#include "defines.h"
#include "i386.h"
#include "gdt.h"
#include "mmu.h"
#include "task.h"



typedef struct str_tss {
    uint16_t  ptl;
    uint16_t  unused0;
    uint32_t    esp0;
    uint16_t  ss0;
    uint16_t  unused1;
    uint32_t    esp1;
    uint16_t  ss1;
    uint16_t  unused2;
    uint32_t    esp2;
    uint16_t  ss2;
    uint16_t  unused3;
    uint32_t    cr3;
    uint32_t    eip;
    uint32_t    eflags;
    uint32_t    eax;
    uint32_t    ecx;
    uint32_t    edx;
    uint32_t    ebx;
    uint32_t    esp;
    uint32_t    ebp;
    uint32_t    esi;
    uint32_t    edi;
    uint16_t  es;
    uint16_t  unused4;
    uint16_t  cs;
    uint16_t  unused5;
    uint16_t  ss;
    uint16_t  unused6;
    uint16_t  ds;
    uint16_t  unused7;
    uint16_t  fs;
    uint16_t  unused8;
    uint16_t  gs;
    uint16_t  unused9;
    uint16_t  ldt;
    uint16_t  unused10;
    uint16_t  dtrap;
    uint16_t  iomap;
} __attribute__((__packed__, aligned (8))) tss;


typedef struct task_task{
    uint8_t  tipo;
    uint8_t estado;
    uint8_t conquistador;
    int32_t columna;
    int32_t fila;
    uint32_t cr3;
    uint32_t dondePortalcol;
    uint32_t dondePortalfil;
} tarea; 


// array de tss y arreglo de estructuras con la informacion de las tareas
extern tss tss_entries [];
extern tarea array_tareas [];



extern tss tss_initial;
extern tss tss_idle;

void tss_init();
uint32_t new_tss_entry();
uint32_t next_gdt_entry();
void New_task(uint32_t task_base_virtual_address, uint32_t task_base_physical_address,uint8_t indiceTarea);
void setDatos ( uint32_t x , uint32_t y , uint8_t indiceTarea);
uint32_t obtenerCodigo(uint8_t indiceTarea);

#endif  /* !__TSS_H__ */
