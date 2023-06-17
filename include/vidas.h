#ifndef __VIDAS_H__
#define __VIDAS_H__

#include <cstddef>
#include <vector>

#include "vec2.h"

struct Lifes {
    Lifes(unsigned int n, float cx, float cy);

    // void vidas_agregar(Lifes *vidas);
    void quitar();

    size_t restantes() const;
    bool estan_agotadas() const;
    aVec2 position() const;

    void resetear();

   private:
    aVec2 start_position;
    unsigned int total;
    unsigned int dibujadas;
};

#endif /* __VIDAS_H__ */
