#pragma once
#include"../../../Manager/RobotAnimeController.h"
#include"../../../Manager/EffectManager.h"
#include"../../../UI/UserInterface.h"
#include"../../Weapon/BeamRifle.h"
#include"../../Weapon/BeamSaber.h"
#include"../../State/StateBase.h"
#include"../../Comon/Transform.h"
#include"../../../Input/Input.h"
#include"../../Camera/Camera.h"
#include"../ActorBase.h"
#include<string>


class ResourceManager;
class BeamRifle;
class BeamSaber;
class Camera;

class Player:
	public ActorBase

{

public:

	//�W�����v���̑O�㍶�E�ւ̍ő�ړ��X�s�[�h
	static constexpr float MAX_JUMP_MOVE_SPEED = 20.0f;

	//�����G����
	static constexpr float BIG_SAFE_TIME = 100.0f;
	//�����G����
	static constexpr float SMALL_SAFE_TIME = 70.0f;


	//�W�����v����Ƃ��̍ŏ��̌�����
	static constexpr float JUMP_FAST_RATE = 10.0f; 
	//�u�[�X�g�Q�[�W����ʒ萔
	static constexpr float BOOST_RATE = 20.0f; 
	static constexpr float BOOST_DASH_MOVE_SPEED = 50.0f;
	static constexpr int	MIN_JUMP_BOOST = 1; //�W�����v�ɕK�v�ȃu�[�X�g�Q�[�W�̗�
	static constexpr float BOOST_MOVE_SPEED = 60.0f;
	static constexpr float MOVE_SPEED = 20.0f;
	static constexpr float MIN_BOOST = 20.0f; //�W�����v�ɕK�v�ȃu�[�X�g�Q�[�W�̗�
	static constexpr float FALL_MAX_MOVE_SPEED = 35.0f;//FALL��Ԏ��ړ��X�s�[�h


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
		COMBAT1,
		COMBAT_RUN,
		WIN,
		LOSE,

	};	
	
	Player(int playerNum, int playMode);

	~Player();
	void Init(void);

	void Update();

	void Draw(void);

	void PlayEffect(STATE state);

	Camera& GetCamera()const;

	BeamRifle &GetBeamRifle(void)const;

	BeamSaber&GetBeamSaber(void)const;

	const Transform& GetTransform(void) const;

	int GetPlayerNum(void)const;

	float GetDeltaTime(void)const;

	void GetMoveDir(void);

	void GetMoveDirBoostDash(void);

	const bool IsGround(void) const;

	const bool& IsHorming(void) const;

	const VECTOR GetEnemyPos(void) const;

	const VECTOR& GetPlayerPos(void) const;

	const float& GetOffsetBossEnemy(void)const;
	
	void CameraDraw();

	//�n�ʂƂ̓����蔻��
	void CollisionGravity(void);

	//�_���[�W�̂Ȃ����̂Ƃ̓����蔻��ǉ��p
	void AddCollider(Collider* collider);

	//����̍��W
	void SetEnemyPosition(const VECTOR* enemyPos);

	//�����Hp
	void SetEnemyHp(const float* enemyHp);
	
	//�G�̃X�e�[�g����ݒ�
	void SetEnemyState(const STATE* enemyState);

	//moveSpeed_�̒l���Z�b�g����֐�
	void SetMoveSpeed(float value);

	//maxMoveSpeed_�̒l���Z�b�g����֐�
	void SetMaxMoveSpeed(float value);

	//maxMoveSpeed_�̒l���Z�b�g����֐�
	void SetGravityPow(float value);

	//maxMoveSpeed_�̒l���Z�b�g����֐�
	void SetJumpSpeed(float value);

	//maxMoveSpeed_�̒l���Z�b�g����֐�
	void SetJumpPowY(float value);

	//�u-�X�g�_�b�V�����I���Ƃ��ɕK���Ă�
	void BoostDashEnd(void);
	
	//�v���C���[�̏�Ԃ��ւ񂱂�����
	void ChangeState(std::unique_ptr<StateBase> state_);

	//�R���g���[���[���͏��̎擾
	const Input& GetInput() const;

	//�ړ�����֐�

	void Move(void);

	//�ړ�����֐�
	void MoveBoodtDash(void);

	//�W�����v����֐�
	void Jump(void);

	//�ˌ�����
	void Shot(void);

	//�_���[�W���炢��Ԃɕς���
	void ChangeDamageState(void);

	//�i���_���[�W���炢��Ԃɕς���
	void ChangeDwonState(void);

	//�u�[�X�g�Q�[�W�����ʒ����Ă��邩�ǂ���
	 bool IsBoostGaugeSufficient(float RequiredGaugeAmount);

	//���G���Ԃ��I��������ǂ���
	//<returns>true:�I����Ă��� false:�I����Ă��Ȃ�</returns>
	 bool IsSafeTimeSufficient(void);

	//�_���[�W���󂯂����◧���~�܂�Ƃ��Ɉړ��ʂ��O�ɂ���
	void MoveStop(void);

	//jumpPow_��0�ɂ���
	void JumpStop(void);

	//JumpPow�����X�Ɍ��炵�Ă���
	void JumpPowZero(void);


	//�d��_��0�ɂ���
	void GravityZero(void);

	//�u�[�X�g�_�b�V��������
	void Boost(void);

	//���G���Ԍv��(�����̐��l��safeTime_�����Z���Ă���)
	void CountSafeTime( float value);

	//���G���Ԃ����Z�b�g����
	void SetSafeTime(float value);

	//�u�[�X�g�Q�[�W�񕜊J�n�v�������Z�b�g����
	void RechargeBoostCountReset(void);

	//���n�d�����Ԃ��I��������ǂ�����Ԃ�
	const bool LandingStunEnded(void)const;

	//�U���d�����Ԃ��I��������ǂ�����Ԃ�
	const bool CombatStunEnded(void)const;

	//���ׂĂ̍d�����Ԃ��I����Ă�����true
	const bool AllStanEnded(void)const;

	//�U���d�����Ԑݒ�
	void SetCombatStan(float stanTime);

	//�d�͂����Z����
	void CalcGravity();

	void ResetShotFlame(void);

	//�A�j���[�V�����R���g���[���[�̃p�����[�^��\��(�f�o�b�O�p)
	void RobotAnimDebugDraw(int playerType);

	//�v���C���[�̊e��p�����[�^��\��(�f�o�b�O�p)
	void PlayerDebugDraw(int playerType);

	//�i���U����ԂɈڍs������֐�
	void Combat(void);

	//�u�[�X�g�_�b�V����������������߂�֐�
	void BoostQuaRot(void);

	//�u�[�X�g�Q�[�W�������֐�
	void ConsumeBoostGauge(float rate);

	//�u�[�X�g�Q�[�W�̉񕜂��~������
	void StopRechargeBoost(void);

	bool IsDead(void);

	void Lose(void);

	void Win(void);

	const STATE& GetState(void);

	//�X�[�p�[�A�[�}�[�����
	void DamageSuperArmor(void);

	//�X�[�p�[�A�[�}�[���c���Ă��邩�ǂ����𔻒�
	bool IsSuperArmor(void);

	 std::unique_ptr<UserInterface> MoveUI(void);

	//��]�̐ݒ�p�ϐ�
	Quaternion quaRot_;

	//�v���C���[�̌��݂̏��
	STATE pState_;

