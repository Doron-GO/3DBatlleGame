#include<string>
#include<DxLib.h>
#include"../../../../Config.h"
#include"../../../Utility/AsoUtility.h"
#include"../../../Manager/ResourceManager.h"
#include"../../Time/DeltaTime.h"
#include"../../State/PlayerState/CombatTransitionState.h"
#include"../../State/PlayerState/DamageState.h"
#include"../../State/PlayerState/WinnerState.h"
#include"../../State/PlayerState/IdleState.h"
#include"../../State/PlayerState/FallState.h"
#include"../../State/PlayerState/DownState.h"
#include"../../State/PlayerState/LoseState.h"
#include"../../DamageObject/BeamShot.h"
#include "Player.h"

#pragma region Parameter

//�f���^�^�C���ɂ�����萔(60.0f)
constexpr float DEFAULT_RATE = 60.0f;

 //�v���C���[�̏����q�b�g�|�C���g
constexpr float MAX_PLAYER_HP = 300.0f;

//�v���C���[�̏��������i��]�j
constexpr VECTOR PLAYER_INIT_ROT = { 0.0f,180 * DX_PI_F / 180 ,0.0f };
constexpr float GRAVITY_POW = 30.0f;
//�d�̓f���^�^�C���ɂ�����萔
constexpr float GRAVITY_RATE = 100.0f;
 //�d�͂̍ő�l
constexpr float MAX_GRAVITY_POW = 30.0f;

//�u�[�X�g�Q�[�W�̍ő��
constexpr float MAX_BOOST_GAGE = 100.0f;
//�����d�͒l
constexpr float DEFAULT_GRAVITY_POW = 3.0f;

//�u�[�X�g�Q�[�W���񕜂��n�߂�܂ł̎���
constexpr float RECHARGE_BOOST_DELAY = 100.0f;
//�u�[�X�g�Q�[�W�񕜃J�E���g�̃f���^�^�C���ɂ�����萔
constexpr float RECHARGE_BOOST_COUNT_RATE = 60.0f;
//�u�[�X�g�Q�[�W�񕜂̃f���^�^�C���ɂ�����萔
constexpr float RECHARGE_BOOST_RATE = 10.0f;

//�u�[�X�g�ړ��̃f���^�^�C���ɂ�����萔
constexpr float BOOST_MOVE_RATE = 60.0f;

//���n�d������
constexpr float FALL_STAN_TIME = 40.0f;
 //���n�d���v���f���^�^�C���ɂ�����萔
constexpr float FALL_STAN_RATE = 100.0f;
//�i���d������
constexpr float COMBAT_STAN_TIME = 30.0f;
//�i���d���v���f���^�^�C���ɂ�����萔
constexpr float COMBAT_STAN_RATE = 100.0f;

 //�����G�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 20.0f,20.0f,20.0f };
//�����G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFFECT_EXPLOSION_OFFSET = { 1.0f,1.0f,1.0f };
//�����G�t�F�N�g�̉�]
constexpr VECTOR EFFECT_EXPLOSION_ROT = { 1.0f,1.0f,1.0f };

//�E�W�F�b�g�p�b�N�G�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_JETPACK_RIGHT_SCALE = { 10.0f, 40.0f, 20.0f };
//�E�W�F�b�g�p�b�N�G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFFECT_JETPACK_RIGHT_OFFSET = { 15.5f,157.0f,-19.0f };
//�E�W�F�b�g�p�b�N�G�t�F�N�g�̉�]
constexpr VECTOR EFFECT_JETPACK_RIGHT_ROT = { -40.0f * DX_PI_F / 180,0.0f,0.0f };

//���W�F�b�g�p�b�N�G�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_JETPACK_LEFT_SCALE = { 10.0f, 40.0f, 20.0f };
//���W�F�b�g�p�b�N�G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFFECT_JETPACK_LEFT_OFFSET = { -4.5f,157.0f,-19.0f };
//���W�F�b�g�p�b�N�G�t�F�N�g�̉�]
constexpr VECTOR EFFECT_JETPACK_LEFT_ROT = { -40.0f * DX_PI_F / 180,0.0f,0.0f };

//�u�[�X�ƃG�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_BOOST_SCALE = { 40.0f,40.0f,40.0f };
//�����G�t�F�N�g�̃I�t�Z�b�g
constexpr VECTOR EFFECT_BOOST_OFFSET = { -4.5f, 157.0f, -19.0f };

//�y���G�t�F�N�g�̑傫��
constexpr VECTOR EFFECT_DUST_SCALE = { 20.0f,20.0f,20.0f };

//��l�p���[�h�{�X�̍��W�␳�l
constexpr float OFFSET_BOSS_ENEMY = 1000.0f;

//��l�l�p���[�h���W�␳�l(�l�Ȃ�)
constexpr float OFFSET_ENEMY = 0.0f;

  //�v���C���[�̏����ʒu
constexpr VECTOR DEFAULT_BOSS_POS = { 0.0f, 37.0f, -4000.0f };

constexpr VECTOR DEFAULT_PLAYER2_POS = { 0.0f, -750.0f, 4000.0f };

//�v���C���[2��]
constexpr VECTOR DEFAULT_PLAYER2_ROT = { 0.0f, 180.0f, 0.0f };

 //�X�[�p�[�A�[�}�[�̍ő�l
