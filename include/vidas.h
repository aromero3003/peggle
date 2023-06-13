#ifndef __VIDAS_H__
#define __VIDAS_H__

#include <vector>

#include "bola.h"

struct vidas_t {
    vidas_t(size_t n, float cx, float cy);

    // void vidas_agregar(vidas_t *vidas);
    void quitar();

    size_t restantes();
    bool estan_agotadas();

    bool dibujar(SDL_Renderer *renderer);
    void resetear();

   private:
    std::vector<bola_t> figuras;
    int dibujadas;
};

#endif /* __VIDAS_H__ */
