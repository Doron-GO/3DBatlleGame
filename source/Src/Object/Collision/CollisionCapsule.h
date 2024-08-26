#pragma once
#include<DxLib.h>
#include"CollisionBase.h"

class Transform;

class CollisionCapsule:
public CollisionBase
{
public:
	CollisionCapsule(Transform& transform,float top,float Down, float r);

	// �`��
	void Draw(void);

	void Update(void);

	// �eTransform����̑��Έʒu���Z�b�g
	void SetRelativePosTop(const VECTOR& pos);
	void SetRelativePosDown(const VECTOR& pos);

	VECTOR GetPosTop(void) const;
	VECTOR GetPosDown(void) const;
	//VECTOR GetRelativePos(const VECTOR& pos) const;

	//// ���a
	//float GetRadius(void) const;
	//void SetRadius(float radius);

	//// ����
	//float GetHeight(void) const;

	// �J�v�Z���̒��S���W
	VECTOR GetCenter(void) const;

	//bool IsCollision(void)const;

	bool Collision(int modelId);

private:

	Transform& transform_;

	// ���Έʒu(�㑤)
	VECTOR relativePosTop_;

	// ���Έʒu(����)
	VECTOR relativePosDown_;

	Quaternion quaRot_;	

	float top_;

	float down_;

	float r_;
	
	void Sync();

};

