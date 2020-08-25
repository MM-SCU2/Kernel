/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


//uint32_t modo_debug = 0;
extern uint32_t  puntaje_rick137;
extern uint32_t  puntaje_rick248;
extern uint8_t numero_de_cronenbergs;
uint8_t relojs [6] = {1 , 2 , 3, 4 , 5, 6 };
uint8_t controlClock =0;

// ==================================================
// ############# FUNCIONES DE LA CATEDRA 
// ==================================================

                             //columnas     filas
void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    uint32_t i;
    uint8_t letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        uint32_t resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    uint8_t hexa[8];
    uint8_t letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_drawBox(uint32_t fInit,
                    uint32_t cInit,
                    uint32_t fSize,
                    uint32_t cSize,
                    uint8_t character,
                    uint8_t attr ) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    uint32_t f;
    uint32_t c;
    for (f = fInit; f < fInit+fSize; f++) {
        for (c = cInit; c < cInit+cSize; c++) {
            p[f][c].c = character;
            p[f][c].a = attr;
        }
    }   
}

void atender_teclado(uint8_t code){
 if(code > 0x0b || code < 0x02){
        return;
    }
 if(code == 0x0b) {
        print_dec((uint32_t)0, 1, 10, 10, ((0x0 << 4) | 0x7) );
    } else {
        print_dec((uint32_t)code - 1, 1, 10, 10, ((0x0 << 4) | 0x7) );
    }
}


void print_string(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int32_t i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (uint8_t) text[i];
        p[y][x].a = (uint8_t) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}




// ======================================================================
// #####$$$$$$$$$$ FUNCIONES QUE ACTUALIZAN LA PANTALLA SEGUN LOS SUCESOS
// ======================================================================



void screen_init(){

    screen_drawBox(0, 0, 1, 80, 0x32, 0x00);   // parte superior oscura 
    screen_drawBox(40, 0, 10, 80, 0x32, 0x00); // parte inferior oscura
    screen_drawBox(44, 5, 3, 9, 0x00, 0xCC); // cuadrado rojo 
    screen_drawBox(44, 67, 3, 9, 0x00, 0x99);  // cuadrado azul
}





void colorearJugadores ( uint32_t columna,uint32_t fila,uint8_t Tipo_Tarea,uint8_t estado,uint8_t conquistador){


    if (Tipo_Tarea == RickC137){
        print("R", (columna) , (fila),0x1F52);
    }
    else if (Tipo_Tarea == MortyC137){
        print("M",columna,fila,0x1F52);
    }
    else if (Tipo_Tarea == RickD248){
        print("R",(columna) ,(fila),0x401A);
    }
    else if ( Tipo_Tarea == MortyD248){
        print("M", (columna) ,(fila),0x401A);   
    }
    //cronenbergs
    else {
        // tarea viva 
        if(estado == 1){
            print("+", (columna), (fila) ,C_BG_BROWN); 
        }
        // tarea conquistada
        else if(estado == 2){
            
                if(conquistador == RickC137){
                    print("+", (columna), (fila) ,0x1F52);        
                }
                else{
                    print("+", (columna), (fila) ,0x401A);   
                }   
        }
        // tarea muerta 
        else{
            print("+", (columna), (fila) ,C_BG_BLACK);
        }
    }
}


void printear_portal( tarea corriendo , uint32_t pos_columna , uint32_t pos_fila){

    uint16_t atributo;

    atributo = (corriendo.tipo == RickC137 || corriendo.tipo == MortyC137 )? 0x1F52:0x401A;

    if( hayCronenberg(corriendo.dondePortalcol,corriendo.dondePortalfil)){
        print( "+", corriendo.dondePortalcol , corriendo.dondePortalfil ,atributo);
    }
    else{
    // borro el portal viejo 
         print( " ", corriendo.dondePortalcol , corriendo.dondePortalfil , C_BG_GREEN);
    }

    // dibujo portal nuevo
    print( "*", pos_columna , pos_fila , 0xd);
}



