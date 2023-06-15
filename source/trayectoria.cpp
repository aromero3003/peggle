#include "trayectoria.h"

#include "config.h"
#include "utility.h"
#include "vec2.h"

trayectoria_t::trayectoria_t() : linea(nullptr, 0) {}

bool trayectoria_t::agregar_coordenada(aVec2 p) {
    linea.agregar_vertice(p);
    return true;
}

bool trayectoria_t::dibujar(SDL_Renderer* renderer) {
    linea.dibujar(renderer);
    return true;
}

trayectoria_t calcular(aVec2 pi, aVec2 vi, aVec2 a, float dt) {
    trayectoria_t actual;
    while (pi.x > MIN_X and pi.x < MAX_X and pi.y < MAX_Y) {
        actual.agregar_coordenada(pi);
        vi = computar_velocidad(vi, a, dt);
        vi *= ROZAMIENTO;
        pi = computar_posicion(pi, vi, dt);
    }
    return actual;
}
