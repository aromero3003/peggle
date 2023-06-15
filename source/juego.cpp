#include "juego.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "bola.h"
#include "config.h"
#include "obstaculo.h"
#include "poligono.h"
#include "tipos.h"
#include "vec2.h"

void reflejar(aVec2 norm, aVec2 &c, aVec2 &v) {
    float proy = aDot(norm, v);

    if (proy >= 0) return;

    v -= 2 * norm * proy;
    c += norm * 0.1;
}

// ------------------------------------------ VIDAS
// ------------------------------------------

/*
// Inicializa las vidas en la posicion cx cy
vidas_t *vidas_inicializar(size_t n, float cx, float cy) {
    vidas_t *vidas = (vidas_t *)malloc(sizeof(vidas_t));
    if (vidas == NULL) return NULL;

    vidas->figuras = (bola_t **)malloc(sizeof(bola_t) * n - 1);
    if (vidas->figuras == NULL) {
        free(vidas);
        return NULL;
    }

    for (int i = 0; i < n - 1; i++) {
        vidas->figuras[i] = bola_t()
            bola_crear(cx, cy + (BOLA_RADIO * 3 * i), BOLA_RADIO, BOLA_RESOL);
        if (vidas->figuras[i] == NULL) {
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

bool vidas_estan_agotadas(vidas_t *vidas) { return vidas->dibujadas == 0; }

size_t vidas_restantes(vidas_t *vidas) { return 1 + vidas->dibujadas; }

void vidas_resetear(vidas_t *vidas) { vidas->dibujadas = vidas->totales - 1; }

void vidas_quitar(vidas_t *vidas) { (vidas->dibujadas)--; }

void vidas_agregar(vidas_t *vidas) {
    if(vidas->dibujadas < vidas->totales - 1)
        (vidas->dibujadas)++;
}

bool vidas_dibujar(SDL_Renderer *renderer, vidas_t *vidas) {
    if (vidas == NULL) return false;
    for (size_t i = 0; i < vidas->dibujadas; i++) {
        bola_dibujar(renderer, vidas->figuras[i]);
    }
    return true;
}
*/

// -------------------------------------------- PUNTAJE
// ----------------------------------------------------

void puntaje_actualizar(puntaje_t *puntos_totales, obstaculo_t &obstaculo,
                        int multiplicador) {
    if (obstaculo.get_tocado()) return;

    puntaje_t puntaje_azul = 10;
    switch (obstaculo.get_color()) {
        case COLOR_AZUL:
            // Las azules valen 20 puntos
            break;

        case COLOR_NARANJA:
            puntaje_azul *= 10;
            break;

        case COLOR_VERDE:
            puntaje_azul *= 50;  // como las moradas;
            break;

        case COLOR_GRIS:
            puntaje_azul *= 0;
            break;
    }
    *puntos_totales = *puntos_totales + puntaje_azul * multiplicador;
}

void puntaje_actualizar_multiplicador(int *multiplicador,
                                      size_t naranjas_golpeados) {
    if (naranjas_golpeados < MULT1) (*multiplicador) = 1;

    if (naranjas_golpeados >= MULT1) (*multiplicador) = 2;

    if (naranjas_golpeados >= MULT2) (*multiplicador) = 3;

    if (naranjas_golpeados >= MULT3) (*multiplicador) = 5;

    if (naranjas_golpeados >= MULT4) (*multiplicador) = 10;
}
