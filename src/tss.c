/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/



#include "tss.h"
#include "game.h"

// variables para ir declarando los segmentos de tareas en la tss y en el array de tss
uint32_t next_gdt_free_index;
uint32_t next_tss_free_index;

uint32_t direccion_mundo_cronenberg;


// arreglo global de tareas 

tarea array_tareas [25];
tss tss_entries [30];


// para llevar registro de las tss inicializadas 
// la tarea inicial y la tarea idle no importan en este arreglo 



// inicializo los valores de la tarea initial

tss tss_initial = {
  .ptl = 0,
      // stacks pointers de 0, 1 y 2 
  .esp0 = 0,
  .ss0 = 0,

     
  .esp1 = 0,
  .ss1 = 0,

  .esp2 = 0,
  .ss2 = 0,
      // page directory y program counter 

  .cr3 = 0x27000, //task_base_virtual_address & 0xFFC00000,
  .eip = 0,
  .eflags = 0x202,    // Interrupciones habilitadas.

      // registeos de proposito general 
  .eax = 0,
  .ecx = 0,
  .edx = 0,
  .ebx = 0,
  .esp = 0,
  .ebp = 0,
  .esi = 0,
  .edi = 0,

  .cs = 0,
  .ds = 0,
  .es = 0,
  .fs = 0,
  .gs = 0,
  .ss = 0,
  .ldt = 0,
  .dtrap = 0,
  .iomap = 0xFFFF,

  .unused0 = 0,
  .unused1 = 0,
  .unused2 = 0,
  .unused3 = 0,
  .unused4 = 0,
  .unused5 = 0,
  .unused6 = 0,
  .unused7 = 0,
  .unused8 = 0,
  .unused9 = 0,
  .unused10 = 0,

};

// inicializo los valores de la tarea idle 

tss tss_idle = {
  .ptl = 0,
      // stacks pointers de 0, 1 y 2 
  .esp0 = 0,
  .ss0 = 0,

  .esp1 = 0,
  .ss1 = 0,

  .esp2 = 0,
  .ss2 = 0,

     // selectores de segmento      
  .ss  =    GDT_OFF_DATA_LEVEL0_DESC ,
  .ds  =    GDT_OFF_DATA_LEVEL0_DESC ,
  .es  =    GDT_OFF_DATA_LEVEL0_DESC ,
  .fs  =    GDT_OFF_DATA_LEVEL0_DESC ,
  .gs  =    GDT_OFF_DATA_LEVEL0_DESC ,
  .cs  =    GDT_OFF_CODE_LEVEL0_DESC ,
     // control register and program counter     

  .cr3  =   0x27000,
  .eip  =   0x1A000,
     
  .ebp  =   0,
  .esp  =   0x27000,
  .eflags = EFLAGS_DEFECTO,
  .iomap = 0xFFFF,

  .ldt = 0,
  .dtrap = 0,

  .unused0 = 0,
  .unused1 = 0,
  .unused2 = 0,
  .unused3 = 0,
  .unused4 = 0,
  .unused5 = 0,
  .unused6 = 0,
  .unused7 = 0,
  .unused8 = 0,
  .unused9 = 0,
  .unused10 = 0,
}; 



void tss_init() {

     // direcciones de la gdt
  gdt[GDT_TAREA_INICIAL].base_0_15 = (uint16_t)((uint32_t) &tss_initial);
  gdt[GDT_TAREA_INICIAL].base_23_16 = (uint8_t)((uint32_t) &tss_initial >> 16);
  gdt[GDT_TAREA_INICIAL].base_31_24 = (uint8_t)((uint32_t) &tss_initial >> 24);

     // direcciones de la gdt
  gdt[GDT_TASK_IDLE].base_0_15 = (uint16_t)((uint32_t) &tss_idle);
  gdt[GDT_TASK_IDLE].base_23_16 = (uint8_t)((uint32_t) &tss_idle >> 16);
  gdt[GDT_TASK_IDLE].base_31_24 = (uint8_t)((uint32_t) &tss_idle >> 24);

  next_gdt_free_index = GDT_FIRST_USER_TASK;
  next_tss_free_index = 1;
}

// para iniciar una tss se necesita una entrada de gdt, una tss con un cr3 valido y la direccion de esa Tss

// para obtener la proxima entrada de gdt 
uint32_t next_gdt_entry (){
  
  uint32_t new_index = next_gdt_free_index;
  next_gdt_free_index++;
  return new_index;
}

uint32_t new_tss_entry (){
  
  uint32_t new_entry = next_tss_free_index ;
  next_tss_free_index ++;
  return new_entry;
}



// para iniciar una nueva tarea se necesita 
// 1) un directorio valido y mapeado
// 2) la entrada correspondiente de gdt 
// 3) una tss que describa el contexto  




