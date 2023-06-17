#include "renderer.h"

#include <SDL_render.h>

#include "bola.h"
#include "cannon.h"
#include "config.h"
#include "vec2.h"

Renderer::Renderer(SDL_Renderer *renderer) : r(renderer) {}

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

void drawCircle(SDL_Renderer *renderer, float center_x, float center_y,
                uint32_t radius, uint32_t num_segments) {
    // Calculate the integer coordinates and radius
    int x = static_cast<int>(center_x);
    int y = static_cast<int>(center_y);
    int r = static_cast<int>(radius);

    // Calculate the angle step between line segments
    float angle_step = (2.0f * M_PI) / static_cast<float>(num_segments);
    float angle = 0.0f;

    // Set the draw color to red
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Draw the outline of the circle using multiple lines
    for (uint32_t i = 0; i < num_segments; ++i) {
        int x1 = static_cast<int>(x + r * cos(angle));
        int y1 = static_cast<int>(y + r * sin(angle));
        int x2 = static_cast<int>(x + r * cos(angle + angle_step));
        int y2 = static_cast<int>(y + r * sin(angle + angle_step));

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        angle += angle_step;
    }
}

void Renderer::drawBall(const Ball &ball) const {
    drawCircle(r, ball.position().x, ball.position().y, BOLA_RADIO, 100);
}

void Renderer::drawLifes(const Lifes &vidas) const {
    aVec2 p = vidas.position();
    for (size_t i = 0; i < vidas.restantes(); i++, p.y += BOLA_RADIO * 3) {
        drawCircle(r, p.x, p.y, BOLA_RADIO, BOLA_RESOL);
    }
}
