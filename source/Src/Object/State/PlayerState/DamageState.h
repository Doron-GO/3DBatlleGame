#pragma once
#include "../StateBase.h"

class Player;

class DamageState :
    public StateBase
{

public:

    DamageState(Player& player);
    virtual void Update()override;


private:

    Player& player_;
    float count_;

};



