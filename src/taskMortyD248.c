/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 
#include "i386.h"

void task() {
    

    while(1) {
        syscall_usePortalGun(4, 0, 1, 0);
        syscall_IamRick(0xD248);
    };
  

}