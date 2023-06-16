#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <SDL_render.h>

#include "bola.h"
#include "cannon.h"
#include "vidas.h"
class Renderer {
   public:
    Renderer(SDL_Renderer *renderer);
    Renderer(Renderer &&) = default;
    Renderer(const Renderer &) = delete;
    Renderer &operator=(Renderer &&) = default;
    Renderer &operator=(const Renderer &) = delete;
    ~Renderer() = default;

    void drawScenario() const;
    void drawBall(const bola_t &ball) const;
    void drawCannon(const Cannon &cannon) const;
    void drawLifes(const vidas_t &vidas) const;

   private:
    SDL_Renderer *r;
};

#endif /* __RENDERER_H__ */
