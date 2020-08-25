/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#define CODE_0 0x11
#define CODE_1 0x1f
#define CODE_2 0x2c 
#define CODE_3 0x2d
#define CODE_4 0x2e 
#define CODE_5 0x17 
#define CODE_6 0x25 
#define CODE_7 0x30 
#define CODE_8 0x31 
#define CODE_9 0x32

// ==========================================
// ######## ~ VARIABLES GLOBALES ~ ##########
// ==========================================
uint8_t creo_portal;
uint32_t veces_que_rick137_uso_arma = 0;
uint32_t veces_que_rick248_uso_arma = 0;
uint32_t puntaje_rick137 = 0;
uint32_t puntaje_rick248 = 0;
uint8_t indice_array_tareas = 0;
uint8_t numero_de_cronenbergs = 20;
tarea tarea_corriendo;


// =====================================================
//########## FUNCIONES DE INICICIO DE JUEGO Y POSICIONES 
// =====================================================
const uint8_t valuesX[24] = {2,  63, 18, 36, 30, 79, 21, 10, 73, 32, 70, 28, 73, 64, 28, 12,  3, 41, 27, 24, 54, 20, 33, 74};
const uint8_t valuesY[24] = {31, 32, 39,  5, 37, 29, 12, 24,  0,  4,  2, 16, 15, 24, 24, 18, 23, 38, 12, 25, 14, 22,  0, 28};

void getRandomPosition(uint8_t index, uint8_t *x, uint8_t *y) {
    *x = valuesX[index%24];//columna
    *y = valuesY[index%24];//fila    
}
 


void game_init() {
 		// inicio de la pantalla 
	creo_portal = 0;

	screen_drawBox(0, 0, 1, 80, 0x32, 0x00);   // parte superior oscura 
	screen_drawBox(40, 0, 10, 80, 0x32, 0x00); // parte inferior oscura
	screen_drawBox(44, 5, 3, 9, 0x00, 0xCC); // cuadrado rojo 
	screen_drawBox(44, 67, 3, 9, 0x00, 0x99);  // cuadrado azul 

	// puntajes

	print_dec(puntaje_rick137, 9, 5,45, 0xf);
    print_dec(puntaje_rick248, 9, 67,45, 0xf);

    // imprimer estados
   
    imprimir_estados();
}




void I_AM_RICK(uint32_t codigo_ID){

    if(tarea_corriendo.tipo >= Cronenberg && tarea_corriendo.estado == 1){
        if( codigo_ID == 0xC137){
            puntaje_rick137++;  
            print_dec(puntaje_rick137, 9, 5,45, 0xf);

            // actualizo los estados de la tarea 
            tarea_corriendo.estado = 2; // convertida
            tarea_corriendo.conquistador = RickC137; // quien la poseyo 
            print("+",tarea_corriendo.columna,tarea_corriendo.fila, 0x1F52); //cambio color de tarea
            // actualizar informacion de la tarea 
            array_tareas[indice_array_tareas] = tarea_corriendo;
            numero_de_cronenbergs--;
        }
        else {
            puntaje_rick248 ++;
            print_dec(puntaje_rick248, 9, 67,45, 0xf);

            // actualizar los estados de la tarea 
            tarea_corriendo.estado =2;  // convertida 
            tarea_corriendo.conquistador = RickD248; // quien la poseyo 
            print("+",tarea_corriendo.columna,tarea_corriendo.fila, 0xD248); //cambio color de tarea
            // actualizar informacion de la tarea 
            array_tareas[indice_array_tareas] = tarea_corriendo; 
            numero_de_cronenbergs--;
        }

    }
    // matar a la tarea si su mente fue poseida por el otro jugador
    else{
        matar_mente_poseida(tarea_corriendo,codigo_ID);
    }
}




// ======================================================
// $$$$$$$$$$ FUNCIONES QUE CHEQUEAN EL ESTADO DEL JUEGO
// ======================================================



void game_update( uint32_t tarea_a_correr){
	creo_portal = 0;
	// la posicion en el array de tareas se corresponde con el selector shifteado 3 - 15
    indice_array_tareas = (tarea_a_correr>>3 ) - 15;
	tarea_corriendo = array_tareas [indice_array_tareas];

	imprimir_estados();
}


uint16_t check_endGame(){
 if (array_tareas[rick137].estado == 0 || array_tareas[rick248].estado == 0 
 	|| array_tareas[morty137].estado == 0 || array_tareas[morty248].estado == 0
  	|| puntaje_rick137 >= 20 || puntaje_rick248 >= 20 || (puntaje_rick248 + puntaje_rick137 >= 20) || numero_de_cronenbergs ==0
  ){
 	return 1;
 }
 else {
 	return 0;
 }

}


