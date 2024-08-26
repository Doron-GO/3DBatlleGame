#pragma once
#include "../StateBase.h"

class BossEnemy;

class DeadState :
    public StateBase
{
public:

    DeadState(BossEnemy& bossEnemy);
private:
    virtual void Update()override;

    BossEnemy& bossEnemy_;


};

