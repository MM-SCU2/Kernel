/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "syscall.h" 
#include "i386.h"

void task() {
    int x,y;
    syscall_whereIsMorty(&x,&y);
    syscall_usePortalGun(x, y, 1, 1);
    syscall_usePortalGun(4, 4, 1, 0);
     while(1) {
        for(int j=0; j< 40; j++) {
            for(int i=0; i< 80; i++) {
                syscall_usePortalGun(1, 0, 1, 1);
            }
            syscall_usePortalGun(1, 1, 1, 1);
            syscall_IamRick(0xD248);
        }
    }
    
}