#pragma once
#include"../Common/Transform.h"
#include"../../Manager/ResourceManager.h"

class Player;

class WeaponBase
{
public:

	//右手親指フレーム番号
	static constexpr int RIGHT_HAND_THUMB_FRAME = 96;

	//モデルの左手手フレーム番号
	static constexpr int ATTACH_LEFT_HAND = 71;

	//モデルの右手フレーム番号
	static constexpr int RIGHT_HAND_FRAME = 93;

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

	void AddCollider(Collider* collider);

	const Transform& GetTransform(void) const;


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
	bool isActive_;

	//ステージなどの障害物当たり判定用
	std::vector<Collider*>colliders_;


};

