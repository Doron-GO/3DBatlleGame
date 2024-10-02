#pragma once
#include "../StateBase.h"

class Player;

class FallState :
    public StateBase
{
public:

    FallState(Player& player);

private:
    Player& player_;

    virtual void Update()override;

};

