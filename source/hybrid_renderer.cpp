#include "hybrid_renderer.h"

#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

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
    writeText("Peggle++", aVec2(VENTANA_ANCHO / 2., 20), true);
}

void HybridRenderer::drawLevel(Level *level) { Renderer::drawLevel(level); }
void HybridRenderer::drawLifes(Lifes &lifes) { Renderer::drawLifes(lifes); }

void HybridRenderer::writeText(const char *s, aVec2 position, bool centered) {
    SDL_Color color = {0xFF, 0xFF, 0xFF};
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);

    SDL_Rect rect;
    rect.x = centered ? position.x - (int)(surface->w / 2) : position.x;
    rect.y = centered ? position.y - (int)(surface->h / 2) : position.y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(r, texture, nullptr, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
