#pragma once
#include "../StateBase.h"

class BossEnemy;

class EarthQuakeState :
    public StateBase
{
public:

    EarthQuakeState(BossEnemy& bossEnemy);
private:
    virtual void Update()override;

    BossEnemy& bossEnemy_;

};

