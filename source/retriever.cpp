#include "retriever.h"

#include "ball.h"
#include "settings.h"
#include "vec2.h"

Retriever::Retriever(float ancho, float alto, float velocidad)
    : velocidad(1), xi(MIN_X), ancho(ancho), r(nullptr, 0) {
    float aux[4][2] = {{MIN_X, MAX_Y},
                       {MIN_X, MAX_Y - alto},
                       {MIN_X + ancho, MAX_Y - alto},
                       {MIN_X + ancho, MAX_Y}};
    r = Polygon(aux, 4);
}
void Retriever::mover(float dt) {
    if (xi < MIN_X || xi + ancho > MAX_X) velocidad = -velocidad;
    r.trasladar(aVec2(velocidad * dt, 0));
    xi += (velocidad * dt);
}
void Retriever::dibujar(SDL_Renderer *renderer) const { r.dibujar(renderer); }

bool Retriever::recuperar(Ball &bola) const {
    return r.punto_dentro(bola.position());
}
