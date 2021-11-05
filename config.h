#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

#define MIN_X 77
#define MAX_X 721
#define MIN_Y 52
#define MAX_Y 592

#define CANON_X 401
#define CANON_Y 71
#define CANON_LARGO 93
#define CANON_MAX 1.6790669176553528

#define BOLA_RADIO 6
#define BOLA_VI 450
#define BOLA_RESOL 12       // Resoluciones de la bola y de
#define RESOL_BOLA_OBS 30   // los obstaculos circulares

// Si el contador cuando la bola está quieta llega a BOLA_RESET se eliminan los obstaculos
#define BOLA_RESET 13

// La cantidad de naranjas que deben ser golpeados para que cambie el multiplicador
#define MULT0 0
#define MULT1 10
#define MULT2 15
#define MULT3 19
#define MULT4 21

#define VIDAS_INICIALES 15


#define CONTADOR_GAME_OVER 8 // Cuenta regresiva para terminar el juego

#define PUNTAJE_AZUL 10
#define PUNTAJE_NARANJA 100
#define PUNTAJE_VERDE 500
#define PUNTAJE_GRIS 0

#define JUEGO_FPS 200

#define G 600
#define PLASTICIDAD 0.8
#define ROZAMIENTO 0.9999

#endif // _CONFIG_H_
