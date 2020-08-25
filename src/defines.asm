;/* ** por compatibilidad se omiten tildes **
;================================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
;================================================================================

;   Definiciones globales del kernel..
;   */




; DEFINICIONES DE OFFSETS A SEGMENTOS

%define RING_0CS            9 << 3
%define RING_0DS            11 << 3 


; PILA DEL KERNEL 

%define Kernel_ebp			0x27000
%define Kernel_esp			0x27000


; DEFINICIONES PARA LA PANTALLA 
%define CARACTER_LIMPIO 		32 << 8 | (0x0 << 4) | 0x0



; DEFINICIONES DE PAGINACION 	
%define KERNEL_PAGE_DIR			0x00027000

; DEFINICIONES DE TSS	

%define GDT_TAREA_INICIAL  	 (  14 << 3)
%define GDT_TASK_IDLE		 (	15 << 3)

%define INICIO_PAGINAS_LIBRES_TASK       0x400000