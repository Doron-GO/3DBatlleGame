#pragma once
#include "../StateBase.h"

class Player;

class LoseState :
    public StateBase
{
public:


    LoseState(Player& player);
private:

    Player& player_;

    virtual void Update()override;

};

