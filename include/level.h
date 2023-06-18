#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <SDL_render.h>

#include <cstddef>
#include <list>

#include "ball.h"
#include "obstacle.h"

class Level {
   public:
    Level(std::list<Obstacle> &obstaculos);
    Level(Level &&) = default;
    Level(const Level &) = delete;
    Level &operator=(Level &&) = default;
    Level &operator=(const Level &) = delete;
    ~Level() = default;

    void move_obstacles(double dt);
    void handle_collisions(Ball &bola);
    void clean_touched_obstacles();
    size_t update_score(const Obstacle &obs);
    size_t update_multiplier();
    void reset();
    void draw(SDL_Renderer *renderer);

    bool is_completed() const;
    size_t get_id() const;
    size_t get_hit_oranges() const;
    size_t get_total_oranges() const;
    size_t get_multiplier() const;
    size_t get_score() const;

   private:
    std::list<Obstacle> obstaculos;
    size_t id;
    size_t naranjas, naranjas_golpeados;
    size_t score, multiplier;
    static size_t global_count;  // Increiblemente ILEGAL
};

#endif /* __LEVEL_H__ */
