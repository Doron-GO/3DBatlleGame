#pragma once
#include"../../Actor/Player/Player.h"
class Player;

class JumpState :
    public StateBase
{
public:

    JumpState(Player& player);
    virtual void Update()override;

private:
    Player& player_;

};

