#pragma once
#include <vector>
#include <DxLib.h>
#include "../../Common/Quaternion.h"

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

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void Draw(void);

	void SetTransform(const Transform* transform);
	void SetEnemyTransform(const Transform* enemyTransform);

	VECTOR GetPos(void) const;
	VECTOR GetAngles(void) const;
	VECTOR GetTargetPos(void) const;
	Quaternion GetQuaRot(void) const;
	Quaternion GetQuaRotOutX(void) const;
	VECTOR GetDir(void) const;
	VECTOR GetDirvec(VECTOR vec) ;
	void SetTargetPos(VECTOR pos);
	void SetCameraPos(VECTOR pos);

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

	const Transform* enemyTransform_;

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

