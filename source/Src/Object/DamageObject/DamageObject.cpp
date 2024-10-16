#include "DamageObject.h"
#include"../../Manager/ResourceManager.h"

DamageObject::DamageObject(int playernum):resMng_(ResourceManager::GetInstance())
{
    //��L����
    isActive_ = false;
    //�_���[�W�ʂ̏�����
    damage_ = 0.0f;
    //�N�[���^�C���̏�����
    coolTime_ = 0.0f;
    //�f���^�^�C���̏�����
    deltaTime_ = 0.0F;
    //�v���C���[�^�C�v�̐ݒ�
    playerType_ = playernum;
}

void DamageObject::Activate(void)
{
    //�L����
    isActive_ = true;
}

bool DamageObject::IsActive(void)
{
    return isActive_;
}

const void DamageObject::SetPos(VECTOR pos)
{
   transform_.pos = pos;
}


 int DamageObject::GetPlayerType(void) const
{
    return playerType_;
}

void DamageObject::InActivate(void)
{
    isActive_ = false;
}

const float& DamageObject::GetDamage(void) const
{
    return damage_;
}


const int& DamageObject::GetModelId(void) const
{
    return modelId_;
}
