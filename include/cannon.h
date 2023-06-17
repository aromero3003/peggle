#ifndef __CANNON_H__
#define __CANNON_H__

#include <SDL_render.h>

#include "bola.h"
#include "vec2.h"

class Cannon {
   public:
    // TODO que también reciba un largo de cannon
    Cannon(aVec2 p0, Ball &bala);
    Cannon(Cannon &&) = default;
    Cannon(const Cannon &) = delete;
    Cannon &operator=(Cannon &&) = delete;
    Cannon &operator=(const Cannon &) = delete;
    ~Cannon() = default;

    float angle() const;
    void update(float angle);

    bool fire();
    bool reload();

    aVec2 tip() const;
    void draw(SDL_Renderer *renderer) const;

   private:
    const aVec2 bottom;
    Ball &ball;
    float rotation;
};

#endif /* __CANNON_H__ */
