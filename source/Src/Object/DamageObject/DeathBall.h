#pragma once
#include<functional>
#include "DamageObject.h"
class DeathBall :
    public DamageObject
{
public:

    DeathBall(int playerType,const VECTOR& pos);
    ~DeathBall();
    //�`��
    virtual void Draw(void) override;
    //�X�V
    virtual void Update(VECTOR enemyPos) override;
    //�L����
    virtual void Activate(void)override;
    //��L����
    virtual void InActivate(void)override;
    //���f��ID�̎擾
    virtual const int& GetModelId(void)const override;
    //�ҋ@���X�V
    void WaitUpdate(void);
    //�ʏ펞�X�V
    void NormalUpdate(void);

private:

    //�o������Y���W
    static constexpr float POS_DETHBALL_Y = 5500.0f;

    enum class BALL_STATE
    {
        BALL,
        HIT
    };

    void (DeathBall::* _update)();
    std::function<void(void)> update_;

    const VECTOR& playerPos_;
};

