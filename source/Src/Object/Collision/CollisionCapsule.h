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
	// �J�v�Z���̒��S���W
	VECTOR GetCenter(void) const;


	bool Collision(int modelId);

private:

	Transform& transform_;

	// ���Έʒu(�㑤)
	VECTOR relativePosTop_;

	// ���Έʒu(����)
	VECTOR relativePosDown_;

	//����W
	float top_;

	//�����W
	float down_;

	//���a
	float r_;

	//�J�v�Z���𓯊�������
	void Sync();

};

