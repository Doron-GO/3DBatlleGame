#include "DamageObject.h"
#include"../../Manager/ResourceManager.h"

DamageObject::DamageObject(int playernum):resMng_(ResourceManager::GetInstance())
{
    //非有効化
    isActive_ = false;
    //ダメージ量の初期化
    damage_ = 0.0f;
    //クールタイムの初期化
    coolTime_ = 0.0f;
    //デルタタイムの初期化
    deltaTime_ = 0.0F;
    //プレイヤータイプの設定
    playerType_ = playernum;
}

void DamageObject::Activate(void)
{
    //有効化
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
