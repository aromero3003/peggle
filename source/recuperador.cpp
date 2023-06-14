#include "recuperador.h"

#include "config.h"
#include "vec2.h"

recuperador_t::recuperador_t(float ancho, float alto, float velocidad)
    : velocidad(1), xi(MIN_X), ancho(ancho), r(nullptr, 0) {
    float aux[4][2] = {{MIN_X, MAX_Y},
                       {MIN_X, MAX_Y - alto},
                       {MIN_X + ancho, MAX_Y - alto},
                       {MIN_X + ancho, MAX_Y}};
    r = poligono_t(aux, 4);
}
void recuperador_t::mover(float dt) {
    if (xi < MIN_X || xi + ancho > MAX_X) velocidad = -velocidad;
    r.trasladar(aVec2(velocidad * dt, 0));
    xi += (velocidad * dt);
}
void recuperador_t::dibujar(SDL_Renderer *renderer) const {
    r.dibujar(renderer);
}

bool recuperador_t::bola_recuperada(float cx, float cy) const {
    return r.punto_dentro(aVec2(cx, cy));
}
