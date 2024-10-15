#pragma once
#include "../StateBase.h"

class Player;

class DamageState :
    public StateBase
{

public:

    DamageState(Player& player);


private:

    Player& player_;

    virtual void Update(void)override;

    float count_;

};



