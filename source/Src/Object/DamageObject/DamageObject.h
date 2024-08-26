#pragma once
#include"../../Manager/ResourceManager.h"
#include"../../Manager/EffectManager.h"
#include"../Comon/Transform.h"
#include<memory>

class ResourceManager;

class DamageObject
{
public:

	enum class DMG_OBJ_TYPE
	{
		NON,
		BEAM,
		STRONG_BEAM,
		SABEL
	};

	DamageObject(int playernum);
	virtual void Draw(void)=0;
	virtual void Update(VECTOR enemyPos)=0;
	virtual void Activate(void);
	virtual bool IsActive(void);
	const void SetPos(VECTOR pos);
	virtual int GetPlayerNum(void)const;
	virtual void InActivate(void);
	virtual const float& GetDamage(void)const;
	virtual const int& GetModelId(void)const;
protected:

	static constexpr VECTOR HIT_EFFECT_SCARE = { 35.0f,35.0f,35.0f };

	ResourceManager& resMng_;
	DMG_OBJ_TYPE dmgObjType_;
	Transform transform_;

	bool activeFlag_ =false;
	//エフェクト再生
	std::unique_ptr<EffectManager> effectManager_;
	float damage_;
	float coolTime_;
	float deltaTime_;

	int modelId_;

	int playerNum_;

private:



};

