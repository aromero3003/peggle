#ifndef __PEGGLE_H__
#define __PEGGLE_H__

#include "ball.h"
#include "cannon.h"
#include "game.h"
#include "hybrid_renderer.h"
#include "level.h"
#include "lifes.h"
#include "retriever.h"
#include "trajectory.h"

class Peggle {
   public:
    Peggle();
    Peggle(Peggle &&) = default;
    Peggle(const Peggle &) = delete;
    Peggle &operator=(Peggle &&) = delete;
    Peggle &operator=(const Peggle &) = delete;
    ~Peggle() = default;

    void run();

    void gameLevelUp(Game &game, Level **l);
    void gameLevelFailed(Game &game, Level **l);
    void gameOver(Game &g);
    void gameRunning(Game &g, Level **l);

   private:
    HybridRenderer r;
    Ball b;
    Cannon canon;
    Trajectory tray;
    Lifes vidas;
    Retriever recuperador;
    SDL_Event event;
    puntaje_t puntaje_total = 0;
    size_t contador_trayectoria = 0;
};

#endif /* __PEGGLE_H__ */
