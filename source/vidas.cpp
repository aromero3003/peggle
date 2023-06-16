#include "vidas.h"

#include "config.h"
#include "vec2.h"

vidas_t::vidas_t(unsigned int n, float cx, float cy)
    : start_position(cx, cy), total(n), dibujadas(n - 1) {
    if (n == 0) throw -1;  // TODO lanzar excepcion más descriptiva
}

bool vidas_t::estan_agotadas() const { return restantes() == 0; }

size_t vidas_t::restantes() const { return dibujadas; }

void vidas_t::resetear() { dibujadas = total; }

void vidas_t::quitar() { dibujadas--; }

aVec2 vidas_t::position() const { return start_position; }

// bool vidas_t::dibujar(SDL_Renderer *renderer) {
//     for (size_t i = 0; i < dibujadas; i++) figuras[i].dibujar(renderer);
//     return true;
// }
