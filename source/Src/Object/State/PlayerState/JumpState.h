#pragma once
#include"../../Actor/Player/Player.h"
class Player;

class JumpState :
    public StateBase
{
public:

    JumpState(Player& player);

private:

    virtual void Update(void)override;

    Player& player_;

};

