#pragma once
#include "../StateBase.h"

class Player;

class WinnerState :
    public StateBase
{
public:

    WinnerState(Player& player);
    


private:

        //�X�V
    virtual void Update()override;

    //�v���C���[���
    Player& player_;

};

