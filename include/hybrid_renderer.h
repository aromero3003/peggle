#ifndef __HYBRID_RENDERER_H__
#define __HYBRID_RENDERER_H__

#include <SDL_ttf.h>

#include "level.h"
#include "lifes.h"
#include "renderer.h"

class HybridRenderer : public Renderer {
   public:
    HybridRenderer() = delete;
    HybridRenderer(int width, int height, uint32_t flags);

    HybridRenderer(HybridRenderer &&) = default;
    HybridRenderer(const HybridRenderer &) = delete;
    HybridRenderer &operator=(HybridRenderer &&) = default;
    HybridRenderer &operator=(const HybridRenderer &) = delete;
    ~HybridRenderer();

    void drawTitle();
    void drawLevel(Level *level);
    // void drawScore();
    void drawLifes(Lifes &lifes);

   private:
    TTF_Font *font;
};

#endif /* __HYBRID_RENDERER_H__ */
