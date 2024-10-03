#include"../../State/PlayerState/CombatTransitionState.h"
#include"../../State/PlayerState/DamageState.h"
#include"../../State/PlayerState/WinnerState.h"
#include"../../State/PlayerState/IdleState.h"
#include"../../State/PlayerState/FallState.h"
#include"../../State/PlayerState/DownState.h"
#include"../../State/PlayerState/LoseState.h"
#include"../../../Manager/ResourceManager.h"
#include"../../../Utility/AsoUtility.h"
#include"../../DamageObject/BeamShot.h"
#include"../../Time/DeltaTime.h"
#include"../../../../Config.h"
#include "Player.h"
#include<DxLib.h>
#include<string>

#pragma region Parameter

//�f���^�^�C���ɂ�����萔(60.0f)
 constexpr float DEFAULT_RATE = 60.0f;

 //�v���C���[�̏����q�b�g�|�C���g
 constexpr float MAX_PLAYER_HP = 1000.0f;

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

//��l�p���[�h�{�X�̍��W�␳�l
 constexpr float OFFSET_BOSS_ENEMY = 1000.0f;

//��l�l�p���[�h���W�␳�l(�l�Ȃ�)
 constexpr float OFFSET_ENEMY = 0.0f;

 //�v���C���[�̏����ʒu
 constexpr VECTOR DEFAULT_POS = { 0.0f, -30.0f, 800.0f };

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

 constexpr float DEFAULT_DISTANCE = 4000.0f;



#pragma endregion


Player::Player(int playerNum, int playMode):playerType_(playerNum),
state_(nullptr),input_(std::make_unique<Input>(padNum_)),camera_(std::make_unique<Camera>())
{
	playMode_ = static_cast<PLAY_MODE>(playMode);
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
	InitParameter();
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
	//�A�j���[�V�����̒ǉ�
	InitAnimation();
	//�J�����̏�����
	InitCamera();
}

void Player::MakeObjects(void)
{
	//�A�j���[�V�����R���g���[���[�̍Đ�
	roboAnimeController_ = std::make_unique< RobotAnimeController>(transform_.modelId);
	//�r�[�����C�t���̐���
	beamRifle_ = std::make_unique <BeamRifle>(playerType_, static_cast<int> (playMode_),*this);
	//�r�[���T�[�x���̐���
	beamSaber_ = std::make_unique <BeamSaber>(playerType_,transform_);
	//�J�v�Z�������蔻��̐���
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSULE_TOP, CAPSULE_DOWN, CAPSULE_RADIUS);
	//UI�̐���
	userInterface_ = std::make_unique<UserInterface>(resMng_, enemyPos_, enemyDistance_, boostGauge_,
		playerHp_, *enemyHp_,isWin_, beamRifle_->GetNumnberOfBullets() , static_cast<int> (playMode_),playerType_, input_->GetJPadType());
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
	transform_.pos = { DEFAULT_POS.x, DEFAULT_POS.y, (DEFAULT_POS.z * playerType_) };
	//��]
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//�v���C���[�^�C�v�ɂ���ĉ�]�A���W��ς���
	if (playerType_ == static_cast<int>(PLAYER_TYPE::PLAYER_2))
	{
		moveQua_ =Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
		transform_.pos.z += DEFAULT_DISTANCE;
	}
	movedPos_ = transform_.pos;
	transform_.Update();
	//�G�Ƃ̋����𑪂�
	Range();
}

void Player::InitParameter(void)
{
	//������Ԃ�Idol���
	pState_ = STATE::IDLE;
	//�p�b�h��1�n�܂�Ȃ̂�+1
	padNum_ = playerType_ + 1;
	//�ړ���
	movePow_ = AsoUtility::VECTOR_ZERO;
	//�ړ�����W
	movedPos_ = AsoUtility::VECTOR_ZERO;
	//�d�͕���
	dirGravity_ = { 0.0f,-1.0f,0.0f };
	//�d�͂̋t����
	dirUpGravity_ = { 0.0f,1.0f,0.0f };
	startUpperQuaRotY_ = transform_.quaRot;
	//�u�[�X�g�Q�[�W�̒l�̏�����
	boostGauge_ = MAX_BOOST_GAGE;
	//HP�l�̏�����
	playerHp_ = MAX_PLAYER_HP;
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
	revertFlag_ = false;
	//�ˌ��t���O�̏�����
	shotFlag_	= false;
	//�ǔ��t���O�̏�����
	isHorming_	= true;
	//�u�[�X�g�Q�[�W�񕜃t���O�̏�����
	rechargeBoostFlag_ = false;
}

