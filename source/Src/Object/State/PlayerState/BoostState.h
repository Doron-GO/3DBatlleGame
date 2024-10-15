#pragma once
#include "../StateBase.h"

class Player;

class BoostState :
    public StateBase
{

public:

    BoostState(Player& player);

private:


    //�u�[�X�g���Ԍv��
    float boostTime_;

    Player& player_;

    virtual void Update(void)override;
};

