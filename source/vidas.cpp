#include "vidas.h"

#include "config.h"
#include "vec2.h"

vidas_t::vidas_t(size_t n, float cx, float cy) {
    for (int i = 0; i < n - 1; i++)
        figuras.emplace_back(aVec2(cx, cy + BOLA_RADIO * 3 * i), BOLA_RADIO,
                             BOLA_RESOL);
    dibujadas = n - 1;
}

bool vidas_t::estan_agotadas() { return restantes() == 0; }

size_t vidas_t::restantes() { return dibujadas; }

void vidas_t::resetear() { dibujadas = figuras.size(); }

void vidas_t::quitar() { (dibujadas)--; }

bool vidas_t::dibujar(SDL_Renderer *renderer) {
    for (size_t i = 0; i < dibujadas; i++) figuras[i].dibujar(renderer);
    return true;
}