constexpr int MAX_SUPER_ARMOR = 2;

//�W�����v���Ɏg�p����A�f���^�^�C���ɂ�����u�[�X�g�Q�[�W�����萔
constexpr float JUMP_BOOST_DAMPING_RATE = 60.0f;
//�W�����v��
constexpr float JUMP_POW = 2500.0f;
//�W�����v�͂����X�Ɍ��炷�f���^�^�C���ɂ�����萔
constexpr float DECREASE_JUMP_POW_RATE = 100.0f;

//�J�v�Z�����_���W
constexpr float CAPSULE_TOP = 200.0f;
//�J�v�Z����Ӎ��W
constexpr float CAPSULE_DOWN = 20.0f;
//�J�v�Z�����a
constexpr float CAPSULE_RADIUS = 100.0f;

//�ˌ��\���Ԍv���f���^�^�C���ɂ�����萔
constexpr float SHOT_FRAME_RATE = 6.0f;
//�Ŏˌ��\����
constexpr float SHOT_FRAME_TIME = 5.0f;

//�d�͔���̃��C������̎n�܂�ƏI���
constexpr float COLL_CHEAK_START_SCALE = 25.0f;
constexpr float COLL_CHEAK_END_SCALE = 10.0f;

constexpr float STAGE_OUT = 5000.0f;

//�X�e�[�W�̒��S���W
constexpr VECTOR STAGE_CENTER = { 0.0f,0.0f,0.0f };

//�d�͕���
constexpr VECTOR GRAVITY_DIR = { 0.0f,-1.0f,0.0f };
//�t�d�͕���
constexpr VECTOR GRAVITY_UP_DIR = { 0.0f,1.0f,0.0f };

//�A���O������p�x
constexpr float LIMIT_ANGLE = 70.0f;

//�u�[�X�g�R��
constexpr float BOOST_FUEL = 100.0f;

#pragma endregion


Player::Player(int playerType, int playMode):playerType_(playerType),
state_(nullptr),input_(std::make_unique<Input>(padNum_)),camera_(std::make_unique<Camera>())
{
	playMode_ = static_cast<PLAY_MODE>(playMode);

	actorType_ = static_cast<ACTOR_TYPE>(playerType_);

	//�v���C���[���Ŏg���ق��N���X�𐶐�
	ChangeState(std::make_unique<IdleState>(*this));
	if (playMode_ == PLAY_MODE::SINGLE_MODE)
	{
		offsetEnemy_ = OFFSET_BOSS_ENEMY;
	}
	else
	{
		offsetEnemy_ = OFFSET_ENEMY;
	}	
}

Player::~Player()
{

}

void Player::Init(void)
{	
	//���f���̐���
	InitTransform();
	//�I�u�W�F�N�g�̐���
	MakeObjects();	
	//
	InitParameter();
	//�A�j���[�V�����̒ǉ�
	InitAnimation();
	//�G�t�F�N�g�̒ǉ�
	InitEffect();
	//�J�����̏�����
	InitCamera();
}

void Player::MakeObjects(void)
{
	//�A�j���[�V�����R���g���[���[�̍Đ�
	robotAnimeController_ = std::make_unique< RobotAnimeController>(transform_.modelId);
	//�r�[�����C�t���̐���
	beamRifle_ = std::make_unique <BeamRifle>(playerType_, static_cast<int> (playMode_),*this);
	//�r�[���T�[�x���̐���
	beamSaber_ = std::make_unique <BeamSaber>(playerType_,transform_);
	//�J�v�Z�������蔻��̐���
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSULE_TOP, CAPSULE_DOWN, CAPSULE_RADIUS);
	//�G�t�F�N�g�}�l�[�W���[�̐���
	effectManager_ = std::make_unique<EffectManager>(transform_);
}

void Player::InitTransform(void)
{
	//���f���̓ǂݍ���
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//�傫��
	transform_.scl = AsoUtility::VECTOR_ONE;
	//���W
	transform_.pos = { DEFAULT_BOSS_POS.x, DEFAULT_BOSS_POS.y, DEFAULT_BOSS_POS.z  };
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(PLAYER_INIT_ROT);
	//�v���C���[�^�C�v�ɂ���ĉ�]�A���W��ς���
	if (playerType_ == static_cast<int>(PLAYER_TYPE::PLAYER_2))
	{
		transform_.pos = { DEFAULT_PLAYER2_POS.x, DEFAULT_PLAYER2_POS.y, DEFAULT_PLAYER2_POS.z };
		moveQua_ =Quaternion::Euler({ DEFAULT_PLAYER2_ROT.x,
					 AsoUtility::Deg2RadF(DEFAULT_PLAYER2_ROT.y),
					  DEFAULT_PLAYER2_ROT.z });
	}
	movedPos_ = transform_.pos;
	transform_.Update();
}

