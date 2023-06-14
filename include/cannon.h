#ifndef __CANNON_H__
#define __CANNON_H__

#include "vec2.h"
class Cannon {
   public:
    Cannon(float x0, float y0);
    Cannon(Cannon &&) = default;
    Cannon(const Cannon &) = delete;
    Cannon &operator=(Cannon &&) = default;
    Cannon &operator=(const Cannon &) = delete;
    ~Cannon() = default;

    void update(float angle);
    void tip(float &x, float &y);

   private:
    aVec2 reference_tip;
    aVec2 bottom;
};

#endif /* __CANNON_H__ */
