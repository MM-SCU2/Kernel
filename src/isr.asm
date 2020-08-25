 ; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "print.mac"
%include "defines.asm" 
extern armaDisponible 
extern game_update
extern resetear_pantalla
extern game_init
extern I_AM_RICK
extern matar_del_Scheduler
extern imprimir_debugging
extern desplazamiento
extern check_endGame
extern pantalla_final
extern excepcion_con_errorCode


USE32

sched_task_offset:     dd 0x0
sched_task_selector:   dw 0x0
creoPortal:            dd 0x0
modoDebug :            dd 0x0
preservarReg:          dd 0x0 
preservarReg2:         dd 0x0
freezar :              dd 0x0
fila :                 dd 0x0 
columna :              dd 0x0 
end_game :             dw 0x1 

;; PIC
extern pic_finish1
;; Sched
extern sched_nextTask
extern atender_teclado
extern print_dec


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1


_isr%1:

    cmp dword [modoDebug], 1

    ; si no esta el modo debug no hace nada 

    jne .noimprimirDebug 
        ; hay que congelar la pantalla 
        mov dword[freezar],1

        ; Preservo eax.
        mov [preservarReg],eax
        ; numero de error 
        mov eax, %1

        ; chequeo si es una exepcion con codigo de error, eso cambia las cosas en la pila.
        push eax
        call excepcion_con_errorCode
        add esp, 4 
        
        ; si tiene error entonces.
        cmp eax, 1
        je .exepcion_conError
        ; se le dice a la funcion que el error code no es valido .
        push -1
    
    
.exepcion_conError:
    ; Pusheamos los parametros en el caso donde hay codigo de error.
    
    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    push 0                  ; no hay cr1
    mov eax, cr0
    push eax

    push gs
    push fs
    push es
    push ds
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    mov eax,[preservarReg]
    push eax
    push %1
    call imprimir_debugging

   
    ; Si tiene error code, liberamos 17 * 4 bytes.
    ; Si no tiene error code, liberamos 18 * 4 bytes.
    push %1
    call excepcion_con_errorCode
    add esp, 4
    cmp eax, 1
    je .restaurar_pila_conError
    
    ; Pop sin error
    add esp, 18*4
    jmp .noimprimirDebug    

.restaurar_pila_conError:
    add esp, 17*4
    

.noimprimirDebug:

    xor ecx,ecx 
    str cx 
    push ecx 
    
    call matar_del_Scheduler 

    add esp,4
    xor eax,eax 
    mov ax,GDT_TASK_IDLE

    mov [sched_task_selector],ax 
    jmp far [sched_task_offset]

iret


%endmacro



;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17   
ISR 18
ISR 19

;; Rutina de atención del RELOJ

;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
   pushad  
    call pic_finish1
    ; actualiza el juego
    ; imprime el reloj 
    call nextClock

    cmp dword [freezar],1
    jne .Nofreezar
        ; frezar pantalla, flag de debug activado     
        xor eax,eax
        mov ax,GDT_TASK_IDLE
        jmp .detencion

.Nofreezar:
    ; chequea fin de juego 
    call check_endGame
    cmp [end_game], ax 
    jne .noTermina
       ;  invocar a la idle 
        call pantalla_final 
        mov dword [freezar],1
        xor eax,eax
        mov ax,GDT_TASK_IDLE
        jmp .detencion

.noTermina:
    ; envio el flag de debug en 0 

    call sched_nextTask

 ; chequeo las condiciones del juego 
    mov [preservarReg],eax
    mov edx,eax 
    push edx
    call game_update
    add esp,4

    mov eax ,[preservarReg]

.detencion:
    xor ecx,ecx 
    str cx
    
    cmp ax,cx
    je .fin 

        mov [sched_task_selector],ax 
        jmp far [sched_task_offset]

    .fin:

    popad
iret


;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
    pushad 

    call pic_finish1
    in al, 0x60
    cmp al,0x15

    jne .SaltearDebug

        cmp dword [modoDebug],1
        je .desactivar 

           mov dword [modoDebug],1
           jmp .SaltearDebug

        .desactivar:
            mov dword [modoDebug],0
            mov dword [freezar],0
            call resetear_pantalla
            call game_init


 .SaltearDebug:

    push eax
    call atender_teclado
    add esp, 4
    popad
    iret
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr137
global _isr138
global _isr139


; arma de portales 
_isr137:
    pushad 
    push edx
    push ecx
    push ebx 
    push eax

    call armaDisponible

    mov [creoPortal],eax 

    add esp,16
    popad
    mov eax,[creoPortal]
iret

; I AM RICK BITCH
_isr138:
    pushad

    push eax

    call I_AM_RICK

    add esp,4
    
    popad
iret


; where is morty
_isr139:    
    pushad

    mov ebx,columna
    mov eax,fila

    push ebx 
    push eax

    call desplazamiento
    add esp,8


    mov [preservarReg],eax
    mov [preservarReg2],ebx
    popad
    mov eax,[preservarReg]
    mov ebx,[preservarReg2]
    mov eax,[eax]
    mov ebx, [ebx]
iret



;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
nextClock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
        
        popad
        ret