void printear_personaje ( tarea corriendo , uint32_t pos_columna , uint32_t pos_fila){
    char* avatar ;
    uint16_t atributo;

    atributo = (corriendo.tipo == RickC137 || corriendo.tipo == MortyC137 )? 0x1F52:0x401A;
    avatar = (corriendo.tipo == RickC137 || corriendo.tipo == RickD248)? "R":"M";

    if( hayCronenberg(corriendo.columna,corriendo.fila)){
        print( "+", corriendo.columna , corriendo.fila ,atributo);
    }
    else{
    // borro el portal viejo 
         print( " ", corriendo.columna , corriendo.fila , C_BG_GREEN);
    }
    // borro la ubicacion vieja 

    // dibujo avatar nuevo
    print( avatar, pos_columna , pos_fila , atributo);

}




void imprimir_estados(){

    // primeras cronenbeg 
    uint32_t numero_tarea = 1;
    uint32_t columna = 22;
    uint32_t fila = 43;
    for(uint8_t i= 5; i <15; i++){
        if(array_tareas[i].estado == 0){
            print_dec(numero_tarea,2,columna, fila, 0xf);
            print( "/",columna,fila+1,0x7);
            screen_drawBox(fila+3,columna,1,1,0x0 , C_BG_BLACK);   
        }
        else{
            print_dec(numero_tarea,2,columna, fila, 0xf);
            char * timer = character(controlClock);
            print( timer,columna,fila+1,0x7);
            imprimir_status(array_tareas[i].estado,array_tareas[i].conquistador,columna+2,fila +3 );
        }
        columna = columna + 4;
        numero_tarea++;
 }
    // segundas cronenberg 

    fila = 46;
    columna = 22;

    for(uint8_t i= 15; i <25; i++){
        if(array_tareas[i].estado == 0){
            print_dec(numero_tarea,2,columna, fila, 0xf);
            print("/",columna,fila+2,0x7);      
            screen_drawBox(fila+3,columna,1,1,0x0 , C_BG_BLACK);         
        }
        else{
            print_dec(numero_tarea,2,columna, fila, 0xf);
            char * timer = character(controlClock);
            print( timer,columna,fila+2,0x7);
            imprimir_status(array_tareas[i].estado,array_tareas[i].conquistador,columna+2,fila +3 );
        }
        columna = columna + 4;
        numero_tarea++;
    }
    controlClock++;
    if(controlClock >6){
        controlClock =0;
    }
}

char* character( uint8_t select ){

    if( relojs[select] == 1){
        return "/";
    }
    else if ( relojs[select] == 2 ){
        return "-";
    }

    else if ( relojs[select] == 3 ){
        return "\\";
    }
    else if ( relojs[select] == 4 ){
        return "-";
    }
    else if ( relojs[select] == 5 ){
        return "/";
    }
    else return "-";
}


void imprimir_status(uint8_t estado , uint8_t conqueror ,uint32_t columna,uint32_t fila){


    if( estado == 1){

        screen_drawBox(fila-2,columna,1,1,0x0 , C_BG_BROWN);
    }

    else {
        if( conqueror == RickC137){
            screen_drawBox(fila-2,columna,1,1,0x0 , C_BG_RED);
        }
        else{
            screen_drawBox(fila-2,columna,1,1,0x0 , C_BG_BLUE );   
        }
    }

}



void resetear_pantalla(){

    uint16_t * video = (uint16_t*) 0xB8000;
    for (uint16_t i =0 ; i < 8000 ; i++){
        video[i] = 32 << 8 | (0x0 << 4) | 0x0;
    }

    for (uint8_t i = 0 ; i < 25 ; i++){
        colorearJugadores( (uint32_t) array_tareas[i].columna , (uint32_t) array_tareas[i].fila,array_tareas[i].tipo,array_tareas[i].estado,array_tareas[i].conquistador);
    }

    imprimir_estados();

}





