#pragma once
#include "../StateBase.h"

class Player;

class FirstComboState :
    public StateBase
{
public:

    FirstComboState(Player& player);
    virtual void Update()override;

private:

    Player& player_;

};

