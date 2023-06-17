#include "lifes.h"

#include "settings.h"
#include "vec2.h"

Lifes::Lifes(unsigned int n, float cx, float cy)
    : start_position(cx, cy), total(n), dibujadas(n - 1) {
    if (n == 0) throw -1;  // TODO lanzar excepcion más descriptiva
}

bool Lifes::estan_agotadas() const { return restantes() == 0; }

size_t Lifes::restantes() const { return dibujadas; }

void Lifes::resetear() { dibujadas = total; }

void Lifes::quitar() { dibujadas--; }

aVec2 Lifes::position() const { return start_position; }

// bool Lifes::dibujar(SDL_Renderer *renderer) {
//     for (size_t i = 0; i < dibujadas; i++) figuras[i].dibujar(renderer);
//     return true;
// }