private:

	enum class PLAYER_NUM
	{
		PLAYER_1, 
		PLAYER_2
	};

	enum class PLAY_MODE
	{
		SINGLE_MODE,
		BATTLE_MODE
	};


	bool isHorming_;

	//�G�̃X�e�[�g���
	const STATE* enemyState_;

	PLAY_MODE playMode_;

	//UI
	std::unique_ptr<UserInterface> userInterface_;

	//�G�t�F�N�g�Đ�
	std::unique_ptr<EffectManager> effectManager_;

	//
	Transform transform_;
	//�r�[�����C�t��
	std::unique_ptr<BeamRifle> beamRifle_;	

	//�r�[���T�[�x��
	std::unique_ptr<BeamSaber> beamSaber_;	

	//��Ԃ̊Ǘ�
	std::unique_ptr<StateBase> state_;

	//�R���g���[������
	const std::unique_ptr<Input>input_;	

	//�J����
	const std::unique_ptr<Camera> camera_;	

	//�ړ�����
	Quaternion moveQua_;

	//�ړ�����
	Quaternion moveBoostQua_;

	//�ړ���̍��W
	VECTOR movedPos_;

	// �ړ���
	VECTOR movePow_;

	//�ړ�����X�s�[�h
	float moveSpeed_;

	//�ړ�����ő�X�s�[�h
	float maxMoveSpeed_;

	//�W�����v�̗�
	VECTOR jumpPow_;

	//�W�����v�̃X�s�[�h
	float jumpSpeed_;

	std::string debugString_;
	std::string enemyDebugString_;

	//���X�e�B�b�N���͂��v���C���[�̈ړ������ƑΉ����������
	VECTOR padDir_;

	VECTOR padDirBoost_;

	float padX_,padY_;

	//�X�e�[�W�Ȃǂ̏�Q�������蔻��p
	std::vector<Collider*>colliders_;

	//�������v���C���[1�Ȃ̂��Q���̔��f
	int playerNum_;	
	//�������p�b�h1�Ȃ̂��Q���̔��f
	int padNum_;	

	//���ˌ��ł���܂ł̎���
	float shotFlame_;
	//�ˌ��\�t���O
	bool shotFlag_;	

	//�G�Ǝ����Ƃ̊p�x
	float angle_;

	float  offsetEnemy_; 

	//��������G�ւ̃x�N�g��

	VECTOR enemyVec_;

	//�t�H�[�����n�ʒ��n�t���O
	bool groundedFlag_;

	//�㉺���g�̉�]�����ɖ߂��t���O
	float revertFlag_;

	//���B����ׂ���]
	Quaternion goalUpperQuaRotY_;

	//�n�܂�̉�]
	Quaternion startUpperQuaRotY_;

	//�d�͂̌���
	VECTOR dirGravity_;

	//�d�͂̋t����
	VECTOR dirUpGravity_;

	//���������蔻��p
	float jumpDot_;	

	//�d�͂̋���
	float gravityPow_;

	//�u�[�X�g�Q�[�W
	float boostGauge_;

	//�u�[�X�g�Q�[�W�񕜊J�n���� true:�񕜒��@false:�񕜒�~
	bool rechargeBoostFlag_;

	//�u�[�X�g�Q�[�W���񕜂��n�߂�܂ł̎���
	float rechargeBoostCount_;

	//�v���C���[���G����
	float safeTime_;

	//���n�d������
	float landingStanTime_;

	//�ߐڍU���d������
	float combatStanTime_;

	//�z�[�~���O�e�����p�̃e�X�g�ϐ�
	const VECTOR* enemyPos_;

	//�G�Ƃ̋���
	float enemyDistance_;

	//�Ă�HP
	const float* enemyHp_;

	bool isWin_;

	//�i���ڍs��Ԏ��Ɏ󂯎~�߂���e�̐�
	int superArmor_;

	/// �Q�[�W���C�ӂ̐��l�ȏォ�ǂ����𔻒肷��
	/// <returns>true:�ȏ�ł��� false:�����ł���</returns>
	const bool IsGaugeSufficient(float Gauge, float RequiredGaugeAmount)const;


	//�v���C���[�̏�Ԃɂ���Ĉړ��X�s�[�h����ς���
	void DebugPlayerState(void);

	//�v���C���[�̏�Ԃɂ���Ĉړ��X�s�[�h����ς���
	void EnemyState(void);

	//�ő�
	void SpeedAdd(void);

	void CalculateAngleToTarget();

	//�㔼�g�̉�]�����ɖ߂��֐�
	void RevertRotate();

	//�u�[�X�g�Q�[�W���񕜂���
	void RecoverBoostGauge(void);

	void AddToGauge(float& gauge, float add);

	void ConsumeGauge(float& gauge, float rate);

	void InitValue(float& target);

	//�s���s�\���Ԍv��
	void CountLandingStanTime(void);

	//�s���s�\���Ԍv��
	void CountCombatStanTime(void);

	//�X�e�B�b�N���͂�����Ă���Ƃ��Ɉړ�������ݒ肷��
	void SetMoveQuaRot(void);

	//�X�e�B�b�N���͂�����Ă���Ƃ��Ɉړ�������ݒ肷��
	void SetBoostQuaRot(void);

	//�X�e�B�b�N���͂�����Ă��Ȃ��Ƃ���speed�����X�Ɍ��炵�Ă���
	void MoveSpeedZero(void);


	bool StickAdjustment(VECTOR v1, VECTOR v2);

	//��]�A���W�Ȃǂ��X�V����
	//transform_.pos �� transform_.quaRot
	void TransformUpdate(void);

	void WeaponUpdate(void);

	//����Ƃ̋����𑪂�֐�
	void RangeDistance(void);

	//����Ƃ̋����ɂ���Ēe�������Ǐ]���邩�ǂ����𔻒肷��
	void Range(void);

	void MakeObjects(void); 

	void InitTransform(void);

	void InitParameter(void);

	virtual void InitAnimation(void) override;

	void InitCamera(void);

	void UpdateBattleMode(void);

	void UpdateSIngleMode(void);

};	


