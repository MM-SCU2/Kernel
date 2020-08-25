; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"
%include "defines.asm" 

extern GDT_DESC
extern IDT_DESC
extern idt_init 
extern screen_drawBox
extern pic_reset
extern pic_enable
extern mmu_initKernelDir
extern mmu_mapPage
extern mmu_init
extern tss_init
extern New_task
extern sched_init
extern game_init
extern System_tasks
extern screen_init


global start


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
USE16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    
     ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    ;breakpoint 

    ; Habilitar A20
    call A20_disable
    call A20_check
    call A20_enable
    call A20_check


    ; Cargar la GDT

    lgdt [GDT_DESC]


    ; Setear el bit PE del registro CR0
    
    mov eax, cr0 
    or  eax, 0x1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp RING_0CS:modo_protegido 

    USE32  
    modo_protegido:


    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax,  RING_0DS
    mov ds,  ax
    mov fs,  ax
    mov gs,  ax
    mov ss,  ax

    xor eax, eax
    mov ax, 13
    shl ax, 3
    mov es, ax      ; segmento de video 

    ; Establecer la base de la pila
    mov ebp, Kernel_ebp 
    mov esp, Kernel_esp
    
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0, 0
   
    
    ; inicializar todo lo que involucra el juego , pantalla ,puntajes etc 


    ; Inicializar pantalla
    xor eax,eax
    .limpiarPantalla:
        cmp eax,8000
        je .fin
        mov WORD [es:eax], CARACTER_LIMPIO
        add eax,2               ;los pixeles ocupan 2 bytes
        jmp .limpiarPantalla

    .fin:
    ; primeros atributos de pantalla    
    call screen_init 

    ; Inicializar el manejador de memoria
    call mmu_init

    ; Inicializar el directorio de paginas
    call mmu_initKernelDir
    ; Cargar directorio de paginas
    mov eax, KERNEL_PAGE_DIR ; esto equivale a 0x27000 por consigna.
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, (1 << 31) ; modificamos el bit mas significativo de los 32 bits de cr0.
    mov cr0, eax


    ; Inicializar tss
    call tss_init 
    ; Inicializar las otras tareas 

    call System_tasks 
    ; inicializar juego
    call game_init 

    ; Inicializar el scheduler
    call sched_init 


    ; Inicializar la IDT
    call idt_init

    ; Cargar IDT
    lidt [IDT_DESC]
    

    ; Configurar controlador de interrupciones
    call pic_reset  ;remapeo el PIC
    call pic_enable ;prendo el PIC

    ; Cargar tarea inicial
    mov ax , GDT_TAREA_INICIAL
    ltr ax 

    ; Habilitar interrupciones
    sti



    ; Saltar a la primera tarea: Idle
    jmp (GDT_TASK_IDLE):0x0


    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