void Player::InitParameter(void)
{
	//������Ԃ�Idol���
	actorState_ = STATE::IDLE;
	//�p�b�h��1�n�܂�Ȃ̂�+1
	padNum_ = playerType_ + 1;
	//�ړ���
	movePow_ = AsoUtility::VECTOR_ZERO;
	//�d�͕���
	dirGravity_ = GRAVITY_DIR;
	//�d�͂̋t����
	dirUpGravity_ = GRAVITY_UP_DIR;
	startUpperQuaRotY_ = transform_.quaRot;
	//�u�[�X�g�Q�[�W�̒l�̏�����
	boostFuel_ = MAX_BOOST_GAGE;
	//HP�l�̏�����
	hp_ = MAX_PLAYER_HP;
	//�u�[�X�g�Q�[�W�񕜃J�E���g�̏�����
	rechargeBoostCount_ = RECHARGE_BOOST_DELAY;
	//���n�d���l�̏�����
	landingStanTime_ = 0.0f;
	//�ō����̏�����
	maxMoveSpeed_ = 0.0f;
	//�ˌ��d���l�̏�����
	shotFlame_ = 0.0f;
	//���G���Ԃ̏�����
	safeTime_ = 0.0f;
	//�d�͒l�̏�����
	gravityPow_ = DEFAULT_GRAVITY_POW;
	//�X�[�p�[�A�[�}�[�l�̏�����
	superArmor_ = 0;
	//�㔼�g�P��t���O�̏�����
	isRevertUpperBodyRot_ = false;
	//�ˌ��t���O�̏�����
	isShotEnable_	= false;
	//�ǔ��t���O�̏�����
	isHorming_	= true;
	//�u�[�X�g�Q�[�W�񕜃t���O�̏�����
	rechargeBoostFlag_ = false;

	numberofBullets_ = &beamRifle_->GetNumnberOfBullets();
}

void Player::InitAnimation(void)
{
	//�A�j���[�V�����̒ǉ�
	robotAnimeController_->Add(static_cast<int>(STATE::IDLE), PATH_ANIMATION_PLAYER + "Idle.mv1", 20.0f, 750.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::RUN), PATH_ANIMATION_PLAYER + "Run.mv1", 22.0f, 100.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::SHOT), PATH_ANIMATION_PLAYER + "shot.mv1", 160.0f, 100.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::BOOST), PATH_ANIMATION_PLAYER + "Hover.mv1", 30.0f, 20.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::BOOST_DASH), PATH_ANIMATION_PLAYER + "Hover.mv1", 30.0f, 20.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::JUMP), PATH_ANIMATION_PLAYER + "Jump.mv1", 50.0f, 100.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::DAMAGE), PATH_ANIMATION_PLAYER + "Reaction.mv1", 45.0f, 38.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::COMBAT), PATH_ANIMATION_PLAYER + "Attack_360_High.mv1", 120.0f, 160.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::COMBAT_RUN), PATH_ANIMATION_PLAYER + "RunWithSword.mv1", 20.0f, 20.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::DOWN), PATH_ANIMATION_PLAYER + "Stunned.mv1", 100.0f, 96.0f);
	robotAnimeController_->Add(static_cast<int>(STATE::WIN), PATH_ANIMATION_PLAYER + "Win.mv1", 60.0f, 170.0f);

	robotAnimeController_->Update();
}

void Player::InitEffect(void)
{
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(EFFECT_TYPE::LOSE),
		EFFECT_EXPLOSION_SCALE,
		EFFECT_EXPLOSION_OFFSET,
		EFFECT_EXPLOSION_ROT,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::EXPLOSION).handleId_);	

	//�o�b�N�p�b�N�E�o�[�j�A
	effectManager_->Add(static_cast<int>(EFFECT_TYPE::JET_BACK_RIGHT),
		EFFECT_JETPACK_RIGHT_SCALE,
		EFFECT_JETPACK_RIGHT_OFFSET,
		EFFECT_JETPACK_RIGHT_ROT,
		false,
		true,
		resMng_.Load(ResourceManager::SRC::VERNIER).handleId_);

	//�o�b�N�p�b�N���o�[�j�A
	effectManager_->Add(static_cast<int>(EFFECT_TYPE::JET_BACK_LEFT),
		EFFECT_JETPACK_LEFT_SCALE,
		EFFECT_JETPACK_LEFT_OFFSET,
		EFFECT_JETPACK_LEFT_ROT,
		false,
		true,
		resMng_.Load(ResourceManager::SRC::VERNIER).handleId_);

	//�u�[�X�g
	effectManager_->Add(static_cast<int>(EFFECT_TYPE::BOOST),
		EFFECT_BOOST_SCALE,
		EFFECT_BOOST_OFFSET,
		AsoUtility::VECTOR_ZERO,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::BOOST).handleId_);

	//�y��
	effectManager_->Add(static_cast<int>(EFFECT_TYPE::DUST_CLOUD),
		EFFECT_DUST_SCALE,
		AsoUtility::VECTOR_ZERO,
		AsoUtility::VECTOR_ZERO,
		false,
		false,
		resMng_.Load(ResourceManager::SRC::DUST_CLOUD).handleId_);

}

