#include "level.h"

#include <SDL_render.h>

#include "bola.h"
#include "config.h"
#include "obstaculo.h"
#include "vec2.h"

Level::Level(std::list<Obstacle> &obstaculos)
    : obstaculos(obstaculos),
      naranjas(0),
      naranjas_golpeados(0),
      score(0),
      multiplier(1) {
    for (const Obstacle &obs : obstaculos) naranjas += obs.es_naranja();
}

size_t Level::update_score(const Obstacle &obs) {
    if (obs.get_tocado()) return 0;

    size_t puntaje_azul = 10;
    switch (obs.get_color()) {
        case COLOR_AZUL:
            // Las azules valen 20 puntos
            break;

        case COLOR_NARANJA:
            puntaje_azul *= 10;
            break;

        case COLOR_VERDE:
            puntaje_azul *= 50;  // como las moradas;
            break;

        case COLOR_GRIS:
            puntaje_azul *= 0;
            break;
    }
    // *puntos_totales = *puntos_totales + puntaje_azul * multiplicador;
    score += puntaje_azul * multiplier;
    return puntaje_azul * multiplier;
}

size_t Level::update_multiplier() {
    if (naranjas_golpeados < MULT1) multiplier = 1;
    if (naranjas_golpeados >= MULT1) multiplier = 2;
    if (naranjas_golpeados >= MULT2) multiplier = 3;
    if (naranjas_golpeados >= MULT3) multiplier = 5;
    if (naranjas_golpeados >= MULT4) multiplier = 10;
    return multiplier;
}

void Level::handle_collisions(Ball &bola) {
    for (auto &obs : obstaculos) {
        if (obs.get_dibujar()) {
            // obs.dibujar(renderer);
            //
            aVec2 norma(aVec2Zero);
            if (obs.distancia(bola.position(), norma) < BOLA_RADIO) {
                bola.reflejar(norma);
                if (obs.es_gris()) continue;

                naranjas_golpeados += obs.es_naranja() and obs.get_dibujar() and
                                      not obs.get_tocado();
                // puntaje_actualizar(&puntaje_en_nivel, obs, multiplicador);
                update_score(obs);
                obs.set_tocado(true);
            }
        }
    }
}

void Level::reset() {
    naranjas_golpeados = 0;
    score = 0;
    multiplier = 1;
    for (auto &obs : obstaculos) {
        obs.set_tocado(false);
        obs.set_dibujar(true);
    }
}

void Level::move_obstacles(double dt) {
    for (auto &obs : obstaculos) obs.mover(dt);
}

void Level::clean_touched_obstacles() {
    for (auto &obs : obstaculos)
        if (obs.get_tocado()) obs.set_dibujar(false);
}

bool Level::is_completed() const { return naranjas == naranjas_golpeados; }

void Level::draw(SDL_Renderer *r) {
    for (auto &obs : obstaculos) obs.dibujar(r);
}
