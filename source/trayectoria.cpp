#include "trayectoria.h"

#include "config.h"

bool trayectoria_t::agregar_coordenada(float x, float y) {
    linea.agregar_vertice(x, y);
    return true;
}

bool trayectoria_t::dibujar(SDL_Renderer* renderer) {
    linea.dibujar(renderer);
    return true;
}

double computar_velocidad(double vi, double a, double dt) {
    return vi + (a * dt);
}

double computar_posicion(double pi, double vi, double dt) {
    return pi + (vi * dt);
}

trayectoria_t calcular(float xi, float yi, float vxi, float vyi, float a,
                       float dt) {
    trayectoria_t actual;
    while (xi > MIN_X && xi < MAX_X && yi < MAX_Y) {
        actual.agregar_coordenada(xi, yi);
        vyi = computar_velocidad(vyi, a, dt);
        vxi *= ROZAMIENTO;
        vyi *= ROZAMIENTO;
        xi = computar_posicion(xi, vxi, dt);
        yi = computar_posicion(yi, vyi, dt);
    }
    return actual;
}
