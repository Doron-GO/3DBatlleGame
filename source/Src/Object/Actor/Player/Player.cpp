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

//デルタタイムにかける定数(60.0f)
 constexpr float DEFAULT_RATE = 60.0f;

 //プレイヤーの初期ヒットポイント
 constexpr float MAX_PLAYER_HP = 1000.0f;

 constexpr float GRAVITY_POW = 30.0f;
//重力デルタタイムにかける定数
 constexpr float GRAVITY_RATE = 100.0f;
 //重力の最大値
 constexpr float MAX_GRAVITY_POW = 30.0f;

//ブーストゲージの最大量
 constexpr float MAX_BOOST_GAGE = 100.0f;
//初期重力値
 constexpr float DEFAULT_GRAVITY_POW = 3.0f;

//ブーストゲージが回復し始めるまでの時間
 constexpr float RECHARGE_BOOST_DELAY = 100.0f;
//ブーストゲージ回復カウントのデルタタイムにかける定数
 constexpr float RECHARGE_BOOST_COUNT_RATE = 60.0f;
//ブーストゲージ回復のデルタタイムにかける定数
 constexpr float RECHARGE_BOOST_RATE = 10.0f;

 //ブースト移動のデルタタイムにかける定数
 constexpr float BOOST_MOVE_RATE = 60.0f;

//着地硬直時間
 constexpr float FALL_STAN_TIME = 40.0f;
 //着地硬直計測デルタタイムにかける定数
 constexpr float FALL_STAN_RATE = 100.0f;
//格闘硬直時間
 constexpr float COMBAT_STAN_TIME = 30.0f;
//格闘硬直計測デルタタイムにかける定数
 constexpr float COMBAT_STAN_RATE = 100.0f;

 //爆発エフェクトの大きさ
 constexpr VECTOR EFFECT_EXPLOSION_SCALE = { 20.0f,20.0f,20.0f };

//一人用モードボスの座標補正値
 constexpr float OFFSET_BOSS_ENEMY = 1000.0f;

//二人人用モード座標補正値(値なし)
 constexpr float OFFSET_ENEMY = 0.0f;

 //プレイヤーの初期位置
 constexpr VECTOR DEFAULT_POS = { 0.0f, -30.0f, 800.0f };

 //スーパーアーマーの最大値
 constexpr int MAX_SUPER_ARMOR = 2;

//ジャンプ時に使用する、デルタタイムにかけるブーストゲージ減少定数
 constexpr float JUMP_BOOST_DAMPING_RATE = 60.0f;
//ジャンプ力
 constexpr float JUMP_POW = 2500.0f;
 //ジャンプ力を徐々に減らすデルタタイムにかける定数
 constexpr float DECREASE_JUMP_POW_RATE = 100.0f;

//カプセル頂点座標
 constexpr float CAPSULE_TOP = 200.0f;
//カプセル底辺座標
 constexpr float CAPSULE_DOWN = 20.0f;
//カプセル半径
 constexpr float CAPSULE_RADIUS = 100.0f;

 //射撃可能時間計測デルタタイムにかける定数
 constexpr float SHOT_FRAME_RATE = 6.0f;
 //最射撃可能時間
 constexpr float SHOT_FRAME_TIME = 5.0f;

 //重力判定のライン判定の始まりと終わり
 constexpr float COLL_CHEAK_START_SCALE = 25.0f;
 constexpr float COLL_CHEAK_END_SCALE = 10.0f;

 constexpr float DEFAULT_DISTANCE = 4000.0f;



#pragma endregion


Player::Player(int playerNum, int playMode):playerType_(playerNum),
state_(nullptr),input_(std::make_unique<Input>(padNum_)),camera_(std::make_unique<Camera>())
{
	playMode_ = static_cast<PLAY_MODE>(playMode);
	//プレイヤー内で使うほかクラスを生成
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
	//モデルの生成
	InitTransform();
	//オブジェクトの生成
	MakeObjects();
	//アニメーションの追加
	InitAnimation();
	//カメラの初期化
	InitCamera();
}

