#ifndef __RETRIEVER_H__
#define __RETRIEVER_H__

#include "ball.h"
#include "polygon.h"

class Retriever {
   public:
    Retriever() = delete;
    Retriever(Retriever &&) = default;
    Retriever(const Retriever &) = default;
    Retriever &operator=(Retriever &&) = default;
    Retriever &operator=(const Retriever &) = default;
    ~Retriever() = default;

    Retriever(float ancho, float alto, float velocidad);
    void mover(float dt);
    void dibujar(SDL_Renderer *renderer) const;

    bool recuperar(Ball &b) const;

   private:
    Polygon r;
    float velocidad;
    float xi;
    float ancho;
};

#endif /* __RETRIEVER_H__ */
