#pragma once
#include"../../Actor/Player/Player.h"
class Player;

class JumpState :
    public StateBase
{
public:

    JumpState(Player& player);

private:

    virtual void Update()override;

    Player& player_;

};