void pantalla_final (){
    screen_drawBox(9,19,17,27,0x0,C_BG_BLACK);
    screen_drawBox(10,20,15,25,0x0,C_BG_LIGHT_GREY);
    print_string("GAME OVER" , 27,14 ,C_FG_RED | C_BG_BLACK);

    if( numero_de_cronenbergs == 0){
        print_string("los cronenberg murieron!" , 21,16 ,C_FG_RED | C_BG_BLACK);        
    }

    else if ( puntaje_rick137 == 20){
        print_string("el equipo 137 ha ganado" , 21,16 ,C_FG_RED | C_BG_BLACK); 
    }

    else if ( puntaje_rick248 == 20){
        print_string("el equipo 248 ha ganado" , 21,16 ,C_FG_RED | C_BG_BLACK); 
    }
    else if (array_tareas[rick137].estado == 0 ){
        print_string("Rick 137 ha muerto" , 21,16 ,C_FG_RED | C_BG_BLACK);  
        print_string("WUBBA DUBBA LUB LUB" , 21,18 ,C_FG_RED | C_BG_BLACK); 
  
    }
    else if (array_tareas[morty137].estado == 0 ){
        print_string("Morty 137 ha muerto" , 21,16 ,C_FG_RED | C_BG_BLACK); 
        print_string("MALDITA SEA RICK" , 21,18 ,C_FG_RED | C_BG_BLACK);    
  
    }
    else if (array_tareas[rick248].estado == 0 ){
        print_string("Rick 137 ha muerto" , 22,16 ,C_FG_RED | C_BG_BLACK);  
        print_string("WUBBA DUBBA LUB LUB" , 22,18 ,C_FG_RED | C_BG_BLACK); 
  
    }
    else if (array_tareas[morty248].estado == 0 ){
        print_string("Morty 248 ha muerto" , 21,16 ,C_FG_RED | C_BG_BLACK); 
       print_string("MALDITA SEA RICK" , 21,18 ,C_FG_RED | C_BG_BLACK); 
 
    }
}





