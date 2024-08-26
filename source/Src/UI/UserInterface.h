#pragma once
#include<map>

class ResourceManager;

class UserInterface
{
public:

	UserInterface(ResourceManager& resMng, const VECTOR* enemyPos,float& distance,
		float& boostGauge, float& hpGauge, const float& enemyHp, bool& IsWin, const int& numnberofBullets,int SinglPlay);

	~UserInterface();
	void Update(void);

	void Draw(bool& gameSet);

private:

	enum class IMG_H
	{
		BOOST_GAUGE,
		BOOST_GAUGE_CASE,
		ENEMY_HP_GAUGE,
		WIN,
		LOSE,
		TARGERT,
		TARGET_RED,
		RIFLE_IMAGE
	};

	// �V���O���g���Q��
	ResourceManager& resMng_;

	//UI�摜�n���h��
	std::map<IMG_H,int> uiImgH_;

	//�u�[�X�g�Q�[�W�̐��l
	float& boostGauge_;

	//HP�̐��l
	float& hpGauge_;

	//�GHP�̐��l
 	const float& enemyHpGauge_;

 	 float enemyHpGaugeCause_;

	//�c�e��
	const int& numnberofBullets_;

	//�G�Ƃ̋���
	float& enemyDistance_;

	int singlePlay_;

	//Hp�\��
	int fontHp_;

	//�c�e���\���\��
	int fontBullets_;

	//�v���C���[�������������s�k�������̔���
	bool& IsWin_;

	//�����A�s�k�摜�̍��W
	VECTOR victoryOrDefeatPos_;

	//�u�[�X�g�Q�[�W�摜�̍��W
	VECTOR boostGaugePos_;

	int boostGaugeOffsetX_;
	int boostGaugeOffsetY_;

	//�u�[�X�g�Q�[�W�摜�̍��W
	double boostGaugeCaseScale_;

	//HP�̍��W
	VECTOR HpGaugePos_;

	//�c�e�\���摜�̍��W
	VECTOR numberOfBulletsPos_;

	//�c�e�\���摜�̍��W
	double numberOfBulletsScale_;


	//�G�̍��W
	const VECTOR* enemyPos_;

	void InitImage(void);

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

};

