#pragma once
#include "../StateBase.h"

class Player;

class WinnerState :
    public StateBase
{
public:

    WinnerState(Player& player);
    


private:

        //更新
    virtual void Update(void)override;

    //プレイヤー情報
    Player& player_;

};

