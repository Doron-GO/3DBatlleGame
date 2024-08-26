#pragma once
#include "DamageObject.h"
class DeathBall :
    public DamageObject
{
public:

    DeathBall(int playerType,const VECTOR& pos);
    ~DeathBall();

    virtual void Draw(void) override;
    virtual void Update(VECTOR enemyPos) override;

    virtual void Activate(void)override;
    virtual void InActivate(void)override;
    virtual const int& GetModelId(void)const override;

    void WaitUpdate(void);
    void NormalUpdate(void);

private:

    //èoåªÇ∑ÇÈYç¿ïW
    static constexpr float POS_DETHBALL_Y = 5500.0f;

    enum class BALL_STATE
    {
        BALL,
        HIT
    };

    void (DeathBall::* _update)();

    const VECTOR& playerPos_;
};

