#ifndef IMPSPRITE_H
#define IMPSPRITE_H

#include "EnnemyEntity.h"

class ImpEntity : public EnnemyEntity
{
  public:
    enum impTypeEnum { ImpTypeRed, ImpTypeBlue };
    ImpEntity(float x, float y, impTypeEnum impType);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);
    virtual bool hurt(int damages, enumShotType hurtingType, int level);

	  virtual void dying();
  private:
    impTypeEnum impType;
    float changingDelay;
    int state;
    void fire();
    void teleport();
    void generateStar(sf::Color starColor);
};

#endif // BATSPRITE_H