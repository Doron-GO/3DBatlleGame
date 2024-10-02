#include "Stage.h"
#include<DxLib.h>
#include"../../Manager/ResourceManager.h"

Stage::Stage():resMng_(ResourceManager::GetInstance())
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	//大きさ
	transform_.scl = {5.0f,1.0f,5.0f};
	//座標
	transform_.pos = { 0.0f, -30.0f, 0.0f };
	//回転
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });
	transform_.Update();
	//コリジョンの作成
	transform_.MakeCollider(Collider::TYPE::STAGE);
}

Stage::~Stage()
{
}

void Stage::Draw()
{
	//描画
	MV1DrawModel(transform_.modelId);
}

Collider* Stage::GetCollider()
{
	return transform_.collider;
}