void Player::MakeObjects(void)
{
	//アニメーションコントローラーの再生
	roboAnimeController_ = std::make_unique< RobotAnimeController>(transform_.modelId);
	//ビームライフルの生成
	beamRifle_ = std::make_unique <BeamRifle>(playerType_, static_cast<int> (playMode_),*this);
	//ビームサーベルの生成
	beamSaber_ = std::make_unique <BeamSaber>(playerType_,transform_);
	//カプセル当たり判定の生成
	capsule_ = std::make_unique<CollisionCapsule>(transform_, CAPSULE_TOP, CAPSULE_DOWN, CAPSULE_RADIUS);
	//UIの生成
	userInterface_ = std::make_unique<UserInterface>(resMng_, enemyPos_, enemyDistance_, boostGauge_,
		playerHp_, *enemyHp_,isWin_, beamRifle_->GetNumnberOfBullets() , static_cast<int> (playMode_),playerType_, input_->GetJPadType());
	//エフェクトマネージャーの生成
	effectManager_ = std::make_unique<EffectManager>(transform_);
}

void Player::InitTransform(void)
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//大きさ
	transform_.scl = AsoUtility::VECTOR_ONE;
	//座標
	transform_.pos = { DEFAULT_POS.x, DEFAULT_POS.y, (DEFAULT_POS.z * playerType_) };
	//回転
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//プレイヤータイプによって回転、座標を変える
	if (playerType_ == static_cast<int>(PLAYER_TYPE::PLAYER_2))
	{
		moveQua_ =Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
		transform_.pos.z += DEFAULT_DISTANCE;
	}
	movedPos_ = transform_.pos;
	transform_.Update();
	//敵との距離を測る
	Range();
}

void Player::InitParameter(void)
{
	//初期状態はIdol状態
	pState_ = STATE::IDLE;
	//パッドは1始まりなので+1
	padNum_ = playerType_ + 1;
	//移動量
	movePow_ = AsoUtility::VECTOR_ZERO;
	//移動後座標
	movedPos_ = AsoUtility::VECTOR_ZERO;
	//重力方向
	dirGravity_ = { 0.0f,-1.0f,0.0f };
	//重力の逆方向
	dirUpGravity_ = { 0.0f,1.0f,0.0f };
	startUpperQuaRotY_ = transform_.quaRot;
	//ブーストゲージの値の初期化
	boostGauge_ = MAX_BOOST_GAGE;
	//HP値の初期化
	playerHp_ = MAX_PLAYER_HP;
	//ブーストゲージ回復カウントの初期化
	rechargeBoostCount_ = RECHARGE_BOOST_DELAY;
	//着地硬直値の初期化
	landingStanTime_ = 0.0f;
	//最高速の初期化
	maxMoveSpeed_ = 0.0f;
	//射撃硬直値の初期化
	shotFlame_ = 0.0f;
	//無敵時間の初期化
	safeTime_ = 0.0f;
	//重力値の初期化
	gravityPow_ = DEFAULT_GRAVITY_POW;
	//スーパーアーマー値の初期化
	superArmor_ = 0;
	//上半身捻りフラグの初期化
	revertFlag_ = false;
	//射撃フラグの初期化
	shotFlag_	= false;
	//追尾フラグの初期化
	isHorming_	= true;
	//ブーストゲージ回復フラグの初期化
	rechargeBoostFlag_ = false;
}

void Player::InitAnimation(void)
{
	//アニメーションの追加
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
	//エフェクトの追加
	effectManager_->Add(static_cast<int>(STATE::LOSE), EFFECT_EXPLOSION_SCALE, false,
		resMng_.Load(ResourceManager::SRC::EXPLOSION).handleId_);

}