void Player::Update()
{
	//�f���^�^�C���̍X�V
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();

	//�R���g���[�����͂̍X�V
	input_->Update();
	
	//�J�����ɓG�̍��W��n��
	auto enemyPos = enemyPos_;
	camera_->SetTargetPos(*enemyPos_);

	//�u�[�X�g�Q�[�W��
	RecoverBoostGauge();

	//�����ΐ탂�[�h���ǂ����𔻒�
	if (playMode_ == PLAY_MODE::BATTLE_MODE)
	{
		//���݂̓G�̏�Ԃ𒲂ׂ�
		StopHomingIfBoosted();
	}
	//���݂̃X�e�[�g�̃A�b�v�f�[�g
	state_->Update();

	//���G���Ԃ�����΂�������炵�Ă���
	if (!IsSafeTimeSufficient())
	{
		CountSafeTime(deltaTime_ * 50.0f);
	}

	//�s���s�\���Ԍv��
	CountCombatStanTime();

	//�G�Ƃ̋����ɉ����ăz�[�~���O�̗L�������߂�
	RangeDistance();

	// �A�j���[�V�����Đ�
	robotAnimeController_->Update();

	//�n�ʂƂ̓����蔻��
	CollisionGravity();

	//�X�e�[�W�O�ɏo���Ȃ�����
	StageOut();

	//���W�ړ��A���f����]�Ȃǂ̍X�V
	TransformUpdate();

	//�G�t�F�N�g�Đ�
	effectManager_->Update();

	//�e����̃A�b�v�f�[�g
	WeaponUpdate();

	//�����̗̑͂��O�Ȃ�s�k��Ԃɕς���
	IsDead();
}

void Player::TransformUpdate(void)
{
	movePow_ = VAdd(movePow_, jumpPow_);
	movedPos_ = VAdd(movedPos_, movePow_);
	transform_.pos = movedPos_;
	transform_.quaRot = quaRot_;
	transform_.Update();
	capsule_->Update();
}

void Player::WeaponUpdate(void)
{
	beamRifle_->Update();
	beamSaber_->Update();
}

void Player::RangeDistance(void)
{
	//����Ƃ̋����𑪂�
	VECTOR enemyVec= VSub(*enemyPos_, transform_.pos);
	enemyVec = { abs(enemyVec.x),abs(enemyVec.y),abs(enemyVec.z) };
	enemyDistance_ = AsoUtility::SqrMagnitudeF(enemyVec);
	enemyDistance_ /= 10000.0f;

}

void Player::HormingRange(void)
{
	//����Ƃ̋����ɂ���āA�ǔ����邩�����߂�
	RangeDistance();
	if (enemyDistance_< WITHIN_RANGE)
	{
		isHorming_ = true;
	}
	else
	{
		isHorming_ = false;
	}
}
bool Player::IsDead(void)
{
	//HP��0�ȉ��Ȃ玀�S����
	if (!IsValueSufficient(hp_,-1))
	{
		return true;
		//�s�k��Ԃɂ���
		Lose();
	}
	return true;
}

void Player::Lose(void)
{
	//�s�k��ԂɈڍs
	ChangeState(std::make_unique<LoseState>(*this));
}

void Player::Win(void)
{
	isWin_ = true;
	//������ԂɈڍs
	ChangeState(std::make_unique<WinnerState>(*this));
}

const Player::STATE& Player::GetState(void)
{
	return actorState_;
}

void Player::DamageSuperArmor(void)
{
	//�X�[�p�[�A�[�}�[�����炷
	superArmor_ --;
}

void Player::Draw(void)
{	//���f���̕`��
	MV1DrawModel(transform_.modelId);
	beamRifle_->Draw();
	beamSaber_->Draw();
}

void Player::PlayEffect(EFFECT_TYPE effectType)
{
	effectManager_-> Play(static_cast<int>(effectType));
}

void Player::StopEffect(EFFECT_TYPE effectType)
{
	effectManager_->Stop(static_cast<int>(effectType));
}

void Player::CameraDraw()
{
	camera_->SetBeforeDraw();
}

const Transform& Player::GetTransform(void) const
{
	return transform_;
}

void Player::CollisionGravity(void)
{
	//
	VECTOR checkPos = VAdd(transform_.pos, VScale(transform_.GetUp(), COLL_CHEAK_START_SCALE));
	VECTOR checkDPos = VAdd(transform_.pos, VScale(dirGravity_, COLL_CHEAK_END_SCALE));
	for(auto& collider:colliders_)
	{
		auto hit = MV1CollCheck_Line(collider->modelId_, -1, checkPos, checkDPos);
		//���������������̓����蔻��
		jumpDot_ = VDot(dirGravity_, VNorm(jumpPow_));

		//�v���C���[�����������A�n�ʂɐG��Ă�����true
		if ( jumpDot_ > 0.9f && hit.HitFlag>0.0f )
		{
			float dis = 4.0f;
			//���n�����ꏊ�̍����ɍ��킹��
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity_, dis));
			//�W�����v�͂�����
			jumpPow_ = VECTOR{0.0f,0.0f,0.0f};
			//�d�͂�߂�
			gravityPow_ = GRAVITY_POW ;
			//�u�[�X�g�Q�[�W�񕜃t���O��true�ɂ���
			rechargeBoostFlag_ = true;
			//�ڒn�t���O��true�ɂ���
			isGrounded_ = true;
			//���n�d���J�E���g���v��
			CountLandingStanTime();
		}
		else
		{
			isGrounded_ = false;
			landingStanTime_ = FALL_STAN_TIME;
			//�������Ɉړ����Ă���Ƃ��̓t�H�[����Ԃɂ���
			if (actorState_ ==STATE::IDLE|| actorState_ == STATE::RUN)
			{
				ChangeState(std::make_unique<FallState>(*this));
			}
		}
	}
}

void Player::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void Player::SetEnemyPosition(const VECTOR* enemyPos)
{
	enemyPos_ = enemyPos;
}

