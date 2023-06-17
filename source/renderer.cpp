#include "renderer.h"

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "ball.h"
#include "cannon.h"
#include "settings.h"
#include "trajectory.h"
#include "vec2.h"

Renderer::Renderer(int width, int height, uint32_t flags) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width, height, flags, &w, &r);
    SDL_SetWindowTitle(w, "Peggle++");
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
}

void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(this->r, r, g, b, a);
}

void Renderer::present() { SDL_RenderPresent(r); }

void Renderer::clear() { SDL_RenderClear(r); }

void Renderer::drawScenario() const {
    SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0x00);
    SDL_RenderDrawLine(r, MIN_X, MIN_Y, MAX_X, MIN_Y);
    SDL_RenderDrawLine(r, MIN_X, MAX_Y, MAX_X, MAX_Y);
    SDL_RenderDrawLine(r, MIN_X, MAX_Y, MIN_X, MIN_Y);
    SDL_RenderDrawLine(r, MAX_X, MAX_Y, MAX_X, MIN_Y);
}

void Renderer::drawCannon(const Cannon &cannon) const {
    aVec2 cannon_tip = cannon.tip();
    SDL_RenderDrawLine(r, CANON_X, CANON_Y, cannon_tip.x, cannon_tip.y);
}

void Renderer::drawBall(const Ball &ball) const {
    drawCircle(ball.position(), BOLA_RADIO, 100);
}

void Renderer::drawTrajectory(Trajectory &traj) { traj.dibujar(r); }
void Renderer::drawRetriever(const Retriever &ret) { ret.dibujar(r); }
void Renderer::drawLevel(Level *level) { level->draw(r); }

void Renderer::drawLifes(const Lifes &vidas) const {
    aVec2 p = vidas.position();
    for (size_t i = 0; i < vidas.restantes(); i++, p.y += BOLA_RADIO * 3)
        drawCircle(p, BOLA_RADIO, BOLA_RESOL);
}

void Renderer::drawCircle(aVec2 center, float radius,
                          uint32_t num_segments) const {
    // Calculate the angle step between line segments
    float angle_step = (2.0f * M_PI) / static_cast<float>(num_segments);
    float angle = 0.0f;

    // Set the draw color to red
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

    // Draw the outline of the circle using multiple lines
    for (uint32_t i = 0; i < num_segments; ++i) {
        float x1 = center.x + radius * cos(angle);
        float y1 = center.y + radius * sin(angle);
        float x2 = center.x + radius * cos(angle + angle_step);
        float y2 = center.y + radius * sin(angle + angle_step);

        SDL_RenderDrawLineF(r, x1, y1, x2, y2);

        angle += angle_step;
    }
}
