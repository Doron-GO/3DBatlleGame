#pragma once
#include "../StateBase.h"

class BossEnemy;

class BarrageState :
    public StateBase
{
public:

    BarrageState(BossEnemy& bossEnemy);
private:
    virtual void Update(void)override;

    BossEnemy& bossEnemy_;

};