void Player::InitAnimation(void)
{
	//�A�j���[�V�����̒ǉ�
	roboAnimeController_->Add(static_cast<int>(STATE::IDLE), PATH_ANIMATION_PLAYER + "Idle.mv1", 20.0f, 750.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::RUN), PATH_ANIMATION_PLAYER + "Run.mv1", 22.0f, 100.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::SHOT), PATH_ANIMATION_PLAYER + "shot.mv1", 160.0f, 100.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::BOOST), PATH_ANIMATION_PLAYER + "Hover.mv1", 30.0f, 20.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::BOOST_DASH), PATH_ANIMATION_PLAYER + "Hover.mv1", 30.0f, 20.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::JUMP), PATH_ANIMATION_PLAYER + "Jump.mv1", 50.0f, 100.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::DAMAGE), PATH_ANIMATION_PLAYER + "Reaction.mv1", 45.0f, 38.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::COMBAT), PATH_ANIMATION_PLAYER + "Attack_360_High.mv1", 120.0f, 160.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::COMBAT_RUN), PATH_ANIMATION_PLAYER + "RunWithSword.mv1", 20.0f, 20.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::DOWN), PATH_ANIMATION_PLAYER + "Stunned.mv1", 100.0f, 96.0f);
	roboAnimeController_->Add(static_cast<int>(STATE::WIN), PATH_ANIMATION_PLAYER + "Win.mv1", 60.0f, 170.0f);

	roboAnimeController_->Update();
	//�G�t�F�N�g�̒ǉ�
	effectManager_->Add(static_cast<int>(STATE::LOSE), EFFECT_EXPLOSION_SCALE, false,
		resMng_.Load(ResourceManager::SRC::EXPLOSION).handleId_);

}

