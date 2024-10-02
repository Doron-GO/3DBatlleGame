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

	// FIXED_POINT : �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 800.0f, 600.0f, 800.0f };

	// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr float SYNC_PLATER_POS_OFFSET =300.0f;

	// FOLLOW : �����_(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 100.0f, 500.0f };

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);
	
	// �Ǐ]�ΏۂƂ̒������W
	static constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };
	
	// �J������]�X�s�[�h
	static constexpr float SPEED = 2.0f;

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

	//�m�F�p�`��
	void DrawDebug(void);

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

