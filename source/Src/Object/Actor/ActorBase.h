#pragma once
#include"../Collision/CollisionCapsule.h"
#include<memory>

class RobotAnimeController;
class ResourceManager;

class ActorBase
{
public:

	enum class STATE
	{
		NONE,
		RUN,
		JUMP,
		DAMAGE,
		DOWN,
		SHOT,
		IDLE,
		BOOST,
		BOOST_DASH,
		FALL,
		COMBAT,
		COMBAT_RUN,
		WIN,
		LOSE,

	};

	enum class ACTOR_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		BOSS,
		NON
	};


	ActorBase();

	//���f���̑S�g�̃A�j���[�V�����̍Đ����s��
	virtual void PlayAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//���f���̏㔼�g�̃A�j���[�V�����̍Đ����s��
	virtual void PlayUpperAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//���f���̉����g�̃A�j���[�V�����̍Đ����s��
	virtual void PlayLowerAnim(int state, bool priority, bool isLoop, bool isStop = false, float endStep = -1.0f, float startStep = 0.0f);

	//�A�j���[�V�����Đ����I����Ă�����
	const bool IsAnimEnded(void)const;

	//j������HP���擾
	const float& GetHP(void)const;

	//�����̐��l��HP�����炷
	void Damage(float damage);

	//�g�����X�t�H�[��
	const Transform& GetTransform(void)const;

	//�J�v�Z�����擾
	CollisionCapsule& GetCapsule(void)const;

	virtual const STATE& GetState(void);

	//�r�[�����C�t���̒e��
	const int& GetNumnberOfBullets(void);

	//�u�[�X�g�Q�[�W
	const float& GetBoostFuel(void) const;

	//�G�Ƃ̋���
	const float& GetEnemyDistance(void) const;

	//�����������ǂ����𔻒�
	const bool& IsWin(void) const;

	//�G�̍��W��ݒ�
	 void SetEnemyPosition(const VECTOR* enemyPos);

	 //�G��HP��ݒ�
	 void SetEnemyHp(const float* enemyHp);

	 //�G�̃X�e�[�g��ݒ�
	 void SetEnemyState(const STATE* enemyState);

	 //�A�N�^�[�^�C�v���擾
	 const ACTOR_TYPE& GetActorType(void);

protected:

	// �V���O���g���Q��
	ResourceManager& resMng_;

	//�g�����X�t�H�[��
	Transform transform_;

	//�A�j���[�V�����Ǘ�
	std::unique_ptr<RobotAnimeController> robotAnimeController_;

	//�J�v�Z����̓����蔻��p
	std::unique_ptr<CollisionCapsule> capsule_;

	//�G�̍��W
	const VECTOR *enemyPos_;

	//�GHP
	const float* enemyHp_;

	//�G�̃X�e�[�g���
	const STATE* enemyState_;

	//�f���^�^�C��
	float deltaTime_;

	//����HP
	float hp_;

	//�u�[�X�g�c��
	float boostFuel_;

	//�G�Ƃ̋���
	float enemyDistance_;

	//�G�Ƃ̋���
	int numberofBullets_;

	//�����������ǂ�������
	bool isWin_;

	//�X�e�[�g
	STATE actorState_;

	//
	ACTOR_TYPE actorType_;

	//�A�j���[�V�����̐ݒ�
	virtual void InitAnimation(void);

};
