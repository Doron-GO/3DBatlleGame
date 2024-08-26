#pragma once
#include "../StateBase.h"

class Player;

class BoostState :
    public StateBase
{

public:



    BoostState(Player& player);

private:

    //�u�[�X�g���̍d������
    static constexpr float MAX_BOOST_TIME = 20.0f;

    //�u�[�X�g���Ԍv��
    float time_;


    Player& player_;
    virtual void Update()override;
};