void Player::UpdateBattleMode(void)
{
	//�J�����ɓG�̍��W��n��
	camera_->SetTargetPos(*enemyPos_);

	//�u�[�X�g�Q�[�W��
	RecoverBoostGauge();

	if (playMode_ == PLAY_MODE::BATTLE_MODE)
	{
		//���݂̓G�̏�Ԃ𒲂ׂ�
		EnemyState();
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
	Range();

	// �A�j���[�V�����Đ�
	roboAnimeController_->Update();

	//�G�t�F�N�g�Đ�
	effectManager_->Update();

	//�n�ʂƂ̓����蔻��
	CollisionGravity();

	//���W�ړ��A���f����]�Ȃǂ̍X�V
	TransformUpdate();

	//�e����̃A�b�v�f�[�g
	WeaponUpdate();

	//�����̗̑͂��O�Ȃ�s�k��Ԃɕς���
	IsDead();

}

void Player::UpdateSIngleMode(void)
{
	VECTOR enemyPos = { enemyPos_->x,enemyPos_->y + offsetEnemy_,enemyPos_->z };
	//�J�����ɓG�̍��W��n��
	camera_->SetTargetPos(enemyPos);
	//�u�[�X�g�Q�[�W��
	RecoverBoostGauge();

	//�v���C���[�̌��݂̃X�e�[�g�̃A�b�v�f�[�g
	state_->Update();

	if (!IsSafeTimeSufficient())
	{
		CountSafeTime(deltaTime_ * 50.0f);
	}

	//�s���s�\���Ԍv��
	CountCombatStanTime();

	//�G�Ƃ̋����ɉ����ăz�[�~���O�̗L�������߂�
	Range();

	// �A�j���[�V�����Đ�
	roboAnimeController_->Update();

	//�G�t�F�N�g�Đ�
	effectManager_->Update();

	//�n�ʂƂ̓����蔻��
	CollisionGravity();

	//���W�ړ��A���f����]�Ȃǂ̍X�V
	TransformUpdate();

	//�e����̃A�b�v�f�[�g
	WeaponUpdate();

	//�����̗̑͂��O�Ȃ�s�k��Ԃɕς���
	IsDead();

}

void Player::Update()
{
	//�f���^�^�C���̍X�V
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();

	//�R���g���[�����͂̍X�V
	input_->Update();
	
	//�J�����ɓG�̍��W��n��
	camera_->SetTargetPos(*enemyPos_);

	//�u�[�X�g�Q�[�W��
	RecoverBoostGauge();

	//�����ΐ탂�[�h���ǂ����𔻒�
	if (playMode_ == PLAY_MODE::BATTLE_MODE)
	{
		//���݂̓G�̏�Ԃ𒲂ׂ�
		EnemyState();
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
	Range();

	// �A�j���[�V�����Đ�
	roboAnimeController_->Update();

	//�G�t�F�N�g�Đ�
	effectManager_->Update();

	//�n�ʂƂ̓����蔻��
	CollisionGravity();

	//���W�ړ��A���f����]�Ȃǂ̍X�V
	TransformUpdate();

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

void Player::Range(void)
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
	if (!IsGaugeSufficient(playerHp_,-1))
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
	return pState_;
}

void Player::DamageSuperArmor(void)
{
	superArmor_ --;
}

void Player::Draw(void)
{	//���f���̕`��
	MV1DrawModel(transform_.modelId);
	beamRifle_->Draw();
	beamSaber_->Draw();
}


void Player::PlayEffect(STATE state)
{
	effectManager_-> Play(static_cast<int>(state));
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
			groundedFlag_ = true;
			//���n�d���J�E���g���v��
			CountLandingStanTime();
		}
		else
		{
			groundedFlag_ = false;
			landingStanTime_ = FALL_STAN_TIME;
			//�������Ɉړ����Ă���Ƃ��̓t�H�[����Ԃɂ���
			if (pState_==STATE::IDLE|| pState_ == STATE::RUN)
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

void Player::DebugPlayerState()
{
	switch (pState_)
	{
	case Player::STATE::RUN:
		debugString_ = "RUN";
		break;
	case Player::STATE::JUMP:
		debugString_ = "JUMP";
		break;
	case Player::STATE::DAMAGE:
		debugString_ = "DAMAGE";
		break;
	case Player::STATE::SHOT:
		debugString_ = "SHOT";
		break;
	case Player::STATE::IDLE:
		debugString_ = "IDLE";
		break;
	case Player::STATE::BOOST:
		debugString_ = "BOOST";
		break;
	case Player::STATE::BOOST_DASH:
		debugString_ = "BOOST_DASH";
		break;
	case Player::STATE::FALL:
		debugString_ = "FALL";
		break;
	case Player::STATE::COMBAT:
		debugString_ = "COMBAT1";
		break;
	case Player::STATE::COMBAT_RUN:
		debugString_ = "COMBAT_RUN";
		break;
	default:
		break;
	}

}

void Player::EnemyState(void)
{
	switch (*enemyState_)
	{
	case Player::STATE::RUN:
		enemyDebugString_ = "RUN";
		break;
	case Player::STATE::JUMP:
		enemyDebugString_ = "JUMP";
		break;
	case Player::STATE::DAMAGE:
		enemyDebugString_ = "DAMAGE";
		break;
	case Player::STATE::SHOT:
		enemyDebugString_ = "SHOT";
		break;
	case Player::STATE::IDLE:
		enemyDebugString_ = "IDLE";
		break;
	case Player::STATE::BOOST:
		enemyDebugString_ = "BOOST";
		beamRifle_->InActivateHorming();
		break;
	case Player::STATE::BOOST_DASH:
		enemyDebugString_ = "BOOST_DASH";
		break;
	case Player::STATE::FALL:
		enemyDebugString_ = "FALL";
		break;
	case Player::STATE::COMBAT:
		enemyDebugString_ = "COMBAT1";
		break;
	case Player::STATE::COMBAT_RUN:
		enemyDebugString_ = "COMBAT_RUN";
		beamRifle_->InActivateHorming();
		break;
	default:
		break;
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

void Player::MoveBoodtDash(void)
{
	movePow_ = VScale(moveBoostQua_.GetForward(), moveSpeed_ * (deltaTime_ * DEFAULT_RATE));
	Quaternion xOutQuaRot = moveBoostQua_;
	xOutQuaRot.x = moveBoostQua_.x = 0.0f;
	quaRot_ = Quaternion::LookRotation(xOutQuaRot.GetForward());

}

void Player::RobotAnimDebugDraw(int playerType)
{
	if (playerType  == playerType_)
	{

		roboAnimeController_->Draw();
	}
}

void Player::PlayerDebugDraw(int playerType)
{
	if (playerType  == playerType_)
	{
		// �ق����l�́|1�`1�Ȃ̂�1000�Ŋ���
		float padX = input_->keyLx_ / 1000.0f;
		// �㉺���]������
		float padY = input_->keyLy_ * -1.0f / 1000.0f;

		// Y���Ȃ������x�N�g��
		VECTOR padDir = {};
		padDir = { padX,0.0f,padY };
		DrawFormatStringF(0.0f, 0.0f, 0xffffff, "x = %f,y = %f,z = %f",
			transform_.pos.x, transform_.pos.y, transform_.pos.z);
		DrawFormatStringF(0.0f, 160.0f, 0xffffff, "movePow:x = %f,y = %f,z = %f",
			movePow_.x, movePow_.y, movePow_.z);
		DrawFormatStringF(0.0f, 180.0f, 0xffffff, "movedPos_:x = %f,y = %f,z = %f",
			movedPos_.x, movedPos_.y, movedPos_.z);
		DrawFormatStringF(0.0f, 30.0f, 0xffffff, "deltatime = %f", deltaTime_);
		DrawFormatStringF(0.0f, 100.0f, 0xffffff, "RX = %d,RY =%d", input_->keyRx_, input_->keyRy_);
		DrawFormatStringF(0.0f, 120.0f, 0xffffff, "LX = %d,LY =%d", input_->keyLx_, input_->keyLy_);
		DrawFormatStringF(0.0f, 200.0f, 0xffffff, "padDir:LX = %d,LY =%d", padX, padY);
		DrawFormatStringF(0.0f, 140.0f, 0xffffff, "%s", debugString_.c_str());
		DrawFormatStringF(0.0f, 230.0f, 0xffffff, "�Z�b�g����Ă���A�j���[�V�����̐�:%d", MV1GetAnimNum(transform_.modelId));
		auto vec = VNorm(VSub(*enemyPos_, transform_.pos));

		DrawFormatStringF(0.0f, 60.0f, 0xffffff, "�G�Ƃ̊p�x:%f", angle_);
		DrawFormatStringF(0.0f, 80.0f, 0xffffff, "GetForward:%f,%f,%f", transform_.GetForward().x, transform_.GetForward().y, transform_.GetForward().z);
		DrawFormatStringF(0.0f, 270.0f, 0xffffff, "goalUpperQuaRotY_:%f,%f,%f,%f", goalUpperQuaRotY_.w, goalUpperQuaRotY_.x, goalUpperQuaRotY_.y, goalUpperQuaRotY_.z);
		DrawFormatStringF(0.0f, 290.0f, 0xffffff, "startUpperQuaRotY_:%f,%f,%f,%f", startUpperQuaRotY_.w, startUpperQuaRotY_.x, startUpperQuaRotY_.y, startUpperQuaRotY_.z);
		DrawFormatStringF(0.0f, 310.0f, 0xffffff, "QuaRot_:%f,%f.%f", quaRot_.x, quaRot_.y, quaRot_.z);

		//���X�̃��f����matrix(�w��)
		MATRIX frameLocalMatrix = MV1GetFrameBaseLocalMatrix(transform_.modelId, 61);
		//���f���̃��[�J���̌���(�N�H�[�^�j�I��)
		Quaternion frameLocalQua = Quaternion::GetRotation(frameLocalMatrix);
		DrawFormatStringF(0.0f, 330.0f, 0xffffff, "frameLocalQua:%f,%f.%f", frameLocalQua.x, frameLocalQua.y, frameLocalQua.z);
		DrawFormatStringF(0.0f, 350.0f, 0xffffff, "jumpPow_:%f,%f.%f", jumpPow_.x, jumpPow_.y, jumpPow_.z);
		DrawFormatStringF(0.0f, 370.0f, 0xffffff, "gravityPow_:%f", gravityPow_);
		DrawFormatStringF(0.0f, 390.0f, 0xffffff, "jumpAdd_:%f", jumpSpeed_);
		roboAnimeController_->DebugDraw();
		camera_->DrawDebug();
		float dot = VDot(dirGravity_, jumpPow_);
		DrawFormatStringF(0.0f, 470.0f, 0xffffff, "jumpdot:%f", dot);
		DrawFormatStringF(0.0f, 500.0f, 0xffffff, "boostGauge_:%f", boostGauge_);
		DrawFormatStringF(0.0f, 590.0f, 0xffffff, "shotFlame_:%f", shotFlame_);
		DrawFormatStringF(0.0f, 610.0f, 0xffffff, "recoverBoostCount_:%f", rechargeBoostCount_);
		DrawFormatStringF(0.0f, 630.0f, 0xffffff, "landingStanTime_:%f", landingStanTime_);
		DrawFormatStringF(0.0f, 650.0f, 0xffffff, "combatStanTime_:%f",combatStanTime_);
		DrawFormatStringF(0.0f, 650.0f, 0xffffff, "moveSpeed_:%f", moveSpeed_);
		DrawFormatStringF(0.0f, 670.0f, 0xffffff, "moveQua_:%f,%f.%f", moveQua_.x, moveQua_.y, moveQua_.z);
		DrawFormatStringF(0.0f, 690.0f, 0xffffff, "moveBoostQua_:%f,%f.%f", moveBoostQua_.x, moveBoostQua_.y, moveBoostQua_.z);
		DrawFormatStringF(0.0f, 750.0f, 0xffffff, "enemyDistance_%f", enemyDistance_);
		DrawFormatStringF(0.0f, 810.0f, 0xffffff, "safeTime_:%f", safeTime_);
		DrawFormatStringF(0.0f, 820.0f, 0xffffff, "debugString_%s", debugString_.c_str());
		DrawFormatStringF(0.0f, 840.0f, 0xffffff, "jumpAdd_%f", jumpSpeed_*deltaTime_);
		DrawFormatStringF(0.0f, 860.0f, 0xffffff, "gravityPow_%f", gravityPow_*deltaTime_);
		DebugPlayerState();

		std::string str;
		if (shotFlag_)
		{
			str = "shotFlag:true";
		}
		else
		{
			str = "shotFlag:false";
		}
		DrawFormatStringF(0.0f, 710.0f, 0xffffff, "%s", str.c_str());
		if (revertFlag_)
		{
			str = "revertFlag_:true";
		}
		else
		{
			str = "revertFlag_:false";
		}
		DrawFormatStringF(0.0f, 730.0f, 0xffffff, "%s", str.c_str());
		for (auto& beam : GetBeamRifle().GetBeams())
		{
			beam->Draw();
		}
	}
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
	ConsumeGauge(boostGauge_, rate);
	//���ȉ��ɂȂ�����l���Œ�
	if (boostGauge_<=0.0f)
	{
		boostGauge_ = 0.0f;
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

std::unique_ptr<UserInterface> Player::MoveUI(void)
{	//��������UI��n��
	return std::move(userInterface_);
}

void Player::Jump(void)
{
	//�u�[�X�g�Q�[�W�����ȏ�c���Ă���΃W�����v����
	if (IsGaugeSufficient(boostGauge_, MIN_JUMP_BOOST))
	{
		//�W�����v�͂�ݒ�
		jumpSpeed_ = JUMP_POW;
		VECTOR jump = { 0.0f,1.0f,0.0f };
		jumpPow_ = VScale(jump, jumpSpeed_ * (deltaTime_));
		//�d�͂�0�ɂ���
		gravityPow_ = 0.0f;
		//�u�[�X�g�Q�[�W�����炷
		//boostGauge_ -= JUMP_BOOST_DAMPING_RATE*(deltaTime_ );
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
	if (!IsGaugeSufficient(rechargeBoostCount_, RECHARGE_BOOST_DELAY))
	{
		rechargeBoostCount_+= (deltaTime_ * RECHARGE_BOOST_COUNT_RATE);
	}
	else
	{
		//�u�[�X�g�Q�[�W���񕜂���
		if (!IsGaugeSufficient(boostGauge_, MAX_BOOST_GAGE))
		{
			boostGauge_+= 10.0f*(deltaTime_ * RECHARGE_BOOST_RATE);
			rechargeBoostCount_ = RECHARGE_BOOST_DELAY;
		}
		else//���𒴂��Ă�~�߂�
		{
			boostGauge_ = 100.0f;
			rechargeBoostFlag_ = false;
		}
	}	
}


const bool Player::IsGaugeSufficient(float Gauge, float RequiredGaugeAmount) const
{
	//����1������2�ُ킩�ǂ����𔻒肷��
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
	return IsGaugeSufficient(boostGauge_,RequiredGaugeAmount);
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
void Player::CalculateAngleToTarget()
{
	//����Ɍ������ăx�N�g�������
	VECTOR enemyPos = *enemyPos_;
	enemyVec_ = VSub({ enemyPos.x,0.0f,enemyPos.z }, { transform_.pos.x, 0.0f, transform_.pos.z });

	//�����̑O���x�N�g���Ƒ���Ɍ������Ẵx�N�g���Ԃ̉�]�ʂ��Ƃ�
	goalUpperQuaRotY_ = Quaternion::FromToRotation(transform_.GetForward(), enemyVec_);

	//��������G�ւ̊p�x
	angle_ = AsoUtility::Rad2DegF(goalUpperQuaRotY_.ToEuler().y);

	//�G�Ƃ̊p�x�����ȏ�ȉ��Ȃ����̊p�x�ɂ���
	if (angle_ >= 70.0f)
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

void Player::RevertRotate()
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
		revertFlag_ = false;
		shotFlag_ = false;
	}
}

void Player::Shot(void)
{
	if (input_->IsTriggerd("shot")&&!shotFlag_)
	{
		//�ˌ��t���O��true��
		shotFlag_ = true;
		//���݂̃��f���̌������i�[
		startUpperQuaRotY_ = transform_.quaRot;
		//���f���̖ڐ��������瑊������ւ̊p�x�𑪂�
		CalculateAngleToTarget();
		//�e�𔭎˂���
		beamRifle_->Trigger();
		//�ˌ��A�j���[�V�������Đ�
		PlayUpperAnim(static_cast<int>(STATE::SHOT), true, true, false);
		//���P��t���O��true
		revertFlag_ = true;
	}
	//�ˌ��t���O��true�Ȃ�
	if (shotFlag_)
	{
		startUpperQuaRotY_ = { startUpperQuaRotY_.w,0.0f,startUpperQuaRotY_.y,0.0f };	
		startUpperQuaRotY_ = Quaternion::Slerp(startUpperQuaRotY_, goalUpperQuaRotY_, 1.0);
		MV1SetFrameUserLocalMatrix(transform_.modelId, 61, Quaternion::ToMatrix(startUpperQuaRotY_));
		shotFlame_ += (deltaTime_ * SHOT_FRAME_RATE);
		if (shotFlame_ > SHOT_FRAME_TIME)
		{
			shotFlame_ = 0.0f;
			shotFlag_ = false;
		}
	}
	if (revertFlag_)
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
		if (!(pState_ == STATE::JUMP))
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
	movePow_ = VScale(quaRot_.GetForward(), BOOST_MOVE_SPEED * (deltaTime_ * BOOST_MOVE_RATE));
	StopRechargeBoost();
}

const bool Player::IsGround(void) const
{
	return groundedFlag_;
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

const Input& Player::GetInput() const
{
	return *(input_);
}
