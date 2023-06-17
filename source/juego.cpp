#include "juego.h"

#include "loader.h"

Game::Game(Loader &l) : current_level(0), state(GAME_RUNNING) {
    while (l.can_continue()) {
        levels.emplace_back(l.read_level());
    }
}

game_state_t Game::getState() const { return state; }

size_t Game::getTotalLevels() const { return levels.size(); }

size_t Game::getCurrentLevelId() const { return current_level; }

Level *Game::getCurrentLevel() {  // debería ser const?
    return &(levels[current_level]);
}

bool Game::levelUp() {
    if (current_level + 1 >= getTotalLevels()) return false;
    current_level++;
    return true;
}
bool Game::levelDown() {
    if (current_level == 0) return false;
    current_level--;
    return true;
}