void Player::SetEnemyHp(const float* enemyHp)
{
	enemyHp_ = enemyHp;
}

void Player::SetEnemyState(const STATE* enemyState)
{
	enemyState_ = enemyState;
}

void Player::SetMoveSpeed(float value)
{
	moveSpeed_ = value;
}

void Player::SetMaxMoveSpeed(float value)
{
	maxMoveSpeed_ = value;
}

void Player::SetGravityPow(float value)
{
	gravityPow_ = value;
}

void Player::SetJumpSpeed(float value)
{
	jumpSpeed_ = value;
}

void Player::SetJumpPowY(float value)
{
	jumpPow_.y = value;
}

void Player::BoostDashEnd(void)
{
	moveQua_ = moveBoostQua_;
}


void Player::InitCamera(void)
{
	//�J�����̏����ݒ�
	camera_->SetTransform(&transform_);
	camera_->Init();
}

void Player::InitWeaponCllider(void)
{
	for (auto& collider : colliders_)
	{
		beamRifle_->AddCollider(collider);
	}
}

BeamRifle& Player::GetBeamRifle(void) const
{
	return *beamRifle_;
}

BeamSaber& Player::GetBeamSaber(void) const
{
	return *beamSaber_;
}

int Player::GetPlayerType(void) const
{
	return playerType_;
}

float Player::GetDeltaTime(void) const
{
	return deltaTime_;
}

void Player::ChangeState(std::unique_ptr<StateBase> state)
{
	state_=std::move(state) ;
} 


void Player::StopHomingIfBoosted(void)
{
	if (*enemyState_ ==STATE::BOOST)
	{
		beamRifle_->InActivateHorming();
	}
}

void Player::SpeedAdd(void)
{
	if (maxMoveSpeed_> moveSpeed_)
	{
		moveSpeed_ += (deltaTime_ * DEFAULT_RATE);
	}
	else
	{
		moveSpeed_ = maxMoveSpeed_;
	}
}

void Player::Move(void)
{
	movePow_ = VScale(moveQua_.GetForward(),moveSpeed_*(deltaTime_ * DEFAULT_RATE));
	Quaternion xOutQuaRot = moveQua_;
	xOutQuaRot.x = moveQua_.x = 0.0f;
	quaRot_ = Quaternion::LookRotation(xOutQuaRot.GetForward());
}

void Player::MoveBoostDash(void)
{
	movePow_ = VScale(moveBoostQua_.GetForward(), moveSpeed_ * (deltaTime_ * DEFAULT_RATE));
	Quaternion xOutQuaRot = moveBoostQua_;
	xOutQuaRot.x = moveBoostQua_.x = 0.0f;
	quaRot_ = Quaternion::LookRotation(xOutQuaRot.GetForward());

}

void Player::Combat(void)
{
	superArmor_ = MAX_SUPER_ARMOR;
	//�i���O��ԂɈڍs
	ChangeState(std::make_unique<CombatTransitionState>(*this ,transform_.pos,movePow_ ,quaRot_, offsetEnemy_) );
}


void Player::ConsumeBoostGauge(float rate)
{
	//�Q�[�W�����炷
	ConsumeGauge(boostFuel_, rate);
	//���ȉ��ɂȂ�����l���Œ�
	if (boostFuel_<=0.0f)
	{
		boostFuel_ = 0.0f;
	}
}

void Player::StopRechargeBoost(void)
{	//�u�[�X�g�Q�[�W�̉񕜂��~�߂�
	rechargeBoostCount_ = 0;
	rechargeBoostFlag_ = false;
}

bool Player::IsSuperArmor(void)
{
	//�X�[�p�[�A�[�}�[���c���Ă��邩�ǂ����𔻒�
	if (superArmor_>0)
	{
		return true;
	}
	return false;
}

const float& Player::GetBoostFuel(void) const
{
	return boostFuel_;
}

const bool& Player::IsWin(void) const
{
	return isWin_;
}

const int& Player::GetNumnberOfBullets(void)
{
	return beamRifle_->GetNumnberOfBullets();
}

void Player::Jump(void)
{
	//�u�[�X�g�Q�[�W�����ȏ�c���Ă���΃W�����v����
	if (IsValueSufficient(boostFuel_, MIN_JUMP_COST))
	{
		//�W�����v�͂�ݒ�
		jumpSpeed_ = JUMP_POW;
		VECTOR jump = { 0.0f,1.0f,0.0f };
		jumpPow_ = VScale(jump, jumpSpeed_ * (deltaTime_));
		//�d�͂�0�ɂ���
		gravityPow_ = 0.0f;
		//�u�[�X�g�Q�[�W�����炷
		ConsumeBoostGauge(JUMP_BOOST_DAMPING_RATE * (deltaTime_));
		//�u�[�X�g�Q�[�W�񕜂��~�߂�
		rechargeBoostCount_ = 0;
		rechargeBoostFlag_ = false;
	}
}

