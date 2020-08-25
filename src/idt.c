/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (uint32_t) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/


// segsel = GDT_CODIGO_0

//              15 14 13 12    8 7   5 4         0
// atributos = |p | dpl | 0D110  |0 0 0| reservado|
//              1   11    01110   0 0 0    00000   
//           = 1110 1110 0000 0000 = 0xEE00




#define IDT_ENTRY(numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) CODE0_SEG;                                                          \
    idt[numero].attr = (uint16_t) ENTRY_INT_ATTR;                                                          \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);

#define IDT_ENTRYDPL3(numero)                                                                          \
    idt[numero].offset_0_15 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);        \
    idt[numero].segsel = (uint16_t) CODE0_SEG;                                                          \
    idt[numero].attr = (uint16_t) ENTRY_INT_ATTRDPL3;                                                          \
    idt[numero].offset_16_31 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);





void idt_init() {
    // Excepciones
    IDT_ENTRY(0);          // division por 0  ,         fault 
    IDT_ENTRY(1);          // debug exception ,         trap 
    IDT_ENTRY(2);          // nmi interruption ,        interrupt
    IDT_ENTRY(3);          // breakpoint ,              trap 
    IDT_ENTRY(4);          // overflow ,                trap 
    IDT_ENTRY(5);          // bound range   ,           fault 
    IDT_ENTRY(6);          // invalid opcode            fault
    IDT_ENTRY(7);          // devide not available      fault
    IDT_ENTRY(8);          // double fault              fault
    IDT_ENTRY(9);          // compresor segment overrun fault 
    IDT_ENTRY(10);         // invalid TSS               fault
    IDT_ENTRY(11);         // segment not present       fault
    IDT_ENTRY(12);         // stack-segment fault       fault
    IDT_ENTRY(13);         // general protection        fault
    IDT_ENTRY(14);         // page fault                fault
    IDT_ENTRY(15);         // reserved
    IDT_ENTRY(16);         // floating point error      fault
    IDT_ENTRY(17);         // aligment check            fault
    IDT_ENTRY(18);         // machine check             fault
    IDT_ENTRY(19);         // SIMD floating point exc   fault
    IDT_ENTRY(32);         // Clock 
    IDT_ENTRY(33);         // Teclado
    IDT_ENTRYDPL3(137);    // syscalls
    IDT_ENTRYDPL3(138);
    IDT_ENTRYDPL3(139);
}


int strlen ( char* string){
    int res = 0;
    for (int i =0 ; string[i] !=0 ; i++){
        res++;
    }
    return res;
}


uint8_t excepcion_con_errorCode(uint8_t excepcion){
    if(excepcion == 8 || excepcion == 10 || excepcion == 11 || excepcion == 12 || excepcion == 13 || excepcion == 14 || excepcion == 17 || excepcion == 21){
        return 1;    
    }else{
        return 0;    
    }
}