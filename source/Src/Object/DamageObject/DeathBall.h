#pragma once
#include "DamageObject.h"
class DeathBall :
    public DamageObject
{
public:

    DeathBall(int playerType,const VECTOR& pos);
    ~DeathBall();
    //描画
    virtual void Draw(void) override;
    //更新
    virtual void Update(VECTOR enemyPos) override;
    //有効化
    virtual void Activate(void)override;
    //非有効化
    virtual void InActivate(void)override;
    //モデルIDの取得
    virtual const int& GetModelId(void)const override;
    //待機時更新
    void WaitUpdate(void);
    //通常時更新
    void NormalUpdate(void);

private:

    //出現するY座標
    static constexpr float POS_DETHBALL_Y = 5500.0f;

    enum class BALL_STATE
    {
        BALL,
        HIT
    };

    void (DeathBall::* _update)();

    const VECTOR& playerPos_;
};