void Player::CalcGravity()
{
	VECTOR gravity = VScale(dirGravity_, gravityPow_* (deltaTime_));
	//�㉺�̈ړ��ʂɏd�͂�������
	jumpPow_ = VAdd(jumpPow_, gravity);
	float dot = VDot(dirGravity_, jumpPow_);
	if (!(dot > 0.0f))
	{
		return;
	}
	//�d�͂����Z
	if (gravityPow_ < MAX_GRAVITY_POW)
	{
		gravityPow_ += GRAVITY_RATE *(deltaTime_) ;
	}
	else//���𒴂��Ă�Œ�
	{
		gravityPow_ = MAX_GRAVITY_POW ;
	}
	//�W�����v�͂����炷
	if (jumpPow_.y < -MAX_GRAVITY_POW)
	{
		jumpPow_.y =  -MAX_GRAVITY_POW;
	}	

}

void Player::StageOut(void)
{
	//float posX = abs(transform_.pos.x);
	//float posZ = abs(transform_.pos.z);

	//stageDistance_ =(posX*posX) + (posZ * posZ);

	//float distance = 17000.0f * 17000.0f;

	////���ȏ�O�ɂ͂����Ȃ�
	//if (IsValueSufficient(stageDistance_, distance))
	//{
	//	//�X�e�[�W�̒��S���W
	//	VECTOR stageCenter = { 250.0f,0.0f,250.0f };
	//	//�v���C���[���W
	//	VECTOR pos {transform_.pos.x,0.0f,transform_.pos.z };

	//	//�v���C���[����X�e�[�W�ւ̒��S�ւ̃x�N�g��
	//	VECTOR vec = VNorm(VSub(stageCenter, pos));
	//	movedPos_ = VAdd(movedPos_, VScale(vec, 1.0f));
	//	MoveStop();
	//}

	const float LEN = 17000.0f;
	const float LEN_POW = LEN * LEN;

	//�X�e�[�W�̒��S���W
	const VECTOR stageCenter = { 250.0f,0.0f,250.0f };

	VECTOR pos = movedPos_;

	//�X�e�[�W����v���C���[
	float diffX = (pos.x - stageCenter.x);
	float diffZ = (pos.z - stageCenter.z);

	stageDistance_ = (diffX * diffX) + (diffZ * diffZ);

	//���ȏ�O�ɂ͂����Ȃ�
	if (IsValueSufficient(stageDistance_, LEN_POW))
	{
		//�v���C���[����X�e�[�W�ւ̒��S�ւ̃x�N�g��
		VECTOR outDir = VNorm(VECTOR(diffX, 0.0f, diffZ));
		movedPos_ = VAdd(stageCenter, VScale(outDir, LEN));
		movedPos_.y = pos.y;
		//MoveStop();
	}

}

void Player::ResetShotFlame(void)
{
	shotFlame_ = 7.0f;
}

void Player::RecoverBoostGauge(void)
{
	//�u�[�X�g�Q�[�W�񕜃t���O��true�łȂ���Ζ߂�
	if (!rechargeBoostFlag_)
	{
		return;
	}
	//�u�[�X�g�Q�[�W�񕜊J�n���Ԃ��v������
	if (!IsValueSufficient(rechargeBoostCount_, RECHARGE_BOOST_DELAY))
	{
		rechargeBoostCount_+= (deltaTime_ * RECHARGE_BOOST_COUNT_RATE);
	}
	else
	{
		//�u�[�X�g�Q�[�W���񕜂���
		if (!IsValueSufficient(boostFuel_, MAX_BOOST_GAGE))
		{
			boostFuel_+= 10.0f*(deltaTime_ * RECHARGE_BOOST_RATE);
			rechargeBoostCount_ = RECHARGE_BOOST_DELAY;
		}
		else//���𒴂��Ă�~�߂�
		{
			boostFuel_ = BOOST_FUEL;
			rechargeBoostFlag_ = false;
		}
	}	
}


const bool Player::IsValueSufficient(float Gauge, float RequiredGaugeAmount) const
{
	//����1������2�ȏォ�ǂ����𔻒肷��
	if(Gauge >= RequiredGaugeAmount)
	{
		return true;
	}
	else
	{
		return false;
	}
}

 bool Player::IsBoostGaugeSufficient(float RequiredGaugeAmount) 
{
	 //�u�[�X�g�Q�[�W�����ʂ𒴂��Ă��邩�𔻒�
	return IsValueSufficient(boostFuel_,RequiredGaugeAmount);
}

 bool Player::IsSafeTimeSufficient(void)
 {
	 //���G���Ԃ��I����Ă��邩�ǂ����𔻒�
	 if (safeTime_ > 0.0f)
	 {
		 return false;
	 }
	 else
	 {
		 safeTime_ = 0.0f;
		 return true;
	 }
}

void Player::MoveStop(void)
{	//�ړ��ʂ�0�ɂ���
	movePow_ = VECTOR{ 0.0f,0.0f,0.0f };
}

void Player::JumpStop(void)
{	//�W�����v�͂�0�ɂ���
	jumpPow_ = VECTOR{ 0.0f,0.0f,0.0f };
}

void Player::GravityOne(void)
{
	gravityPow_ = 1.0f;
}

void Player::CountSafeTime( float value)
{
	ConsumeGauge(safeTime_, value);
}

void Player::SetSafeTime(float value)
{
	safeTime_ = value;
}

void Player::RechargeBoostCountReset(void)
{
	rechargeBoostCount_ = 0;
}

const bool Player::LandingStunEnded(void) const
{
	//���n�d�����Ԃ��I��������ǂ����𔻒�
	if (landingStanTime_>0.0f)
	{
		return false;
	}
	return true;
}

