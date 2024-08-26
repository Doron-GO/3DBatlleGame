#pragma once
#include "../StateBase.h"

class Player;

class CombatState :
    public StateBase
{
public:

    static constexpr float COMBAT_STAN_TIME = 20.0f;//äiì¨çdíºéûä‘

    CombatState(Player& player);
private:
    Player& player_;
    virtual void Update()override;


};

