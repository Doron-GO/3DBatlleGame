#pragma once
#include"../Comon/Transform.h"
#include"../../Manager/ResourceManager.h"

class Player;

class WeaponBase
{
public:

	static constexpr int ATTACH_RIGHT_HAND_FRAME = 93;
	static constexpr int ATTACH_LEFT_HAND = 71;

	WeaponBase(int playerType, const Transform& playerTransform);
	virtual ~WeaponBase(void) {};
	virtual void Update(void) = 0;
	virtual void Draw(void)=0;
	virtual void Activate(void);
	virtual void InActivate(void);

	virtual void SyncPosition();
protected:
	// シングルトン参照
	ResourceManager& resMng_;
	Transform transform_;
	const Transform& playerTransform_;
	int playerType_;
	bool activeFlag_;
private:


};

