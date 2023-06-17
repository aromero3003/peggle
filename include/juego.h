#ifndef PEGGLE_H
#define PEGGLE_H

#include <cstddef>
#include <vector>

#include "level.h"
#include "loader.h"

typedef enum {
    GAME_CLOSE,
    GAME_RUNNING,
    GAME_LEVEL_UP,
    GAME_LEVEL_FAILED,
    GAME_OVER
} game_state_t;

typedef size_t puntaje_t;

struct Game {
   public:
    Game(Loader &l);
    Game(Game &&) = default;
    Game(const Game &) = default;
    Game &operator=(Game &&) = default;
    Game &operator=(const Game &) = default;
    ~Game() = default;

    game_state_t getState() const;
    size_t getTotalLevels() const;
    size_t getCurrentLevelId() const;
    Level *getCurrentLevel();

    bool levelUp();
    bool levelDown();

    // private:
    game_state_t state;
    std::vector<Level> levels;
    size_t current_level;
};

#endif