void Player::UpdateBattleMode(void)
{
	//カメラに敵の座標を渡す
	camera_->SetTargetPos(*enemyPos_);

	//ブーストゲージ回復
	RecoverBoostGauge();

	if (playMode_ == PLAY_MODE::BATTLE_MODE)
	{
		//現在の敵の状態を調べる
		EnemyState();
	}
	//現在のステートのアップデート
	state_->Update();

	//無敵時間があればそれを減らしていく
	if (!IsSafeTimeSufficient())
	{
		CountSafeTime(deltaTime_ * 50.0f);
	}

	//行動不能時間計測
	CountCombatStanTime();

	//敵との距離に応じてホーミングの有無を決める
	Range();

	// アニメーション再生
	roboAnimeController_->Update();

	//エフェクト再生
	effectManager_->Update();

	//地面との当たり判定
	CollisionGravity();

	//座標移動、モデル回転などの更新
	TransformUpdate();

	//各武器のアップデート
	WeaponUpdate();

	//自分の体力が０なら敗北状態に変える
	IsDead();

}

void Player::UpdateSIngleMode(void)
{
	VECTOR enemyPos = { enemyPos_->x,enemyPos_->y + offsetEnemy_,enemyPos_->z };
	//カメラに敵の座標を渡す
	camera_->SetTargetPos(enemyPos);
	//ブーストゲージ回復
	RecoverBoostGauge();

	//プレイヤーの現在のステートのアップデート
	state_->Update();

	if (!IsSafeTimeSufficient())
	{
		CountSafeTime(deltaTime_ * 50.0f);
	}

	//行動不能時間計測
	CountCombatStanTime();

	//敵との距離に応じてホーミングの有無を決める
	Range();

	// アニメーション再生
	roboAnimeController_->Update();

	//エフェクト再生
	effectManager_->Update();

	//地面との当たり判定
	CollisionGravity();

	//座標移動、モデル回転などの更新
	TransformUpdate();

	//各武器のアップデート
	WeaponUpdate();

	//自分の体力が０なら敗北状態に変える
	IsDead();

}

void Player::Update()
{
	//デルタタイムの更新
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();

	//コントローラ入力の更新
	input_->Update();
	
	//カメラに敵の座標を渡す
	camera_->SetTargetPos(*enemyPos_);

	//ブーストゲージ回復
	RecoverBoostGauge();

	//今が対戦モードかどうかを判定
	if (playMode_ == PLAY_MODE::BATTLE_MODE)
	{
		//現在の敵の状態を調べる
		EnemyState();
	}
	//現在のステートのアップデート
	state_->Update();

	//無敵時間があればそれを減らしていく
	if (!IsSafeTimeSufficient())
	{
		CountSafeTime(deltaTime_ * 50.0f);
	}

	//行動不能時間計測
	CountCombatStanTime();

	//敵との距離に応じてホーミングの有無を決める
	Range();

	// アニメーション再生
	roboAnimeController_->Update();

	//エフェクト再生
	effectManager_->Update();

	//地面との当たり判定
	CollisionGravity();

	//座標移動、モデル回転などの更新
	TransformUpdate();

	//各武器のアップデート
	WeaponUpdate();

	//自分の体力が０なら敗北状態に変える
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
	//相手との距離を測る
	VECTOR enemyVec= VSub(*enemyPos_, transform_.pos);
	enemyVec = { abs(enemyVec.x),abs(enemyVec.y),abs(enemyVec.z) };
	enemyDistance_ = AsoUtility::SqrMagnitudeF(enemyVec);
	enemyDistance_ /= 10000.0f;

}

void Player::Range(void)
{
	//相手との距離によって、追尾するかを決める
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
	//HPが0以下なら死亡する
	if (!IsGaugeSufficient(playerHp_,-1))
	{
		return true;
		//敗北状態にする
		Lose();
	}
	return true;
}

void Player::Lose(void)
{
	//敗北状態に移行
	ChangeState(std::make_unique<LoseState>(*this));
}

