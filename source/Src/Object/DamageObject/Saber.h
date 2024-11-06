#pragma once
#include "DamageObject.h"
class Saber :
    public DamageObject
{
public:

    Saber(int playerType,Transform& transform);
    virtual void Update(VECTOR enemyPos)override;
    virtual void Draw(void)override;

    //�q�b�g���̏���
    void Hit(void);

    //���f��ID�̎擾
    virtual const int& GetModelId(void)const override;

private:

    enum class SABER_STATE
    {
        SLASH,
        HIT
    };

    Transform& transform_;
};

