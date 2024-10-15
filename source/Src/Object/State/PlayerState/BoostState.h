#pragma once
#include "../StateBase.h"

class Player;

class BoostState :
    public StateBase
{

public:

    BoostState(Player& player);

private:


    //ブースト時間計測
    float boostTime_;

    Player& player_;

    virtual void Update(void)override;
};

