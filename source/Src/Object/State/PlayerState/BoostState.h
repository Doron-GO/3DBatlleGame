#pragma once
#include "../StateBase.h"

class Player;

class BoostState :
    public StateBase
{

public:



    BoostState(Player& player);

private:

    //ブースト中の硬直時間
    static constexpr float MAX_BOOST_TIME = 20.0f;

    //ブースト時間計測
    float time_;


    Player& player_;
    virtual void Update()override;
};

