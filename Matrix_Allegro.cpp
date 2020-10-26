
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;


//Globales
//**********************************************************

#define FPS 30.0

//Elementos de allegro que se utilizarán para el juego
//**********************************************************
ALLEGRO_DISPLAY *pantalla;
ALLEGRO_FONT *fuente;

//LAS SIGUIENTES DOS FUNCIONES SE USAN PARA GENERAR NUMEROS RANDOM "VERDADEROS"
//EN PERIODOS DE TIEMPO MUY CORTOS
long g_seed = 1;
inline int fastrand() {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}
char GenerarRandom()
{
	char c = 'A' + fastrand() % 24;
	return c;
}




void inicializarstack(char stack[6]) {
	for (int i = 0; i > 6; i++) {
		stack[i] = ' ';
	}
}


/*
CADA LINEA DE CARACTERES QUE VA CAYENDO SE GUARDA EN UN STACK, QUE ES UN ARREGLO DE CHAR
COMO SE VAN GENERANDO CARACTERES NUEVOS ENTONCES LOS CARACTERES EN EL STACK SE VAN DESPLAZANDO
A MODO DE UNA PILA FIFO, PERO COMO ES UN ARREGLO ENTONCES SE IMPLEMENTA MAS FACIL DE LA SIGUIENTE MANERA
*/
void desplazar(char stack[6]) {
	stack[5] = stack[4];
	stack[4] = stack[3];
	stack[3] = stack[2];
	stack[2] = stack[1];
	stack[1] = stack[0];
}

/*
DIBUJAR LO QUE HACE ES EN BASE A UN STACK DEFINIDO Y UN CONJUNTO DE COORDENADAS IMPRIME EN EL DISPLAY
LA SECUENCIA DE CARACTERES ASCII, Y A SU VEZ LLAMA A LA FUNCION DESPLAZAR PARA SACAR DEL STACK EL CARACTER MAS VIEJO Y
AÑADIR AL INICIO EL MAS NUEVO
*/
char actual[1];
void dibujar(char caracter[1], char stack[6], int x, int y) {
	actual[0] = stack[0];
	al_draw_text(fuente, al_map_rgb(0, 200, 0), x, y - 20, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[1];
	al_draw_text(fuente, al_map_rgb(0, 150, 0), x, y - 40, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[2];
	al_draw_text(fuente, al_map_rgb(0, 100, 0), x, y - 60, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[3];
	al_draw_text(fuente, al_map_rgb(0, 50, 0), x, y - 80, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[4];
	al_draw_text(fuente, al_map_rgb(0, 25, 0), x, y - 100, ALLEGRO_ALIGN_CENTRE, actual);
	actual[0] = stack[5];
	al_draw_text(fuente, al_map_rgb(0, 10, 0), x, y - 120, ALLEGRO_ALIGN_CENTRE, actual);

	al_draw_text(fuente, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, caracter);
	
	al_flip_display();
	
	desplazar(stack);
	stack[0] = caracter[0];
}




int main() {
	if (!al_init()) {
		fprintf(stderr, "No se puede iniciar allegro!\n");
		return -1;
	}

	//Esta línea de código permite que la ventana tenga la capacidad de cambiar de tamaño
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	pantalla = al_create_display(720, 480);
	al_set_window_position(pantalla, 200, 200);
	al_set_window_title(pantalla, "MATRIX");
	if (!pantalla) {
		fprintf(stderr, "No se puede crear la pantalla!\n");
		return -1;
	}
	//Líneas para obtener las funcionalidades del uso de las fuentes
	//*******************
	al_init_font_addon();
	al_init_ttf_addon();
	//*******************
	//Línea para obtener las funcionalidades de las imágenes
	//*******************
	al_init_image_addon();
	//*******************


	//Líneas para obtener las funcionalidades de los audios
	//*******************
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1000);
	//*******************

	fuente = al_load_font("georgia.ttf", 16, NULL);

	//Timers que se necesitarán para el juego
	//**********************************************************
	ALLEGRO_TIMER *primerTimer = al_create_timer(5 / FPS);
	ALLEGRO_TIMER *segundoTimer = al_create_timer(5.0 / FPS);
	ALLEGRO_TIMER *tercerTimer = al_create_timer(1.0 / FPS);
	//**********************************************************

	//Se crea una cola de eventos
	ALLEGRO_EVENT_QUEUE *colaEventos = al_create_event_queue();

	//Registro de los eventos
	//**********************************************************
	al_register_event_source(colaEventos, al_get_timer_event_source(primerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(segundoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(tercerTimer));
	//al_register_event_source(colaEventos, al_get_keyboard_event_source());
	//**********************************************************

	//Inicialización de los timer
	//**********************************************************
	al_start_timer(primerTimer);
	al_start_timer(segundoTimer);
	al_start_timer(tercerTimer);
	//**********************************************************
	bool hecho = true;
	ALLEGRO_KEYBOARD_STATE estadoTeclado;
	//al_get_keyboard_state(&estadoTeclado);
	int x = 300;
	int y = 10;
	char caracter[1];
	char caracter2[1];
	char stack[6];
	char stack2[6]; //borrar

	
	while (hecho) {

		ALLEGRO_EVENT eventos;

		al_wait_for_event(colaEventos, &eventos);
		//al_clear_to_color(transparente);

		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			if (eventos.timer.source == primerTimer) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				caracter[0] = GenerarRandom();
				//AQUI VA LO QUE PASARIA EN EL EVENTO DADO POR EL PRIMER TIMER
				dibujar(caracter,stack, x,y);
				y = y + 20;
			}
			

			if (y >= 700) {
				y = 10;
			}
		}




		al_flip_display();
		
	}



}
