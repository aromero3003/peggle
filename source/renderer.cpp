#include "renderer.h"

#include <SDL_render.h>

#include "config.h"

Renderer::Renderer(SDL_Renderer *renderer) : r(renderer) {}

void Renderer::drawScenario() {
    SDL_SetRenderDrawColor(r, 0x00, 0xFF, 0x00, 0x00);
    SDL_RenderDrawLine(r, MIN_X, MIN_Y, MAX_X, MIN_Y);
    SDL_RenderDrawLine(r, MIN_X, MAX_Y, MAX_X, MAX_Y);
    SDL_RenderDrawLine(r, MIN_X, MAX_Y, MIN_X, MIN_Y);
    SDL_RenderDrawLine(r, MAX_X, MAX_Y, MAX_X, MIN_Y);
}
