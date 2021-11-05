#include <SDL2/SDL.h>
#include <stdlib.h> 

#include "obstaculo.h"
#include "poligono.h"
#include "config.h"
#include "tipos.h"
#include "juego.h"

struct bola{
    poligono_t *forma;
};

struct vidas{
    bola_t **figuras;
    size_t totales;
    int dibujadas;
};

struct coordenada {
    float x;
    float y;
};

struct trayectoria {
    poligono_t *linea;
};


//------------------------------------------ BOLA ------------------------------------------
 
bola_t *bola_crear(float cx, float cy, float radio, int resolucion) {
    
    bola_t *bola = malloc(sizeof(bola_t));
    if(bola == NULL) return NULL;

    bola->forma = poligono_crear_circular(radio, resolucion);
    if(bola->forma == NULL) {
        free(bola);
        return NULL;
    }
    poligono_trasladar(bola->forma, cx, cy);
    return bola;
}

void bola_destruir(bola_t *bola) {
    poligono_destruir(bola->forma);
    free(bola);
}

bool bola_dibujar(SDL_Renderer *renderer, bola_t *bola) {
    return poligono_dibujar(renderer ,bola->forma);
}


// ------------------------------------------TRAYECTORIA------------------------------------------ 
trayectoria_t *trayectoria_crear() {
    trayectoria_t *tray = malloc(sizeof(trayectoria_t));
    if(tray == NULL) return NULL;

    tray->linea = poligono_crear(NULL, 0);
    if(tray->linea == NULL) {
        free(tray);
        return NULL;
    }
    return tray;
}

void trayectoria_destruir(trayectoria_t *tray) {
    if(tray == NULL) 
        return;
    poligono_destruir(tray->linea);
    free(tray);
}

bool trayectoria_agregar_coordenada(trayectoria_t *tray, float x, float y) {
    return poligono_agregar_vertice(tray->linea, x, y);
}


bool trayectoria_dibujar(SDL_Renderer *renderer, trayectoria_t *tray) {
    return poligono_abierto_dibujar(renderer, tray->linea);
}

trayectoria_t *trayectoria_calcular(float xi, float yi, float vxi, float vyi, float a, float dt) {
    trayectoria_t *actual = trayectoria_crear();
    while(xi > MIN_X && xi < MAX_X && yi < MAX_Y) {
        trayectoria_agregar_coordenada(actual, xi, yi);
        vyi = computar_velocidad(vyi, a, dt);
        vxi *= ROZAMIENTO;
        vyi *= ROZAMIENTO;
        xi = computar_posicion(xi, vxi, dt);
        yi = computar_posicion(yi, vyi, dt);
    }
    return actual;
}



// ------------------------------------------ FUNCIONES VARIAS ------------------------------------------

double computar_velocidad(double vi, double a, double dt) {
    return vi + (a * dt);
}

double computar_posicion(double pi, double vi, double dt) {
    return pi + (vi * dt);
}

double modulo(float vx, float vy) {
    return sqrt(vx * vx + vy * vy);
}


float producto_interno(float ax, float ay, float bx, float by) {
    return ax * bx + ay * by;
}

void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy) {
    float proy = producto_interno(norm_x, norm_y, *vx, *vy);

    if(proy >= 0)
        return;

    *vx -= 2 * norm_x * proy;
    *vy -= 2 * norm_y * proy;

    // Además empujamos a la bola hacia afuera para que no se pegue
    *cx += norm_x * 0.1;
    *cy += norm_y * 0.1;
}

// ------------------------------------------ VIDAS ------------------------------------------


// Inicializa las vidas en la posicion cx cy
vidas_t *vidas_inicializar(size_t n, float cx, float cy) { 
    vidas_t *vidas = malloc(sizeof(vidas_t));
    if(vidas == NULL) return NULL;
    
    vidas->figuras = malloc(sizeof(bola_t) * n - 1);
    if(vidas->figuras == NULL) {
        free(vidas);
        return NULL;
    }

    for (int i = 0; i < n - 1; i++) {
        vidas->figuras[i] = bola_crear(cx, cy + (BOLA_RADIO * 3 * i), BOLA_RADIO, BOLA_RESOL);
        if(vidas->figuras[i] == NULL) {
            for (size_t j = 0; j < i; j++) { 
                bola_destruir(vidas->figuras[j]);
                free(vidas->figuras);
                free(vidas);
                return NULL;
            }
        }
    }
    vidas->totales = n;
    vidas->dibujadas = n - 1;

    return vidas;
}