void Player::Win(void)
{
	isWin_ = true;
	//勝利状態に移行
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
{	//モデルの描画
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
		//落下時だけ足元の当たり判定
		jumpDot_ = VDot(dirGravity_, VNorm(jumpPow_));

		//プレイヤーが落下中かつ、地面に触れていたらtrue
		if ( jumpDot_ > 0.9f && hit.HitFlag>0.0f )
		{
			float dis = 4.0f;
			//着地した場所の高さに合わせる
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity_, dis));
			//ジャンプ力を消す
			jumpPow_ = VECTOR{0.0f,0.0f,0.0f};
			//重力を戻す
			gravityPow_ = GRAVITY_POW ;
			//ブーストゲージ回復フラグをtrueにする
			rechargeBoostFlag_ = true;
			//接地フラグをtrueにする
			groundedFlag_ = true;
			//着地硬直カウントを計測
			CountLandingStanTime();
		}
		else
		{
			groundedFlag_ = false;
			landingStanTime_ = FALL_STAN_TIME;
			//落下中に移動しているときはフォール状態にする
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
	//カメラの初期設定
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
		// ほしい値は－1～1なので1000で割る
		float padX = input_->keyLx_ / 1000.0f;
		// 上下反転させる
		float padY = input_->keyLy_ * -1.0f / 1000.0f;

		// Y軸なし方向ベクトル
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
		DrawFormatStringF(0.0f, 230.0f, 0xffffff, "セットされているアニメーションの数:%d", MV1GetAnimNum(transform_.modelId));
		auto vec = VNorm(VSub(*enemyPos_, transform_.pos));

		DrawFormatStringF(0.0f, 60.0f, 0xffffff, "敵との角度:%f", angle_);
		DrawFormatStringF(0.0f, 80.0f, 0xffffff, "GetForward:%f,%f,%f", transform_.GetForward().x, transform_.GetForward().y, transform_.GetForward().z);
		DrawFormatStringF(0.0f, 270.0f, 0xffffff, "goalUpperQuaRotY_:%f,%f,%f,%f", goalUpperQuaRotY_.w, goalUpperQuaRotY_.x, goalUpperQuaRotY_.y, goalUpperQuaRotY_.z);
		DrawFormatStringF(0.0f, 290.0f, 0xffffff, "startUpperQuaRotY_:%f,%f,%f,%f", startUpperQuaRotY_.w, startUpperQuaRotY_.x, startUpperQuaRotY_.y, startUpperQuaRotY_.z);
		DrawFormatStringF(0.0f, 310.0f, 0xffffff, "QuaRot_:%f,%f.%f", quaRot_.x, quaRot_.y, quaRot_.z);

		//元々のモデルのmatrix(背骨)
		MATRIX frameLocalMatrix = MV1GetFrameBaseLocalMatrix(transform_.modelId, 61);
		//モデルのローカルの向き(クォータニオン)
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
	//格闘前状態に移行
	ChangeState(std::make_unique<CombatTransitionState>(*this ,transform_.pos,movePow_ ,quaRot_, offsetEnemy_) );
}


void Player::ConsumeBoostGauge(float rate)
{
	//ゲージを減らす
	ConsumeGauge(boostGauge_, rate);
	//一定以下になったら値を固定
	if (boostGauge_<=0.0f)
	{
		boostGauge_ = 0.0f;
	}
}

void Player::StopRechargeBoost(void)
{	//ブーストゲージの回復を止める
	rechargeBoostCount_ = 0;
	rechargeBoostFlag_ = false;
}

bool Player::IsSuperArmor(void)
{
	//スーパーアーマーが残っているかどうかを判定
	if (superArmor_>0)
	{
		return true;
	}
	return false;
}

std::unique_ptr<UserInterface> Player::MoveUI(void)
{	//生成したUIを渡す
	return std::move(userInterface_);
}

void Player::Jump(void)
{
	//ブーストゲージが一定以上残っていればジャンプする
	if (IsGaugeSufficient(boostGauge_, MIN_JUMP_BOOST))
	{
		//ジャンプ力を設定
		jumpSpeed_ = JUMP_POW;
		VECTOR jump = { 0.0f,1.0f,0.0f };
		jumpPow_ = VScale(jump, jumpSpeed_ * (deltaTime_));
		//重力を0にする
		gravityPow_ = 0.0f;
		//ブーストゲージを減らす
		//boostGauge_ -= JUMP_BOOST_DAMPING_RATE*(deltaTime_ );
		ConsumeBoostGauge(JUMP_BOOST_DAMPING_RATE * (deltaTime_));
		//ブーストゲージ回復を止める
		rechargeBoostCount_ = 0;
		rechargeBoostFlag_ = false;
	}
}

