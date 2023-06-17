#include "trayectoria.h"

#include <SDL_render.h>

#include "config.h"
#include "utility.h"
#include "vec2.h"

Trajectory::Trajectory() {}

bool Trajectory::agregar_coordenada(aVec2 p) {
    linea.push_back(p);
    return true;
}

bool Trajectory::dibujar(SDL_Renderer* renderer) {
    for (const aVec2& p : linea) {
        SDL_RenderDrawPointF(renderer, p.x, p.y);
    }
    // linea.dibujar(renderer);
    return true;
}

Trajectory calcular(aVec2 pi, aVec2 vi, aVec2 a, float dt) {
    Trajectory actual;
    while (pi.x > MIN_X and pi.x < MAX_X and pi.y < MAX_Y) {
        actual.agregar_coordenada(pi);
        vi = ROZAMIENTO * computar_velocidad(vi, a, dt);
        pi = computar_posicion(pi, vi, dt);
    }
    return actual;
}
