#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <SDL_render.h>
#include <SDL_video.h>

#include <cstdint>

#include "ball.h"
#include "cannon.h"
#include "level.h"
#include "lifes.h"
#include "retriever.h"
#include "trajectory.h"
#include "vec2.h"

class Renderer {
   public:
    Renderer(int width, int height, uint32_t flags);
    Renderer(Renderer &&) = default;
    Renderer(const Renderer &) = delete;
    Renderer &operator=(Renderer &&) = default;
    Renderer &operator=(const Renderer &) = delete;
    ~Renderer();

    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;

    void drawScenario() const;
    void drawBall(const Ball &ball) const;
    void drawCannon(const Cannon &cannon) const;
    void drawLifes(const Lifes &vidas) const;
    void drawTrajectory(Trajectory &traj);
    void drawRetriever(const Retriever &ret);
    void drawLevel(Level *level);

    void present();
    void clear();

   protected:
    SDL_Window *w;
    SDL_Renderer *r;

    void drawLine(aVec2 a, aVec2 b) const;
    void drawPoint(aVec2 p) const;
    void drawCircle(aVec2 center, float radius, uint32_t num_segments) const;
    void drawFilledCircle(aVec2 center, float radius,
                          uint32_t num_segments) const;
};

#endif /* __RENDERER_H__ */