void matar_mente_poseida(tarea jugador , uint32_t codigo_ID){
	if ((tarea_corriendo.tipo == RickC137 || tarea_corriendo.tipo ==MortyC137 )&& codigo_ID == 0xD248){
		tarea_corriendo.estado = 0;
		array_tareas[indice_array_tareas] = tarea_corriendo;
	}

	else if ((tarea_corriendo.tipo == RickD248|| tarea_corriendo.tipo == MortyD248 )&& codigo_ID == 0xC137){
		tarea_corriendo.estado = 0;
		array_tareas[indice_array_tareas] = tarea_corriendo;
	}

}


void aumentar_portales_de_rick(uint8_t rick ){
	
	if ( rick == RickC137){
		veces_que_rick137_uso_arma++;
	}
	else{
		veces_que_rick248_uso_arma++;
	}
} 


uint8_t  numero_dePortales_De_rick(tarea morty){

	if ( morty.tipo == MortyD248){
		uint32_t temp = veces_que_rick248_uso_arma;
		// se retea el conteo 
		if ( veces_que_rick248_uso_arma >= 10){
			veces_que_rick248_uso_arma = 0;
		}

	return temp;
	}

	else {

		uint32_t temp = veces_que_rick137_uso_arma;
		if( veces_que_rick137_uso_arma >= 10){
			veces_que_rick137_uso_arma =0;
		}

	return temp;
	}

}



// ==================================================
// ############# FUNCIONES QUE ATIENDAN A LAS SYSCALL 
// ==================================================

void desplazamiento( int32_t* fila , int32_t* columna){

	if (tarea_corriendo.tipo == RickC137){
		valor_dezplazamiento(array_tareas[morty137], fila,columna);
	}

	else if ( tarea_corriendo.tipo == RickD248){
		valor_dezplazamiento(array_tareas[morty248], fila,columna);
	}
}


void valor_dezplazamiento (tarea morty, int32_t* fila,int32_t* columna ){

	int32_t valor_columna = (int32_t)( morty.columna  - tarea_corriendo.columna );
	int32_t valor_fila =  (int32_t)( morty.fila - tarea_corriendo.fila);

	*columna = valor_fila; // columna
	*fila = valor_columna; // fila 
}



uint32_t armaDisponible ( int32_t pos_columna , int32_t pos_fila , uint32_t cross ,uint32_t with_morty){

if ( esRick (tarea_corriendo.tipo)){
	if (creo_portal){
		return 0; 			// ya creo portal 
	}
	else {
		aumentar_portales_de_rick(tarea_corriendo.tipo); // aumento la cantidad de veces que rick casteo portal
		if (cross){
			cruzar_portal( pos_columna , pos_fila); // la tarea cruza el portal 

			if ( with_morty){
  				tarea morty = obtener_morty(tarea_corriendo.tipo);
				cruzar_morty(morty,pos_columna,pos_fila);
			}
		}
		else {
			// crea el portal pero no cruza 
			crear_portal(pos_columna,pos_fila);
		}
	return 1; // el portal se creo con exito 
	}

}

else if (esMorty(tarea_corriendo)){
	if(numero_dePortales_De_rick (tarea_corriendo) >= 10){
		if (creo_portal){
			return 0; // no puede crear mas de un portal por tick
		}
		else {

			if (cross){
				cruzar_portal( pos_columna , pos_fila); // la tarea cruza el portal 
			}
			else {
				// crea el portal pero no cruza 
				crear_portal(pos_columna,pos_fila);
			}
		return 1; // el portal se creo con exito 
		}
	}
	else{
		return 0; // no crea portal 
	}
}

else return 0; // era una cronenberg 

}


void cruzar_morty(tarea morty, int32_t columna , int32_t pos_fila){

	// guarda momentaneamente la informacion de la tarea actual 
	uint8_t guardar_indice = indice_array_tareas;
	indice_array_tareas = (morty.tipo == MortyC137)?morty137:morty248;
	tarea temp = tarea_corriendo;
	tarea_corriendo = morty;

	// la morty siempre cruza cuando la llama rick 
	cruzar_portal(columna,pos_fila);

	// restauro la info vieja 
	array_tareas[indice_array_tareas] = tarea_corriendo;
	tarea_corriendo = temp;
	indice_array_tareas = guardar_indice;
}


