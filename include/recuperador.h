#ifndef __RECUPERADOR_H__
#define __RECUPERADOR_H__

#include "poligono.h"

class recuperador_t {
   public:
    recuperador_t() = default;
    recuperador_t(recuperador_t &&) = default;
    recuperador_t(const recuperador_t &) = default;
    recuperador_t &operator=(recuperador_t &&) = default;
    recuperador_t &operator=(const recuperador_t &) = default;
    ~recuperador_t() = default;

    recuperador_t(float ancho, float alto, float velocidad);
    void mover(float dt);
    void dibujar(SDL_Renderer *renderer);

    bool bola_recuperada(float cx, float cy);

   private:
    poligono_t r;
    float velocidad;
    float xi;
    float ancho;
};

#endif /* __RECUPERADOR_H__ */
