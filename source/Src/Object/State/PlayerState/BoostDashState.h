#pragma once
#include "../StateBase.h"

class Player;

class BoostDashState :
    public StateBase
{

public:

    BoostDashState(Player& player);

private:

    Player& player_;

    virtual void Update(void)override;

};

