#pragma once
#include "../../Common/Quaternion.h"
#include <vector>
#include <DxLib.h>

class SceneManager;
class Transform;
class Collider;
class Controller;

class Camera
{

public:





	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW
	};

	Camera();
	~Camera();

	//�����ݒ�
	void Init(void);

	//�J�����̍X�V����
	void SetBeforeDraw(void);

	//��_�J����
	void SetBeforeDrawFixedPoint(void);

	//�Ǐ]�J����
	void SetBeforeDrawFollow(void);

	//�g�����X�t�H�[���̐ݒ�
	void SetTransform(const Transform* transform);

	//��]���擾
	Quaternion GetQuaRot(void) const;

	//X��]�𖳂�������]���擾
	Quaternion GetQuaRotOutX(void) const;

	//�Ǐ]�Ώۍ��W�̐ݒ�
	void SetTargetPos(VECTOR pos);
	//���W�̐ݒ�
	void SetCameraPos(VECTOR pos);

	//���[�h�̕ύX
	void ChangeMode(MODE mode);

private:

	//�J�����̏����ݒ�
	void SetDefault(void);

	// Transform�̈ʒu�Ɠ��������
	void SyncTransform(void);
	// Transform�̈ʒu�Ɠ��������
	void SyncEnemyTransform(void);

	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* syncTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR cameraPos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion quaRotOutX_;

	// �J�����p�x
	Quaternion quaRot_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	//�G�̈ʒu
	VECTOR enemyPos_;
 
};

