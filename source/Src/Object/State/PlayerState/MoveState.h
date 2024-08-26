#pragma once
#include "../StateBase.h"

class Player;
class MoveState :
    public StateBase
{
public:

    MoveState(Player& player);
    virtual void Update()override;

private:
    Player& player_;



};

