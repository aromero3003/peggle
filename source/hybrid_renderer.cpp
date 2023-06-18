#include "hybrid_renderer.h"

#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

#include <sstream>
#include <string>

#include "level.h"
#include "renderer.h"
#include "settings.h"
#include "vec2.h"

HybridRenderer::HybridRenderer(int width, int height, uint32_t flags)
    : Renderer(width, height, flags) {
    TTF_Init();
    font = TTF_OpenFont("resources/FreeSansBold.ttf", 24);
}

HybridRenderer::~HybridRenderer() {
    TTF_CloseFont(font);
    TTF_Quit();
}

void HybridRenderer::drawTitle() {
    writeText("Peggle++", aVec2(VENTANA_ANCHO / 2., 32), true, 1);
}

void HybridRenderer::drawLevel(Level *level) {
    Renderer::drawLevel(level);
    std::string level_id = "Level " + std::to_string(level->get_id());
    writeText(level_id.c_str(), aVec2(MIN_X, MIN_Y / 2.), false, 0.8);

    std::stringstream ss;
    ss << level->get_hit_oranges() << "/" << level->get_total_oranges();
    writeText(ss.str().c_str(), aVec2(MAX_X + 15, MAX_Y - 30), false, 1);

    drawScore(level);

    SDL_SetRenderDrawColor(r, 0xFF, 0x60, 0x00, 0x00);
    drawCircle(aVec2(MAX_X + 40, MAX_Y - 50), 10, BOLA_RESOL);
}

void HybridRenderer::drawScore(Level *level) {
    std::string score = "Score: " + std::to_string(level->get_score());
    std::string multiplier = "x" + std::to_string(level->get_multiplier());
    writeText(score, aVec2(600, MIN_Y / 2.), false, 0.8);
    writeText(multiplier, aVec2(730, 50), false, 0.8);
}
void HybridRenderer::drawLevelUp(Level *level) {
    std::string score = "SCORE: " + std::to_string(level->get_score());
    writeText("SUCCESS", aVec2(VENTANA_ANCHO / 2., 200), true, 1);
    writeText(score, aVec2(VENTANA_ANCHO / 2., 260), true, 1);
    writeText("Click to continue", aVec2(VENTANA_ANCHO / 2., 230), true, 1);
}

void HybridRenderer::drawLifes(Lifes &lifes) {
    Renderer::drawLifes(lifes);
    writeText(std::to_string(lifes.restantes()).c_str(),
              aVec2(MIN_X / 4., MIN_Y), false, 1);
}

void HybridRenderer::writeText(const std::string &s, aVec2 position,
                               bool centered, float scale) {
    SDL_Color color = {0xFF, 0xFF, 0xFF};
    SDL_Surface *surface = TTF_RenderText_Solid(font, s.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);

    SDL_Rect rect;
    rect.x = centered ? position.x - (int)(surface->w / 2) : position.x;
    rect.y = centered ? position.y - (int)(surface->h / 2) : position.y;
    rect.w = surface->w * scale;
    rect.h = surface->h * scale;

    SDL_RenderCopy(r, texture, nullptr, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