const bool Player::CombatStunEnded(void) const
{
	//�U���d�����Ԃ��I��������ǂ����𔻒�
	if (combatStanTime_ > 0.0f)
	{
		return false;
	}
	return true;
}

const bool Player::AllStanEnded(void) const
{
	//���n�d���ƍU���d�����I����Ă��邩����
	if (CombatStunEnded() && LandingStunEnded())
	{
		return true;
	}
	return false;
}

void Player::SetCombatStan(float stanTime)
{
	//�U���d�����Ԃ�ݒ�
	combatStanTime_ = stanTime;
}

void Player::CountLandingStanTime(void)
{
	//���n�d�����Ԍv��
	if (!LandingStunEnded())
	{
		landingStanTime_ -= (deltaTime_ * FALL_STAN_RATE);
	}
	else
	{
		landingStanTime_ = 0.0f;
	}
}

void Player::CountCombatStanTime(void)
{
	//�U���d�����Ԍv��
	if (!CombatStunEnded())
	{
		combatStanTime_ -= (deltaTime_ * COMBAT_STAN_RATE);
	}
	else
	{
		combatStanTime_ = 0.0f;
	}
}

void Player::MoveSpeedZero(void)
{
	//�X�s�[�h��0�ɂȂ�悤�ɁA���X�Ɍ��炷
	if (moveSpeed_>0.0f)
	{
		moveSpeed_ -=  (deltaTime_ * DEFAULT_RATE);
	}
	else
	{
		moveSpeed_ = 0.0f;
	}
}

void Player::JumpPowZero(void)
{
	//�W�����v�͂����炷
	if (jumpPow_.y >= 0.0f)
	{
		jumpPow_.y -= DECREASE_JUMP_POW_RATE * (deltaTime_);
	}
}

bool Player::StickAdjustment(VECTOR v1, VECTOR v2)
{
	//�X�e�B�b�N������|����Ă��邩�𔻒肷��
	if (v1.x >= v2.x || v1.y >= v2.y)
	{
		return true;
	}
	return false;
}

void Player::ConsumeGauge(float& gauge, float rate)
{
	//�Q�[�W�����炷
	gauge -= rate;
	if (gauge<=0.0f)
	{
		gauge = 0.0f;
	}
}
void Player::CalculateAngleToTarget(void)
{
	//����Ɍ������ăx�N�g�������
	VECTOR enemyPos = *enemyPos_;
	enemyVec_ = VSub({ enemyPos.x,0.0f,enemyPos.z }, { transform_.pos.x, 0.0f, transform_.pos.z });

	//�����̑O���x�N�g���Ƒ���Ɍ������Ẵx�N�g���Ԃ̉�]�ʂ��Ƃ�
	goalUpperQuaRotY_ = Quaternion::FromToRotation(transform_.GetForward(), enemyVec_);

	//��������G�ւ̊p�x
	angle_ = AsoUtility::Rad2DegF(goalUpperQuaRotY_.ToEuler().y);

	//�G�Ƃ̊p�x�����ȏ�ȉ��Ȃ����̊p�x�ɂ���
	if (angle_ >= LIMIT_ANGLE)
	{
		goalUpperQuaRotY_ = Quaternion::Euler(goalUpperQuaRotY_.x,AsoUtility::Deg2RadF(120.0f), 0.0f);
		goalUpperQuaRotY_ = Quaternion::Euler(goalUpperQuaRotY_.x,AsoUtility::Deg2RadF(120.0f), 0.0f);
	}
	else if ((angle_ < 0.0f && angle_ < -60.0f))
	{
		goalUpperQuaRotY_ = Quaternion::Euler(goalUpperQuaRotY_.x, AsoUtility::Deg2RadF(-120.0f), 0.0f);
	}
	goalUpperQuaRotY_ = { goalUpperQuaRotY_.w,0.0f,goalUpperQuaRotY_.y,0.0f };
}

void Player::RevertRotate(void)
{
	//���X�̃��f����matrix(�w��)
	MATRIX frameLocalMatrix = MV1GetFrameBaseLocalMatrix(transform_.modelId, 61);
	//���f���̃��[�J���̌���(�N�H�[�^�j�I��)
	Quaternion frameLocalQua = Quaternion::GetRotation(frameLocalMatrix);
	startUpperQuaRotY_ = Quaternion::Slerp(startUpperQuaRotY_, frameLocalQua, 0.3);
	MV1SetFrameUserLocalMatrix(transform_.modelId, 61, Quaternion::ToMatrix(startUpperQuaRotY_));

	float start = static_cast<float>(abs(startUpperQuaRotY_.w));
	float goal = static_cast<float>(abs(frameLocalQua.w));
	if (start== goal)
	{
		isRevertUpperBodyRot_ = false;
		isShotEnable_ = false;
	}
}

