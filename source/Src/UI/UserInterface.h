#pragma once
#include"../Input/Input.h"
#include<map>

class ResourceManager;

class UserInterface
{
public:

	UserInterface(
		int playMode,
		int playerType,
		const float& distance,
		const int& bulletNum,
		const float& boostGauge, 
		const float& hpGauge, 
		const bool& isWin,
		const VECTOR& enemyPos,
		const float& enemyHp);

	~UserInterface(void);

	void Draw(bool& gameSet);

	//����UI(�Q�[���X�^�[�g��)
	void DrawCommonUI(const float& startCount, const bool& isGameSet, const int& rematchMode);

private:

	static constexpr  int MAX_HP_IMAGE_NUM = 10;

	//�摜�^�C�v
	enum class IMG_TYPE
	{
		BOOST_GAUGE,
		BOOST_GAUGE_CASE,
		ENEMY_HP_GAUGE,
		WIN,
		LOSE,
		TARGERT,
		TARGET_RED,
		RIFLE_IMAGE,
		ONE_MOR_FIGHT,
		BACK_TO_TITLE,
		PLEASE_CROSS,
		PLEASE_A,
		TRIANGLE,
		READY,
		FIGHT,
		HP_BACK,
		BULLET_NUM

	};

	struct Info
	{
		VECTOR pos_;
		double scale_;
		int centerX_;
		int centerY_;
		float offset_;
	};

	//�摜�̃p�����[�^�̔z��
	std::map<IMG_TYPE, Info> imgInfo_;

	// �V���O���g���Q��
	ResourceManager& resMng_;

	//UI�摜�n���h��
	std::map<IMG_TYPE, int> uiImgH_;

	//�����摜
	int* numberImg_;

	//�u�[�X�g�Q�[�W�̐��l
	const float& boostGauge_;
	//HP�̐��l
	const float& hpGauge_;

	//HP�����̑傫��
	float hpValueScale_;
	//HP�����̃I�t�Z�b�g
	int hpValueOffset_;

	//�GHP�̐��l
 	const float& enemyHpGauge_;

	//�c�e��
	const int * numnberofBullets_;

	//�G�Ƃ̋���
	const float& enemyDistance_;

	//�v���C���[�h
	int playMode_;

	//�v���C���[�������������s�k�������̔���
	const bool& isWin_;

	//�v���C���[�P�̃p�b�h�̎��
	JOYPAD_TYPE joyPadType_;

	//�Đ탂�[�h
	int rematchMode_;

	//�G�̍��W
	const VECTOR& enemyPos_;

	//�X�^�[�g�v���ϐ�
	float startCount_;

	//�摜�̓ǂݍ���
	void InitImage(void);

	//�Q�[���X�^�[�g�\���Ȃǂ̋���UI�̏�����
	void InitCommonUI(void);

	//�u�[�X�g�Q�[�W�̕\��
	void DrawBoostGauge(void);

	//�c�e���̕\��
	void DrawNumnberOfBullets(void);

	//HP�Q�[�W�̕\��
	void DrawHPGauge(void);

	//WIN��������LOSE��\������
	void VictoryOrDefeat(void);

	//�G�ւ̃^�[�Q�b�g�\��
	void DrawTarget(void);

	//�G��HP�\��
	void DrawEnemyHp(void);

	//�Q�[���X�^�[�g���̕\��
	void DrawGameUIStart(const float& startCount);
	
	//�Q�[���I�����̕\��
	void DrawUIGameSet(const bool& isGameSet, const int& rematchMode);

};

