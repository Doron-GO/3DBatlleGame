#pragma once
#include<DxLib.h>
#include"CollisionBase.h"

class Transform;

class CollisionCapsule:
public CollisionBase
{
public:
	CollisionCapsule(Transform& transform,float top,float Down, float r);

	// 描画
	void Draw(void);

	void Update(void);

	// 親Transformからの相対位置をセット
	void SetRelativePosTop(const VECTOR& pos);
	void SetRelativePosDown(const VECTOR& pos);

	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;
	// カプセルの中心座標
	VECTOR GetCenter(void) const;


	bool Collision(int modelId);

private:

	Transform& transform_;

	// 相対位置(上側)
	VECTOR relativePosTop_;

	// 相対位置(下側)
	VECTOR relativePosDown_;

	//上座標
	float top_;

	//下座標
	float down_;

	//半径
	float r_;

	//カプセルを同期させる
	void Sync();

};