void Player::Shot(void)
{
	if (input_->IsTriggerd("shot")&&!isShotEnable_)
	{
		//�ˌ��t���O��true��
		isShotEnable_ = true;
		//���݂̃��f���̌������i�[
		startUpperQuaRotY_ = transform_.quaRot;
		//���f���̖ڐ��������瑊������ւ̊p�x�𑪂�
		CalculateAngleToTarget();
		//�e�𔭎˂���
		beamRifle_->Trigger();
		//�ˌ��A�j���[�V�������Đ�
		PlayUpperAnim(static_cast<int>(STATE::SHOT), true, true, false);
		//���P��t���O��true
		isRevertUpperBodyRot_ = true;
	}
	//�ˌ��t���O��true�Ȃ�
	if (isShotEnable_)
	{
		startUpperQuaRotY_ = { startUpperQuaRotY_.w,0.0f,startUpperQuaRotY_.y,0.0f };	
		startUpperQuaRotY_ = Quaternion::Slerp(startUpperQuaRotY_, goalUpperQuaRotY_, 1.0);
		MV1SetFrameUserLocalMatrix(transform_.modelId, 61, Quaternion::ToMatrix(startUpperQuaRotY_));
		shotFlame_ += (deltaTime_ * SHOT_FRAME_RATE);
		if (shotFlame_ > SHOT_FRAME_TIME)
		{
			shotFlame_ = 0.0f;
			isShotEnable_ = false;
		}
	}
	if (isRevertUpperBodyRot_)
	{
		RevertRotate();
	}
}

void Player::ChangeDamageState(void)
{
	ChangeState(std::make_unique<DamageState>(*this));
}

void Player::ChangeDwonState(void)
{
	ChangeState(std::make_unique<DownState>(*this));
}

void Player::GetMoveDir(void)
{
	// �ق����l�́|1�`1�Ȃ̂�1000�Ŋ���
	float padX = static_cast<float>( input_->keyLx_);
	float padY = static_cast<float>(input_->keyLy_);

	float padx = padX / 1000.0f;
	// �㉺���]������
	float pady = padY * -1.0f / 1000.0f;
	// Y���Ȃ������x�N�g��
	padDir_ = { padx,0.0f,pady };
	// ���K��
	padDir_ = AsoUtility::VNormalize(padDir_);

	padX = abs(padX);
	padY = abs(padY);

	// Y���Ȃ������x�N�g��
	if (padX > 0 || padY > 0)
	{
		padDirBoost_ = { padx,0.0f,pady };
	}
	// ���K��
	padDirBoost_ = AsoUtility::VNormalize(padDirBoost_);
	SetBoostQuaRot();

	//L�X�e�B�b�N�����ȏ�|����Ă�����movespeed�����Z����
	if (StickAdjustment({padX,padY},{400.0f,400.0f}))
	{
		SetMoveQuaRot();
		if (CombatStunEnded())
		{
			SpeedAdd();
		}
	}
	else
	{
		//�W�����v������Ȃ���΃X�s�[�h���O�ɂȂ�܂Ō��Z����
		if (!(actorState_ == STATE::JUMP))
		{
			MoveSpeedZero();
		}
	}
}

void Player::GetMoveDirBoostDash(void)
{
	// �ق����l�́|1�`1�Ȃ̂�1000�Ŋ���
	float padX = static_cast<float>(input_->keyLx_);
	float padY = static_cast<float>(input_->keyLy_);

	float padx = padX / 1000.0f;
	// �㉺���]������
	float pady = padY * -1.0f / 1000.0f;

	padX = abs(padX);
	padY = abs(padY);


	// Y���Ȃ������x�N�g��
	if (padX >0 || padY > 0)
	{
		padDirBoost_ = { padx,0.0f,pady };
	}
	// ���K��
	padDirBoost_ = AsoUtility::VNormalize(padDirBoost_);
	SetBoostQuaRot();
}

void Player::SetMoveQuaRot(void)
{
	Quaternion cameraRot = camera_->GetQuaRotOutX();
	float rad = atan2f(padDir_.x, padDir_.z);
	//�A�i���O�X�e�B�b�N�̓|���Ă��������Quaternion
	Quaternion quaPadDir = Quaternion::AngleAxis(rad, cameraRot.GetUp());
	//�J�����̉�]�ƍ��������N�H�[�^�j�I��
	moveQua_ = Quaternion::Mult(cameraRot, quaPadDir);
}

void Player::SetBoostQuaRot(void)
{
	Quaternion cameraRot = camera_->GetQuaRotOutX();
	float rad = atan2f(padDirBoost_.x, padDirBoost_.z);
	//�A�i���O�X�e�B�b�N�̓|���Ă��������Quaternion
	Quaternion quaPadDir = Quaternion::AngleAxis(rad, cameraRot.GetUp());
	//�J�����̉�]�ƍ��������N�H�[�^�j�I��
	moveBoostQua_ = Quaternion::Mult(cameraRot, quaPadDir);

}

void Player::BoostQuaRot(void)
{
	Quaternion xOutQuaRot = moveQua_;
	xOutQuaRot.x = moveQua_.x = 0.0f;
	quaRot_ = Quaternion::LookRotation(xOutQuaRot.GetForward());
}


void Player::Boost(void)
{
	// �ړ�����
	movePow_ = VScale(quaRot_.GetForward(), MOVE_SPEED_BOOST * (deltaTime_ * BOOST_MOVE_RATE));
	StopRechargeBoost();
}

const bool Player::IsGround(void) const
{
	return isGrounded_;
}

const bool& Player::IsHorming(void) const
{
	return isHorming_;
}

const VECTOR Player::GetEnemyPos(void) const
{
	return *enemyPos_;
}

const VECTOR& Player::GetPlayerPos(void) const
{
	return transform_.pos;
}

const Input& Player::GetInput(void) const
{
	return *(input_);
}
