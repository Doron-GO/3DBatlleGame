#pragma once
#include "../StateBase.h"

class Player;

class DownState :
    public StateBase
{
public:

    DownState(Player& player);


private:
    Player& player_;

    virtual void Update(void)override;

};

