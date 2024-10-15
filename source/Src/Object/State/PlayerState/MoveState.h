#pragma once
#include "../StateBase.h"

class Player;
class MoveState :
    public StateBase
{
public:

    MoveState(Player& player);


private:

    //更新
    virtual void Update(void)override;

    //プレイヤー情報
    Player& player_;

};

