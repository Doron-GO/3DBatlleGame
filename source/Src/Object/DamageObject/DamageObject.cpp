#include "DamageObject.h"
#include"../../Manager/ResourceManager.h"

DamageObject::DamageObject(int playernum):resMng_(ResourceManager::GetInstance())
{
    dmgObjType_ = DMG_OBJ_TYPE::NON;
    activeFlag_ = false;
    damage_ = 0.0f;
    coolTime_ = 0.0f;
    deltaTime_ = 0.0F;
    playerNum_ = playernum;
}

void DamageObject::Activate(void)
{
    activeFlag_ = true;
}

bool DamageObject::IsActive(void)
{
    return activeFlag_;
}

const void DamageObject::SetPos(VECTOR pos)
{
   transform_.pos = pos;
}


 int DamageObject::GetPlayerNum(void) const
{
    return playerNum_;
}

void DamageObject::InActivate(void)
{
    activeFlag_ = false;
}

const float& DamageObject::GetDamage(void) const
{
    return damage_;
}


const int& DamageObject::GetModelId(void) const
{
    return modelId_;
}