void System_tasks (){

  // inicio de video 

  uint8_t* columna =  (uint8_t*)1;
  uint8_t* fila = (uint8_t*) 0;

// se inician las tareas
 
for ( uint8_t i = 1 ; i < 25 ; i++){
      getRandomPosition(i,columna,fila);
      setDatos(  ((uint32_t)* columna) % 80 , ((uint32_t)* fila) % 40, i);
  }
}




void setDatos ( uint32_t columna , uint32_t fila , uint8_t indiceTarea){
  // seteo datos 
  array_tareas[indiceTarea].tipo = indiceTarea;
  array_tareas[indiceTarea].columna=  columna;
  array_tareas[indiceTarea].fila= fila;
  array_tareas[indiceTarea].estado = 1;

  uint32_t codigo = obtenerCodigo(indiceTarea);
  // direccon de la tarea en el mundo cronenberg 
  direccion_mundo_cronenberg = INICIO_PAGINAS_LIBRES_TASK + fila*0x2000*80 + columna*0x2000  ;
  New_task( direccion_mundo_cronenberg, codigo , indiceTarea);

  // coloreo jugadores 
  colorearJugadores(columna,fila,indiceTarea,1,0);

}



void New_task(uint32_t task_base_virtual_address, uint32_t task_base_physical_address ,uint8_t indiceTarea) {
    // Se realiza el ejercicio 6 f).

  uint32_t newPage_directory = mmu_initTaskDir(task_base_virtual_address, task_base_physical_address);
  // guardo el cr3 correspondiente en el struct 
  array_tareas[indiceTarea].cr3 = newPage_directory;

  uint32_t nextFreeTssTableIndex = new_tss_entry();

  tss_entries[nextFreeTssTableIndex] = (tss) {
    .ptl = 0,
    // stacks pointers de 0, 1 y 2 
    .esp0 = mmu_nextFreeKernelPage() + 0x1000,
    .ss0 = (GDT_OFF_DATA_LEVEL0_DESC),

    .esp1 = 0,
    .ss1 = 0,

    .esp2 = 0,
    .ss2 = 0,
    // page directory y program counter 

    .cr3 = newPage_directory, //task_base_virtual_address & 0xFFC00000,
    .eip = 0x8000000,
    .eflags = 0x202,    // Interrupciones habilitadas.

    // registeos de proposito general 
    .eax = 0,
    .ecx = 0,
    .edx = 0,
    .ebx = 0,
    .esp = 0x08000000 + 0x2000,
    .ebp = 0x80000000 + 0x2000, //si bien su limite esta en 8001000 la pila crece en sentido contrario, asi que apunta al final de la pagina.
    .esi = 0,
    .edi = 0,

    .cs = (GDT_OFF_CODE_LEVEL3_DESC)|3,

    .ds = (GDT_OFF_DATA_LEVEL3_DESC )|3,
    .es = (GDT_OFF_DATA_LEVEL3_DESC )|3,
    .fs = (GDT_OFF_DATA_LEVEL3_DESC )|3,
    .gs = (GDT_OFF_DATA_LEVEL3_DESC)|3,
    .ss = (GDT_OFF_DATA_LEVEL3_DESC )|3,

    .ldt = 0,
    .dtrap = 0,
    .iomap = 0xFFFF,

    .unused0 = 0,
    .unused1 = 0,
    .unused2 = 0,
    .unused3 = 0,
    .unused4 = 0,
    .unused5 = 0,
    .unused6 = 0,
    .unused7 = 0,
    .unused8 = 0,
    .unused9 = 0,
    .unused10 = 0,
  };

    // la direccion es su posicion en el array de tss 
  uint32_t dirTss = (((uint32_t) &tss_entries) + (nextFreeTssTableIndex*104) );

  // Se genera un descriptor de segmento de estado de tarea (tss descriptor) en la gdt
  // siguiente posici\'on libre en la gdt.
  uint32_t entry = next_gdt_entry();

  gdt[entry] = (gdt_entry){
    .base_0_15 = (uint16_t) dirTss,
    .base_23_16 = (uint8_t) dirTss >> 16,
    .base_31_24 = (uint8_t) dirTss >> 24,
    .limit_0_15 = 0x0067,
    .limit_16_19 = 0x00,
    .type = 0x9,  
    .s = 0,
    .dpl = 3,
    .p = 1,
    .db = 0,
    .g = 0,
    .avl = 0,
    .l = 0,
  };
}




uint32_t obtenerCodigo(uint8_t indiceTarea){

  if ( indiceTarea == RickC137 ){
    return 0x10000;
  }
  else if ( indiceTarea == MortyC137){
    return 0x12000;
  }
  else if (indiceTarea == RickD248){
    return 0x14000;
  }
  else if (indiceTarea == MortyD248){
    return 0x16000;
  }
  else {
    return 0x18000;

  }
}
