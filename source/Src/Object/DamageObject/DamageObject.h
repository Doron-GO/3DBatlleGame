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
	//描画
	virtual void Draw(void)=0;
	//更新
	virtual void Update(VECTOR enemyPos)=0;
	//有効化
	virtual void Activate(void);
	//非有効化
	virtual void InActivate(void);

	//座標の設定
	const void SetPos(VECTOR pos);
	//プレイヤータイプの取得
	virtual int GetPlayerType(void)const;
	//ダメージ量の取得
	virtual const float& GetDamage(void)const;
	//モデルIDの取得
	virtual const int& GetModelId(void)const;
	//有効状態かどうかを取得
	virtual bool IsActive(void);

protected:

	//ヒットエフェクトの大きさ
	static constexpr VECTOR HIT_EFFECT_SCARE = { 35.0f,35.0f,35.0f };

	//リソースマネージャー
	ResourceManager& resMng_;

	//トランスフォーム
	Transform transform_;

	//有効状態化を判定
	bool activeFlag_ =false;

	//エフェクト再生マネージャー
	std::unique_ptr<EffectManager> effectManager_;

	//ダメージ量
	float damage_;
	//クールタイム
	float coolTime_;

	//デルタタイム
	float deltaTime_;

	//モデルID
	int modelId_;

	//プレイヤータイプ
	int playerType_;

private:

};

