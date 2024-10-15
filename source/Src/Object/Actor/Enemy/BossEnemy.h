#pragma once
#include"../../Common/Transform.h"
#include"../ActorBase.h"

class ResourceManager;
class DeathBall;
class RobotAnimeController;
class StateBase;
class EffectManager;
class DamageObject;

class BossEnemy:
	public ActorBase
{
public:

	enum class BOSS_STATE
	{
		NONE,
		WAIT,
		SHOOTING,
		DOWN,
		DEAD

	};

	enum class SP_MOVE
	{
		NONE,
		DEATH_BALL
	};

	BossEnemy(int playerType);
	~BossEnemy(void);

	//�A�b�v�f�[�g
	void Update(void);

	//�`��
	void Draw(void);

	//���W���擾
	const VECTOR& GetBossEnemyPos(void) const;

	//�v���C���[�̏�Ԃ��ւ񂱂�����
	void ChangeState(std::unique_ptr<StateBase> state);

	//�A�j���[�V�����̏�����
	virtual void InitAnimation(void)override;


	//�U�����s��
	void ActiveSpMove(SP_MOVE spMove);

	//�U�����I����Ă��邩�ǂ����𔻒�
	const bool& IsActiveSpMove(SP_MOVE spMove);

	//�U���̔z����擾
	const std::map<SP_MOVE, std::unique_ptr<DamageObject>>& GetSpMove()const;

	//�U���I�u�W�F�N�g�̐���
	void MakeSpMoveObjects(void);

	//���S��ԂɈڍs
	void ChangeDeathState(void);

	//�G�t�F�N�g�̍Đ�
	void PlayEffect(BOSS_STATE state);

	virtual const STATE& GetState(void) override;


private:

	//�U���I�u�W�F�N�g�z��
	std::map<SP_MOVE, std::unique_ptr<DamageObject>> spMoves_;

	//�G�t�F�N�g�Đ�
	std::unique_ptr<EffectManager> effectManager_;

	//��Ԃ̊Ǘ�
	std::unique_ptr<StateBase> state_;

	//��Ԃ̊Ǘ�
	std::unique_ptr<DeathBall> deathBall_;


};