void crear_portal( int32_t pos_columna , int32_t pos_fila){
// inhabilitar creacion de portales 
	creo_portal = 1;


// posiciones destino 
	uint32_t des_columna = modulo ( tarea_corriendo.columna + pos_columna, 80);
	uint32_t des_fila = modulo ( tarea_corriendo.fila + pos_fila, 40);

	uint32_t destino = 0x400000 + (des_fila*80+ des_columna)*0x2000; 

	uint32_t cr3Actual = rcr3();
	lcr3(tarea_corriendo.cr3); //Cambio cr3

	mmu_mapPage(tarea_corriendo.cr3, 0x8002000, destino, 1,1);
	mmu_mapPage(tarea_corriendo.cr3, 0x8003000, destino + PAGE_SIZE, 1,1); //Mapeo debajo

	lcr3(cr3Actual);

// dibujo el portal
	printear_portal(tarea_corriendo,des_columna,des_fila);
//actualizo variables
	tarea_corriendo.dondePortalcol = des_columna;
	tarea_corriendo.dondePortalfil = des_fila;
	array_tareas[indice_array_tareas] = tarea_corriendo;
}



void cruzar_portal (int32_t pos_columna , int32_t pos_fila){
// inhabilitar creacion de portales 
	creo_portal = 1;

// posiciones destino 
	uint32_t des_columna = modulo ((int32_t) tarea_corriendo.columna + pos_columna, 80);
	uint32_t des_fila = modulo ((int32_t) tarea_corriendo.fila + pos_fila, 40);

	uint32_t destino = 0x400000 + (des_fila*80+des_columna)*0x2000; 

	// movemos el codigo de la tarea a la nueva posicion 
	// hacemos un mapeo a la direccion destino con una virtual temporal
	// y luego copiamos el codigo en esta nueva posicion
	// desmapeamos la memoria virtual temporal y mapeamos la direccion que nos interesa

	uint32_t Codigo_temp =  CODIGO_TEMPORAL;
	uint32_t Stack_temp = STACK_TEMPORAL;

	uint32_t cr3Actual = rcr3();

	lcr3(tarea_corriendo.cr3);


	mmu_mapPage(tarea_corriendo.cr3, Codigo_temp, destino, 1,1); //Mapeo temporal
	mmu_mapPage(tarea_corriendo.cr3, Stack_temp, destino+0x1000, 1,1);

	// direccion virtuales temporales 
	uint32_t *virtual_Code = (uint32_t *)Codigo_temp;
	uint32_t *virtual_Stack = (uint32_t *)Stack_temp;

	// codigo actualmente mapeado
	uint32_t *codigo_en_ejecucion = (uint32_t *)TASK_CODE;
	uint32_t *stack_en_ejecucion = (uint32_t *)(TASK_CODE + PAGE_SIZE);

	for (int i = 0; i < 1024; i++){
		virtual_Code[i] = codigo_en_ejecucion[i];
		virtual_Stack[i] = stack_en_ejecucion[i];
	}

	// desmapeo las viejas direccion para reemplazarlas con la nueva 
	mmu_unmapPage(tarea_corriendo.cr3, TASK_CODE); 
	mmu_unmapPage(tarea_corriendo.cr3, TASK_CODE + PAGE_SIZE); 

	// desmapeo las virtuales temporales, codigo y stack
	mmu_unmapPage(tarea_corriendo.cr3, Codigo_temp); 
	mmu_unmapPage(tarea_corriendo.cr3, Stack_temp); 

	// mapeo la tarea  a la nueva direccion fisica 
	mmu_mapPage(tarea_corriendo.cr3, TASK_CODE, destino, 1, 1); 
	mmu_mapPage(tarea_corriendo.cr3, TASK_CODE + PAGE_SIZE, destino+PAGE_SIZE, 1, 1);


	lcr3(cr3Actual);


	// dibujo el portal
	printear_personaje(tarea_corriendo,des_columna,des_fila);
	//actualizo variables
	tarea_corriendo.columna = des_columna;
	tarea_corriendo.fila = des_fila;
	array_tareas[indice_array_tareas] = tarea_corriendo;
}




// ==================================================
// ############# FUNCIONES DE RECONOMIENTO  
// ==================================================


uint8_t esRick( uint8_t rick){
	
	if ( rick == RickC137 || rick == RickD248){
		return 1;
	}
	else {
		return 0;
	}
} 


tarea obtener_morty(uint8_t rick){

	if ( rick == RickC137){
		return array_tareas[morty137];
	}
	else {
		return array_tareas[morty248];
	}
}



uint8_t hayCronenberg(uint32_t columna,uint32_t fila){
	for (uint16_t i = 5; i<26 ; i++){
		if(array_tareas[i].columna == columna && array_tareas[i].fila == fila){
			return 1;
		}
	}
	return 0;
}


uint8_t esMorty ( tarea morty){

	if ( morty.tipo == MortyC137){
		return 1;
	}
	else if ( morty.tipo == MortyD248){
		return 1;
	}
	// es otra tarea 
	else return 0;
}


uint32_t modulo(int32_t valor, int32_t modulo){
	if (valor<0){
		while ( valor < 0){
			valor +=modulo;
		}
	}
	while (valor >= modulo){
		valor = valor - modulo;
	}

	return (uint32_t)valor;
}





























































