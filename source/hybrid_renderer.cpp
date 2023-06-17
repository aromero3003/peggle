#include "hybrid_renderer.h"

#include <SDL_ttf.h>

#include "level.h"
#include "renderer.h"

HybridRenderer::HybridRenderer(int width, int height, uint32_t flags)
    : Renderer(width, height, flags) {
    TTF_Init();
    font = TTF_OpenFont("resources/FreeSansBold.ttf", 24);
}

HybridRenderer::~HybridRenderer() {
    TTF_CloseFont(font);
    TTF_Quit();
}

void HybridRenderer::drawLevel(Level *level) { Renderer::drawLevel(level); }
void HybridRenderer::drawLifes(Lifes &lifes) { Renderer::drawLifes(lifes); }
