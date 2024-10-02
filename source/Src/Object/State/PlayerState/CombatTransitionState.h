#pragma once
#include "../StateBase.h"

class Player;

class CombatTransitionState :
    public StateBase
{
public:

    static constexpr float SPEED = 80.0f;
    static constexpr float RATE = 60.0f;


    CombatTransitionState(Player& player,VECTOR& playerPos, VECTOR& movePow, Quaternion& quarot, float offset);
private:
    Player& player_;
    virtual void Update()override;

    VECTOR dashVec_;
    VECTOR& playerPos_;
    VECTOR& movePow_;
    VECTOR enemyPos_;
    Quaternion& quarot_;
    float distance_;

    //‘ŠŽè‚Æ‚Ì‹——£‚ðŒv‘ª‚·‚é
    float Distance(void);
};