void Player::CalcGravity()
{
	VECTOR gravity = VScale(dirGravity_, gravityPow_* (deltaTime_));
	//上下の移動量に重力を加える
	jumpPow_ = VAdd(jumpPow_, gravity);
	float dot = VDot(dirGravity_, jumpPow_);
	if (!(dot > 0.0f))
	{
		return;
	}
	//重力を加算
	if (gravityPow_ < MAX_GRAVITY_POW)
	{
		gravityPow_ += GRAVITY_RATE *(deltaTime_) ;
	}
	else//一定を超えてら固定
	{
		gravityPow_ = MAX_GRAVITY_POW ;
	}
	//ジャンプ力を減らす
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
	//ブーストゲージ回復フラグがtrueでなければ戻る
	if (!rechargeBoostFlag_)
	{
		return;
	}
	//ブーストゲージ回復開始時間を計測する
	if (!IsGaugeSufficient(rechargeBoostCount_, RECHARGE_BOOST_DELAY))
	{
		rechargeBoostCount_+= (deltaTime_ * RECHARGE_BOOST_COUNT_RATE);
	}
	else
	{
		//ブーストゲージを回復する
		if (!IsGaugeSufficient(boostGauge_, MAX_BOOST_GAGE))
		{
			boostGauge_+= 10.0f*(deltaTime_ * RECHARGE_BOOST_RATE);
			rechargeBoostCount_ = RECHARGE_BOOST_DELAY;
		}
		else//一定を超えてら止める
		{
			boostGauge_ = 100.0f;
			rechargeBoostFlag_ = false;
		}
	}	
}


