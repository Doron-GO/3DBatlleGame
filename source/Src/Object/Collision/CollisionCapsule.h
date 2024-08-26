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
	//VECTOR GetRelativePos(const VECTOR& pos) const;

	//// 半径
	//float GetRadius(void) const;
	//void SetRadius(float radius);

	//// 高さ
	//float GetHeight(void) const;

	// カプセルの中心座標
	VECTOR GetCenter(void) const;

	//bool IsCollision(void)const;

	bool Collision(int modelId);

private:

	Transform& transform_;

	// 相対位置(上側)
	VECTOR relativePosTop_;

	// 相対位置(下側)
	VECTOR relativePosDown_;

	Quaternion quaRot_;	

	float top_;

	float down_;

	float r_;
	
	void Sync();

};

