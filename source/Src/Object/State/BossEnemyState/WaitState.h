#pragma once
#include "../StateBase.h"

class BossEnemy;

class WaitState :
    public StateBase
{
public:

    WaitState(BossEnemy& bossEnemy);
private:
    virtual void Update(void)override;

    BossEnemy& bossEnemy_;

};

