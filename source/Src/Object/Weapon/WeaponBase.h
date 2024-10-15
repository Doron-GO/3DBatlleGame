#pragma once
#include"../Common/Transform.h"
#include"../../Manager/ResourceManager.h"

class Player;

class WeaponBase
{
public:

	//モデルの右手フレーム番号
	static constexpr int ATTACH_RIGHT_HAND_FRAME = 93;
	
	//モデルの左手手フレーム番号
	static constexpr int ATTACH_LEFT_HAND = 71;

	WeaponBase(int playerType, const Transform& playerTransform);
	virtual ~WeaponBase(void) {};

	//更新
	virtual void Update(void) = 0;

	//描画
	virtual void Draw(void)=0;

	//有効化する
	virtual void Activate(void);

	//非有効化する
	virtual void InActivate(void);

	//ロボットモデルと座標、回転を同期
	virtual void SyncPosition(void);

protected:

	// シングルトン参照
	ResourceManager& resMng_;

	//トランスフォーム
	Transform transform_;

	//プレイヤーのトランスフォーム
	const Transform& playerTransform_;

	//プレイヤーのタイプ
	int playerType_;

	//有効かどうかを判定
	bool activeFlag_;

};