void imprimir_debugging(uint32_t valor_excepcion, uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi, uint32_t ebp, uint32_t ds, uint32_t es, uint32_t fs, uint32_t gs, uint32_t cr0, uint32_t cr1, uint32_t cr2, uint32_t cr3, uint32_t cr4, int32_t errorCode, uint32_t eip,uint32_t cs,uint32_t eflags,uint32_t esp,uint32_t ss){
    
        
    char * nombre_excepcion = "?? - Desconocida";
    int cant_excepciones = 21;
    char * excepciones[] = {"0x0 - Divide-by-zero Error", "1 - Debug", "2 - Non-maskable Interrupt", "3 - Breakpoint", "4 - Overflow", "5 - Bound Range Exceeded", "6 - Invalid Opcode", "7 - Device Not Available", "8 - Double Fault", "9 - Coprocessor Segment Overrun", "10 - Invalid TSS", "11 - Segment Not Present", "12 - Stack-Segment Fault", "13 - General Protection Fault", "14 - Page Fault", "15 - (Reserved)", "16 - x87 Floating-Point Exception", "0x11 - Alignment Check", "0x12 - Machine Check", "0x13 - SIMD Floating-Point Exception", "0x14 - Virtualization Exception"};

    if (valor_excepcion < cant_excepciones) nombre_excepcion = excepciones[valor_excepcion];

    screen_drawBox(0, 5, 36, 36, 0, C_BG_BLACK);
    screen_drawBox(1, 6, 34, 34, 0, C_BG_LIGHT_GREY);
    print(nombre_excepcion, 6, 0, C_FG_RED | C_BG_BLACK);

    print("eax:", 7, 2, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(eax, 8, 12, 2, C_FG_RED | C_BG_LIGHT_GREY);

    print("ebx:", 7, 4, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ebx, 8, 12, 4, C_FG_RED | C_BG_LIGHT_GREY);

    print("ecx:", 7, 6, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ecx, 8, 12, 6, C_FG_RED | C_BG_LIGHT_GREY);

    print("edx:", 7, 8, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(edx, 8, 12, 8, C_FG_RED | C_BG_LIGHT_GREY);

    print("esi:", 7, 10, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(esi, 8, 12, 10, C_FG_RED | C_BG_LIGHT_GREY);

    print("edi:", 7, 12, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(edi, 8, 12, 12, C_FG_RED | C_BG_LIGHT_GREY);

    print("ebp:", 7, 14, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ebp, 8, 12, 14, C_FG_RED | C_BG_LIGHT_GREY);

    print("esp:", 7, 16, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(esp, 8, 12, 16, C_FG_RED | C_BG_LIGHT_GREY);

    print("eip:", 7, 18, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(eip, 8, 12, 18, C_FG_RED | C_BG_LIGHT_GREY);

    print("cs:", 7, 20, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cs, 4, 12, 20, C_FG_RED | C_BG_LIGHT_GREY);

    print("ds:", 7, 22, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ds, 4, 12, 22, C_FG_RED | C_BG_LIGHT_GREY);

    print("es:", 7, 24, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(es, 4, 12, 24, C_FG_RED | C_BG_LIGHT_GREY);

    print("fs:", 7, 26, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(fs, 4, 12, 26, C_FG_RED | C_BG_LIGHT_GREY);

    print("gs:", 7, 28, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(gs, 4, 12, 28, C_FG_RED | C_BG_LIGHT_GREY);

    print("ss:", 7, 30, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(ss, 4, 12, 30, C_FG_RED | C_BG_LIGHT_GREY);

    print("eflags:", 7, 32, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(eflags, 8, 15, 32, C_FG_RED | C_BG_LIGHT_GREY);

    // derecha
    print("cr0:",22, 7, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr0, 8, 27, 7, C_FG_RED | C_BG_LIGHT_GREY);

    print("cr2:", 22, 9, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr2, 8, 27, 9, C_FG_RED | C_BG_LIGHT_GREY);

    print("cr3:", 22, 11, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr3, 8, 27, 11, C_FG_RED | C_BG_LIGHT_GREY);

    print("cr4:", 22, 13, C_FG_RED | C_BG_LIGHT_GREY);
    print_hex(cr4, 8, 27, 13, C_FG_RED | C_BG_LIGHT_GREY);

    print("stack", 29, 28, C_FG_RED | C_BG_LIGHT_GREY);

    uint32_t* espPrint = (uint32_t*) esp;

    if ( espPrint == (uint32_t*) 0x08002000){
        // printeamos stack
        print_hex(*(espPrint - 0x04) , 8, 28, 30, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x08), 8, 28, 31, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x0c), 8, 28, 32, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x10), 8, 28, 33, C_FG_RED | C_BG_LIGHT_GREY);

    }
    else if ( espPrint == (uint32_t*)(0x08002000 - 0x04 )){
        print_hex(*(espPrint),8, 28, 30, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x04), 8, 28, 31, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x08), 8, 28, 32, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x0C), 8, 28, 33, C_FG_RED | C_BG_LIGHT_GREY);
    }
    
    else if ( espPrint == (uint32_t*)(0x08002000 - 0x08 )){
        print_hex(*(espPrint+0x04), 8, 28, 31, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint),8, 28, 30, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x04), 8, 28, 32, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x08), 8, 28, 33, C_FG_RED | C_BG_LIGHT_GREY);
        
    }

    else if ( espPrint == (uint32_t*)(0x08002000 - 0x0C )){
        print_hex(*(espPrint+0x08), 8, 28, 32, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint+0x04), 8, 28, 31, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint),8, 28, 30, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint-0x04), 8, 28, 33, C_FG_RED | C_BG_LIGHT_GREY);
        
    }

    else{

        print_hex(*(espPrint+0x0C), 8, 28, 33, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint+0x08), 8, 28, 32, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint+0x04), 8, 28, 31, C_FG_RED | C_BG_LIGHT_GREY);
        print_hex(*(espPrint),8, 28, 30, C_FG_RED | C_BG_LIGHT_GREY);
        
    }
    
}