const bool Player::IsGaugeSufficient(float Gauge, float RequiredGaugeAmount) const
{
	//引数1が引数2異常かどうかを判定する
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
	 //ブーストゲージが一定量を超えているかを判定
	return IsGaugeSufficient(boostGauge_,RequiredGaugeAmount);
}

 bool Player::IsSafeTimeSufficient(void)
 {
	 //無敵時間が終わっているかどうかを判定
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
{	//移動量を0にする
	movePow_ = VECTOR{ 0.0f,0.0f,0.0f };
}

void Player::JumpStop(void)
{	//ジャンプ力を0にする
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
	//着地硬直時間が終わったかどうかを判定
	if (landingStanTime_>0.0f)
	{
		return false;
	}
	return true;
}

const bool Player::CombatStunEnded(void) const
{
	//攻撃硬直時間が終わったかどうかを判定
	if (combatStanTime_ > 0.0f)
	{
		return false;
	}
	return true;
}

const bool Player::AllStanEnded(void) const
{
	//着地硬直と攻撃硬直が終わっているか判定
	if (CombatStunEnded() && LandingStunEnded())
	{
		return true;
	}
	return false;
}

void Player::SetCombatStan(float stanTime)
{
	//攻撃硬直時間を設定
	combatStanTime_ = stanTime;
}

void Player::CountLandingStanTime(void)
{
	//着地硬直時間計測
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
	//攻撃硬直時間計測
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
	//スピードが0になるように、徐々に減らす
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
	//ジャンプ力を減らす
	if (jumpPow_.y >= 0.0f)
	{
		jumpPow_.y -= DECREASE_JUMP_POW_RATE * (deltaTime_);
	}
}

bool Player::StickAdjustment(VECTOR v1, VECTOR v2)
{
	//スティックが一定上倒されているかを判定する
	if (v1.x >= v2.x || v1.y >= v2.y)
	{
		return true;
	}
	return false;
}

void Player::ConsumeGauge(float& gauge, float rate)
{
	//ゲージを減らす
	gauge -= rate;
	if (gauge<=0.0f)
	{
		gauge = 0.0f;
	}
}
void Player::CalculateAngleToTarget()
{
	//相手に向かってベクトルを作る
	VECTOR enemyPos = *enemyPos_;
	enemyVec_ = VSub({ enemyPos.x,0.0f,enemyPos.z }, { transform_.pos.x, 0.0f, transform_.pos.z });

	//自分の前方ベクトルと相手に向かってのベクトル間の回転量をとる
	goalUpperQuaRotY_ = Quaternion::FromToRotation(transform_.GetForward(), enemyVec_);

	//自分から敵への角度
	angle_ = AsoUtility::Rad2DegF(goalUpperQuaRotY_.ToEuler().y);

	//敵との角度が一定以上以下なら既定の角度にする
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
	//元々のモデルのmatrix(背骨)
	MATRIX frameLocalMatrix = MV1GetFrameBaseLocalMatrix(transform_.modelId, 61);
	//モデルのローカルの向き(クォータニオン)
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
		//射撃フラグをtrueに
		shotFlag_ = true;
		//現在のモデルの向きを格納
		startUpperQuaRotY_ = transform_.quaRot;
		//モデルの目線方向から相手方向への角度を測る
		CalculateAngleToTarget();
		//弾を発射する
		beamRifle_->Trigger();
		//射撃アニメーションを再生
		PlayUpperAnim(static_cast<int>(STATE::SHOT), true, true, false);
		//腰捻りフラグをtrue
		revertFlag_ = true;
	}
	//射撃フラグがtrueなら
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
	// ほしい値は－1～1なので1000で割る
	float padX = static_cast<float>( input_->keyLx_);
	float padY = static_cast<float>(input_->keyLy_);

	float padx = padX / 1000.0f;
	// 上下反転させる
	float pady = padY * -1.0f / 1000.0f;
	// Y軸なし方向ベクトル
	padDir_ = { padx,0.0f,pady };
	// 正規化
	padDir_ = AsoUtility::VNormalize(padDir_);

	padX = abs(padX);
	padY = abs(padY);

	// Y軸なし方向ベクトル
	if (padX > 0 || padY > 0)
	{
		padDirBoost_ = { padx,0.0f,pady };
	}
	// 正規化
	padDirBoost_ = AsoUtility::VNormalize(padDirBoost_);
	SetBoostQuaRot();

	//Lスティックが一定以上倒されていたらmovespeedを加算する
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
		//ジャンプ中じゃなければスピードが０になるまで減算する
		if (!(pState_ == STATE::JUMP))
		{
			MoveSpeedZero();
		}
	}
}

void Player::GetMoveDirBoostDash(void)
{
	// ほしい値は－1～1なので1000で割る
	float padX = static_cast<float>(input_->keyLx_);
	float padY = static_cast<float>(input_->keyLy_);

	float padx = padX / 1000.0f;
	// 上下反転させる
	float pady = padY * -1.0f / 1000.0f;

	padX = abs(padX);
	padY = abs(padY);


	// Y軸なし方向ベクトル
	if (padX >0 || padY > 0)
	{
		padDirBoost_ = { padx,0.0f,pady };
	}
	// 正規化
	padDirBoost_ = AsoUtility::VNormalize(padDirBoost_);
	SetBoostQuaRot();
}

void Player::SetMoveQuaRot(void)
{
	Quaternion cameraRot = camera_->GetQuaRotOutX();
	float rad = atan2f(padDir_.x, padDir_.z);
	//アナログスティックの倒している方向のQuaternion
	Quaternion quaPadDir = Quaternion::AngleAxis(rad, cameraRot.GetUp());
	//カメラの回転と合成したクォータニオン
	moveQua_ = Quaternion::Mult(cameraRot, quaPadDir);
}

void Player::SetBoostQuaRot(void)
{
	Quaternion cameraRot = camera_->GetQuaRotOutX();
	float rad = atan2f(padDirBoost_.x, padDirBoost_.z);
	//アナログスティックの倒している方向のQuaternion
	Quaternion quaPadDir = Quaternion::AngleAxis(rad, cameraRot.GetUp());
	//カメラの回転と合成したクォータニオン
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
	// 移動処理
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
