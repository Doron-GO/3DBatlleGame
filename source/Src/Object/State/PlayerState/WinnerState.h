#pragma once
#include "../StateBase.h"

class Player;

class WinnerState :
    public StateBase
{
public:

    WinnerState(Player& player);
    virtual void Update()override;


private:
    Player& player_;

};

