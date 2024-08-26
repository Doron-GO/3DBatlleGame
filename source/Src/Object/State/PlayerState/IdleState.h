#pragma once
#include "../StateBase.h"
class Player;

class IdleState :
    public StateBase
{
public:


    IdleState(Player& player);
private:
    Player& player_;
    virtual void Update()override;

};

