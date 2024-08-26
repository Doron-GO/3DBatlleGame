#pragma once
#include "DamageObject.h"
class Saber :
    public DamageObject
{
public:

    Saber(int playerType,Transform& transform);
    virtual void Update(VECTOR enemyPos)override;
    virtual void Draw(void)override;
    void Hit(void);
    virtual const int& GetModelId(void)const override;

private:

    static constexpr VECTOR HIT_EFFECT_SCARE = { 60.0f,60.0f,60.0f };


    enum class SABER_STATE
    {
        SLASH,
        HIT
    };
    Transform& transform_;
};

