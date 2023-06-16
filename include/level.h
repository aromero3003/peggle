#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <SDL_render.h>

#include <list>

#include "bola.h"
#include "obstaculo.h"

class Level {
   public:
    Level(std::list<obstaculo_t> &obstaculos);
    Level(Level &&) = default;
    Level(const Level &) = delete;
    Level &operator=(Level &&) = default;
    Level &operator=(const Level &) = delete;
    ~Level() = default;

    void handle_collisions(bola_t &bola);
    void clean_touched_obstacles();
    size_t update_score(const obstaculo_t &obs);
    size_t update_multiplier();
    void draw(SDL_Renderer *renderer);

   private:
    std::list<obstaculo_t> obstaculos;
    size_t naranjas, naranjas_golpeados;
    size_t score, multiplier;
};

#endif /* __LEVEL_H__ */
