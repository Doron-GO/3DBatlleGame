#pragma once
#include "../StateBase.h"

class Player;
class MoveState :
    public StateBase
{
public:

    MoveState(Player& player);


private:

    //�X�V
    virtual void Update(void)override;

    //�v���C���[���
    Player& player_;

};

