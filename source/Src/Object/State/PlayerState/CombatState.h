#pragma once
#include "../StateBase.h"

class Player;

class CombatState :
    public StateBase
{
public:


    CombatState(Player& player);
private:
    Player& player_;

    virtual void Update(void)override;


};

