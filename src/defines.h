/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
#define PAG_P                   0x00000001
#define PAG_R                   0x00000000
#define PAG_RW                  0x00000002
#define PAG_S                   0x00000000
#define PAG_US                  0x00000004

/* Misc */
/* -------------------------------------------------------------------------- */
#define SIZE_N                  40 // X
#define SIZE_M                  80 // Y

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT                     		55
#define GDT_IDX_NULL_DESC             		0
#define GDT_IDX_CODE_LEVEL0_DESC			9
#define GDT_IDX_CODE_LEVEL3_DESC			10
#define GDT_IDX_DATA_LEVEL0_DESC			11
#define GDT_IDX_DATA_LEVEL3_DESC			12
#define GDT_VIDEOSEG                  		13
#define GDT_TAREA_INICIAL             		14
#define GDT_TASK_IDLE                 		15
#define GDT_FIRST_USER_TASK 				16

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC             (GDT_IDX_NULL_DESC << 3)
#define GDT_OFF_DATA_LEVEL3_DESC  	  (GDT_IDX_DATA_LEVEL3_DESC << 3)
#define GDT_OFF_DATA_LEVEL0_DESC	  (GDT_IDX_DATA_LEVEL0_DESC << 3)
#define GDT_OFF_CODE_LEVEL0_DESC	  (GDT_IDX_CODE_LEVEL0_DESC << 3)
#define GDT_OFF_CODE_LEVEL3_DESC	  (GDT_IDX_CODE_LEVEL3_DESC << 3)

/* RPL en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_RPL_DATA_LEVEL3     0x03
#define GDT_RPL_DATA_LEVEL0     0x00
#define GDT_RPL_CODE_LEVEL0     0x00
#define GDT_RPL_CODE_LEVEL3     0x03


/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */



#define CODE0_SEG 	 		 		GDT_IDX_CODE_LEVEL0_DESC << 3
#define CODE3_SEG 	 		 		GDT_IDX_CODE_LEVEL3_DESC << 3	
#define DATA0_SEG				   	GDT_IDX_DATA_LEVEL0_DESC << 3


/* Atributo de las entradas IDT  */
/* -------------------------------------------------------------------------- */

//  1000 1110 0000 0000            0x8E00
#define ENTRY_INT_ATTR 			   	   0x8E00
#define ENTRY_INT_ATTRDPL3			   0xEE00

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR                0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                    0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                     0x000B8000 /* direccion fisica del buffer de video */

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE                 0x08000000 /* direccion virtual del codigo */
#define EFLAGS_DEFECTO            0x00000202
#define STACK_TEMPORAL 			  0x8007000
#define CODIGO_TEMPORAL			  0x8006000



/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

// indices de tareas 


#define rick137   1
#define morty137  2
#define rick248   3
#define morty248  4

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR                   0x00027000
#define KERNEL_PAGE_TABLE_0               0x00028000
#define CRYSTALS_MAP                      0x00029000


#define  INICIO_PAGINAS_LIBRES_TASK       0x400000
#define  INICIO_PAGINAS_LIBRES_KERNEL     0x100000
#define  PAGE_SIZE                        0x00001000


#endif  /* !__DEFINES_H__ */