void vidas_destruir(vidas_t *vidas) {
    for (int i = 0; i < vidas->totales - 1; i++)
        bola_destruir(vidas->figuras[i]);
    free(vidas->figuras);
    free(vidas);
}

bool vidas_estan_agotadas(vidas_t *vidas) {
    return vidas->dibujadas == 0;
}

size_t vidas_restantes(vidas_t *vidas) {
    return 1 + vidas->dibujadas;
}

void vidas_resetear(vidas_t *vidas) {
    vidas->dibujadas = vidas->totales - 1;
}

void vidas_quitar(vidas_t *vidas) {
    (vidas->dibujadas)--;
}

/*
void vidas_agregar(vidas_t *vidas) {
    if(vidas->dibujadas < vidas->totales - 1)
        (vidas->dibujadas)++;
}
*/

bool vidas_dibujar(SDL_Renderer *renderer, vidas_t *vidas) {
    if(vidas == NULL) return false;
    for (size_t i = 0; i < vidas->dibujadas; i++) {
        bola_dibujar(renderer, vidas->figuras[i]);
    }
    return true;
}


// -------------------------------------------- RECUPERADOR ----------------------------------------------------
struct recuperador {
    poligono_t *r;
    float velocidad;
    float xi;
    float ancho;
};

recuperador_t *recuperador_crear(float ancho, float alto, float velocidad) {

    recuperador_t *recuperador = malloc(sizeof(recuperador_t));
    if(recuperador == NULL) return NULL;
    
    float vertices[][2] = { {MIN_X, MAX_Y}, {MIN_X, MAX_Y - alto}, {MIN_X + ancho, MAX_Y - alto}, {MIN_X + ancho, MAX_Y} };
    recuperador->r = poligono_crear(vertices, 4);
    if(recuperador->r == NULL) {
        free(recuperador);
        return NULL;
    }
    recuperador->velocidad = 1;
    recuperador->xi = MIN_X;
    recuperador->ancho = ancho;
    return recuperador;

}

void recuperador_destruir(recuperador_t *recuperador) {
    poligono_destruir(recuperador->r);
    free(recuperador);
}

void recuperador_mover(recuperador_t *recuperador, float dt) {
    if(recuperador->xi < MIN_X || recuperador->xi + recuperador->ancho > MAX_X)
        recuperador->velocidad = -(recuperador->velocidad);
    poligono_trasladar(recuperador->r, recuperador->velocidad * dt, 0);
    recuperador->xi += (recuperador->velocidad * dt);
}

void recuperador_dibujar(SDL_Renderer *renderer ,recuperador_t *recuperador) {
    poligono_dibujar(renderer, recuperador->r);
}

bool recuperador_bola_recuperada(recuperador_t *recuperador, float cx, float cy) {
    return poligono_punto_dentro(recuperador->r, cx, cy);
}



// -------------------------------------------- PUNTAJE ----------------------------------------------------

void puntaje_actualizar(puntaje_t *puntos_totales, obstaculo_t *obstaculo, int multiplicador) {
    if(obstaculo_get_tocado(obstaculo)) return;

    puntaje_t puntaje_azul = 10;
    switch (obstaculo_get_color(obstaculo)) {
        case COLOR_AZUL:
            //Las azules valen 20 puntos
            break;
        
        case COLOR_NARANJA:
            puntaje_azul *= 10;
            break;
        
        case COLOR_VERDE:
            puntaje_azul *= 50; //como las moradas;
            break;
        
        case COLOR_GRIS:
            puntaje_azul *= 0;
            break;
    }
    *puntos_totales = *puntos_totales + puntaje_azul * multiplicador;
}

void puntaje_actualizar_multiplicador(int *multiplicador, size_t naranjas_golpeados) {
    if(naranjas_golpeados < MULT1)
    (*multiplicador) = 1;

    if(naranjas_golpeados >= MULT1)
    (*multiplicador) = 2;
    
    if(naranjas_golpeados >= MULT2)
    (*multiplicador) = 3;
    
    if(naranjas_golpeados >= MULT3)
    (*multiplicador) = 5;
    
    if(naranjas_golpeados >= MULT4)
    (*multiplicador) = 10;
}