/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


unsigned int proxima_pagina_libre_kernel;
unsigned int proxima_pagina_libre_task;

//page size = 4k = 2(12). podemos usar 4096 bytes porque el direccionamiento (puntero) es a byte


void mmu_init() {
	proxima_pagina_libre_kernel = INICIO_PAGINAS_LIBRES_KERNEL;
    proxima_pagina_libre_task = INICIO_PAGINAS_LIBRES_TASK;
}

 uint32_t mmu_nextFreeKernelPage() {
    uint32_t pagina_libre = proxima_pagina_libre_kernel;
    proxima_pagina_libre_kernel += PAGE_SIZE;
    return pagina_libre;
}



void mmu_mapPage(uint32_t cr3, uint32_t virtual, uint32_t phy, uint32_t attrLec_wrt, uint32_t attrU_S) {
    //  indice de la PDT 
    uint32_t indice_PDE = virtual >> 22;
    // indice de la PT , se pone en 0 los bits que no importan 
    uint32_t indice_PTE = virtual >> 12 & 0x3FF;

    //uint32_t offset = (virtual << 20 ) >> 20;

    // puntero al arreglo de PDT
    page_directory_entry* PD = (page_directory_entry*) cr3;
    
    // puntero al arreglo de PTE 


    // si no es una PT valida hay que inicializarla pidiendo una pagina  
    if ( !PD[indice_PDE].present ){

        page_table_entry*  PTNew = (page_table_entry*) mmu_nextFreeKernelPage();

        // inicializo todas sus paginas en 0
        for (int i = 0 ; i< 1024 ; i++){
            PTNew[i]=(page_table_entry){0};
        }

        // inicializo los atributos importantes de la PTE
        PD[indice_PDE].present  = 1;                       
        PD[indice_PDE].read_write = 1;                 
        PD[indice_PDE].user_supervisor = 1;  
        PD[indice_PDE].page_table_base = ((uint32_t) PTNew) >> 12 ;   
    }

    // extraigo la tabla de paginas e inicializo la pagina buscada  

    page_table_entry* PT = (page_table_entry*) (PD[indice_PDE].page_table_base << 12 );         
    PT[indice_PTE].present = 1;
    PT[indice_PTE].read_write = attrLec_wrt;
    PT[indice_PTE].user_supervisor = attrU_S; 
    PT[indice_PTE].physical_address_base = (phy >> 12) ;

    tlbflush();

}

void mmu_unmapPage(uint32_t cr3, uint32_t virtual) {
    //  indice de la PDT 
    uint32_t indice_PDE = virtual >> 22;
    // indice de la PT , se pone en 0 los bits que no importan 
    uint32_t indice_PTE = virtual >> 12 & 0x3FF;

    // puntero al arreglo de PDT
    page_directory_entry* PD = (page_directory_entry*) cr3;
    
    // puntero al arreglo de PTE

    page_table_entry* PT = (page_table_entry*) (PD[indice_PDE].page_table_base << 12);
    
    if ( PT[indice_PTE].present ) PT[indice_PTE].present = 0;


    tlbflush();
}



uint32_t mmu_initTaskDir(  uint32_t direccion_al_mundo_cronberg,uint32_t direccion_fisica_cod_tarea){
    // pagina libre para una PD
    page_directory_entry* cr3_nuevo = (page_directory_entry*) mmu_nextFreeKernelPage(); 
    
    // se inicializa la page directory en 0
    for(int i =0 ; i< 1024 ; i++){
        cr3_nuevo[i] = (page_directory_entry){0};
    }

    // los primeros 4 megas son del kernel, tienen privilegio 0 para supervisor
    uint32_t flag_user_superviser = 0;
    uint32_t flag_read_write = 1;

    // mapeo los primeros 4 megas para que tenga acceso a rutinas de kernel con identity mapping
    for(int i =0 ; i< 1024 ; i++){
        mmu_mapPage((uint32_t) cr3_nuevo,i*4096,i*4096,flag_read_write,flag_user_superviser);
    }

    // el codigo de las tareas esta en el kernel, hay que pasarlos al mundo cronenberg
    // mapeo el kernel al mundo cronenberg y escribo el codigo de la tarea alli  
    mmu_mapPage(KERNEL_PAGE_DIR,direccion_al_mundo_cronberg,direccion_al_mundo_cronberg,flag_read_write,flag_user_superviser);
    mmu_mapPage(KERNEL_PAGE_DIR,direccion_al_mundo_cronberg + PAGE_SIZE ,direccion_al_mundo_cronberg + PAGE_SIZE ,flag_read_write,flag_user_superviser);
    

    // mapeo la direccion fisica en la PD del kernel para poder almacenar codigo
    uint32_t* storage_place = (uint32_t*)direccion_al_mundo_cronberg;
    uint32_t* source = (uint32_t*) direccion_fisica_cod_tarea;

    // se copian las dos paginas de la tarea
    for(int i =0 ; i < 2048 ; i++){
        storage_place[i]= source[i];
    }

    mmu_unmapPage(KERNEL_PAGE_DIR,direccion_al_mundo_cronberg);
    mmu_unmapPage(KERNEL_PAGE_DIR,direccion_al_mundo_cronberg + PAGE_SIZE);

    // mapeo las direcciones virtuales de las tareas (les vendo humo),desde el inicio del area de tareas
    // a las direcciones fisicas que se le pasan por parametro
    
    mmu_mapPage((uint32_t)cr3_nuevo,TASK_CODE,direccion_al_mundo_cronberg,flag_read_write,flag_user_superviser+1);
    // mapeo la segunda pagina (que le corresponde a sus stacks)
    mmu_mapPage((uint32_t)cr3_nuevo,TASK_CODE + PAGE_SIZE ,direccion_al_mundo_cronberg + PAGE_SIZE ,flag_read_write,flag_user_superviser+1);
    

    return (uint32_t)cr3_nuevo;
}


uint32_t mmu_initKernelDir() {
    // puntero a arreglo de PT
    page_directory_entry * pd = (page_directory_entry *) KERNEL_PAGE_DIR;
    
    // puntero a arreglo de PTE 
    page_table_entry * pt_0 = (page_table_entry *) KERNEL_PAGE_TABLE_0;

    // inicializar las entradas de la PDT y y los atributos de la primer PT en 0  
    for (int i = 0 ; i < 1024 ; i++){
        pd[i]= (page_directory_entry){0}; // la notacion es para poner en 0 todos los atributos del struct
        pt_0[i]= (page_table_entry){0};
    }

    // enciendo los bits necesarios de la PDT 
   pd[0].present  = 1;                     
   pd[0].read_write = 1;                   
   pd[0].user_supervisor = 0;     
   pd[0].page_table_base= ((uint32_t) pt_0 >> 12);

   // inicializo las 1024 paginas de la primer PT 

   for (int i = 0 ; i<1024 ; i++){

        pt_0[i].present  = 1;                       
        pt_0[i].read_write = 1;                 
        pt_0[i].user_supervisor = 0;     
        pt_0[i].physical_address_base = i;
   }

   return (uint32_t) pd;
}













