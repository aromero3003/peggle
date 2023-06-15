#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <SDL_render.h>
class Renderer {
   public:
    Renderer(SDL_Renderer *renderer);
    Renderer(Renderer &&) = default;
    Renderer(const Renderer &) = delete;
    Renderer &operator=(Renderer &&) = default;
    Renderer &operator=(const Renderer &) = delete;
    ~Renderer() = default;

    void drawScenario();

   private:
    SDL_Renderer *r;
};

#endif /* __RENDERER_H__ */
