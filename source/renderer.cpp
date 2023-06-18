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

void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
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
    setColor(0xFF, 0xFF, 0xFF, 0xFF);
    drawLine(aVec2(CANON_X, CANON_Y), cannon.tip());
    // SDL_RenderDrawLine(r, CANON_X, CANON_Y, cannon_tip.x, cannon_tip.y);
}

void Renderer::drawBall(const Ball &ball) const {
    // drawCircle(ball.position(), BOLA_RADIO, 100);
    drawFilledCircle(ball.position(), BOLA_RADIO + 1, 100);
}

void Renderer::drawTrajectory(Trajectory &traj) { traj.dibujar(r); }
void Renderer::drawRetriever(const Retriever &ret) { ret.dibujar(r); }
void Renderer::drawLevel(Level *level) { level->draw(r); }

void Renderer::drawLifes(const Lifes &vidas) const {
    SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
    aVec2 p = vidas.position();
    for (size_t i = 1; i < vidas.restantes(); i++, p.y += BOLA_RADIO * 3)
        drawFilledCircle(p, BOLA_RADIO, BOLA_RESOL);
}

void Renderer::drawFilledCircle(aVec2 center, float radius,
                                uint32_t resolution) const {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        // Draw horizontal lines from (x, y) to (x, -y) to fill the circle
        for (int i = -x; i <= x; ++i) {
            SDL_RenderDrawLine(r, center.x + i, center.y + y, center.x + i,
                               center.y - y);
        }
        // Draw horizontal lines from (y, x) to (-y, x) to fill the circle
        for (int i = -y; i <= y; ++i) {
            SDL_RenderDrawLine(r, center.x + i, center.y + x, center.x + i,
                               center.y - x);
        }
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            --y;
        }
        ++x;
    }
}
void Renderer::drawCircle(aVec2 center, float radius,
                          uint32_t num_segments) const {
    float angle_step = (2.0f * M_PI) / static_cast<float>(num_segments);
    float angle = 0.0f;

    for (uint32_t i = 0; i < num_segments; ++i) {
        aVec2 p1 = center + radius * aVec2(cos(angle), sin(angle));
        angle += angle_step;
        aVec2 p2 = center + radius * aVec2(cos(angle), sin(angle));
        drawLine(p1, p2);
    }

    // float angleIncrement = 2 * M_PI / resolution;
    //
    // for (int i = 0; i < resolution; ++i) {
    //     float angle = i * angleIncrement;
    //     int x = center.x + static_cast<int>(radius * std::cos(angle));
    //     int y = center.y + static_cast<int>(radius * std::sin(angle));
    //     SDL_RenderDrawPoint(r, x, y);
    // }
}

void Renderer::drawLine(aVec2 a, aVec2 b) const {
    SDL_RenderDrawLine(r, a.x, a.y, b.x, b.y);
}

void Renderer::drawPoint(aVec2 p) const { SDL_RenderDrawPoint(r, p.x, p.y); }
