#ifndef __HYBRID_RENDERER_H__
#define __HYBRID_RENDERER_H__

#include <SDL_ttf.h>

#include <string>

#include "level.h"
#include "lifes.h"
#include "renderer.h"
#include "vec2.h"

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
    void drawLevelUp(Level *level);
    void drawLevelFailed(Level *level);
    void drawLifes(Lifes &lifes);
    void drawGameOver(size_t total_score);

   private:
    TTF_Font *font;

    void writeText(const std::string &s, aVec2 position, bool centered,
                   float scale);

    void drawScore(Level *level);
};

#endif /* __HYBRID_RENDERER_H__ */
