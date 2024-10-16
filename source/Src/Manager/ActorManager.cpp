#include "ActorManager.h"
#include"../Object/Actor/ActorBase.h"
#include"../Object/Actor/Enemy/BossEnemy.h"
#include"../Object/Actor/Player/Player.h"
#include"../Object/DamageObject/Saber.h"
#include"../Object/Common/Collider.h"
#include"../UI/UserInterface.h"

#pragma region Parameter

//プレイヤー１のタイプ
constexpr int PLAYER_TYPE = 0;

constexpr int SINGLE_PLAY = 0;

constexpr float STAGE_OUT_Y = -1000.0f;

#pragma endregion



ActorManager::ActorManager(int playMode)
{
	playMode_ = playMode;
	InitActor();
	if (playMode_ == SINGLE_PLAY)
	{
		//InitSingleMode();
		//_update = &ActorManager::UpdateSingleMode;
		update_ = std::bind(&ActorManager::UpdateSingleMode, this);
	}
	else
	{	
		//InitActor();
		//_update = &ActorManager::UpdateBattleMode;
		update_ = std::bind(&ActorManager::UpdateBattleMode, this);
	}
	//どちらかのプレイヤーが死んだかどうかを判定
	isDeadAnyPlayer_ = false;
}

void ActorManager::InitActor(void)
{
	// プレイヤーの生成と初期化
	for (int playerType = 0; playerType <= playMode_; playerType++)
	{
		std::unique_ptr player = std::make_unique<Player>(playerType, playMode_);
		//プレイヤーの初期化
		player->Init();
		//配列に格納
		players_.emplace_back(std::move(player));
	}

	//プレイヤー1
	int player1 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_1);

	//シングルモードならプレイヤー１のUIとボスの情報を設定
	if (playMode_== SINGLE_PLAY)
	{
		//ボスの生成
		int boss = static_cast<int>(ActorBase::ACTOR_TYPE::BOSS);
		bossEnemy_ = std::make_unique<BossEnemy>(boss);


		//プレイヤーのパラメータをボス側に渡す
		SetEnemyInfo(*bossEnemy_, *players_[player1]) ;

		//ボス側のパラメータをプレイヤーに渡す
		SetEnemyInfo(*players_[player1], *bossEnemy_);

		//ボスの攻撃オブジェクト生成
		bossEnemy_->MakeSpMoveObjects();

		//プレイヤー1のUIを生成
		//CreateUserInterface(*players_[player1], *bossEnemy_);

	}
	else
	{
		//プレイヤー2
		int player2 = static_cast<int>(ActorBase::ACTOR_TYPE::PLAYER_2);

		//敵プレイヤーのパラメータを持ってくる
		SetEnemyInfo(*players_[player1], *players_[player2]);
		SetEnemyInfo(*players_[player2], *players_[player1]);

		//プレイヤー１とプレイヤー２のUIを生成
		//CreateUserInterface(*players_[player1], *players_[player2]);
		//CreateUserInterface(*players_[player2], *players_[player1]);

	}
}

void ActorManager::Update()
{
	for (auto& player : players_)
	{
		player->Update();
	}
	//(this->*_update)();
	update_();
}

void ActorManager::UpdateBattleMode(void)
{
	//弾との当たり判定
	DamegeShotCollision();
	//剣との当たり判定
	DamegeSwordCollision();
	//勝者が決まったかどうかを判定
	IsWin();
}

void ActorManager::UpdateSingleMode(void)
{
	//ボスのアップデート
	bossEnemy_->Update();
	//ボスの攻撃との当たり判定
	DmageBossSpMoveCollision();
	//ボスと弾との当たり判定
	DmageBossBeamCollision();
	//勝者が決まったかどうかを判定
	IsSingleModeWin();         
}

void ActorManager::Draw(void)
{
	for (auto& player : players_)
	{
		player->Draw();
	}
	//シングルモードならボスを描画
	if (playMode_ == SINGLE_PLAY)
	{
		bossEnemy_->Draw();
	}
}

void ActorManager::DrawDebug(int playerType)
{
	for (auto& player : players_)
	{
		player->PlayerDebugDraw(playerType);
	}
}

void ActorManager::DrawAnimeDebug(int playerType)
{
	for (auto& player : players_)
	{
		player->RobotAnimDebugDraw(playerType);
	}
}

const std::vector<std::unique_ptr<Player>>& ActorManager::GetPlayers(void)const
{
	return players_;
}

const std::unique_ptr<BossEnemy>& ActorManager::GetBossEnemy(void)const
{
	return bossEnemy_;
}

std::vector<std::unique_ptr<UserInterface>> ActorManager::MoveUI(void)
{
	return std::move(userInterfaces_);
}

bool& ActorManager::IsDeadAnyPlayer(void)
{
	return isDeadAnyPlayer_;
}

void ActorManager::DrawCamera(int playerType)
{
	players_[playerType]->CameraDraw();
}

void ActorManager::DrawUI(int playerType)
{
	userInterfaces_[playerType]->Draw(isDeadAnyPlayer_);
}

void ActorManager::DrawCommonUI(const float& startCount, const bool& isGameSet, const int& rematchMode)
{
	userInterfaces_[PLAYER_TYPE]->DrawCommonUI(startCount, isGameSet, rematchMode);
}


void ActorManager::AddColliders(Collider* collider)
{
	for (auto& player : players_)
	{
		player->AddCollider(collider);
	}
}


void ActorManager::DamegeShotCollision(void)
{
	//当たり判定をとるプレイヤー(当てられる側)
	for (auto& player : players_)
	{
		//弾を打つプレイヤー(当てる側)
		for (auto& playerShots : players_)
		{
			auto& beams= playerShots->GetBeamRifle().GetBeams();
			//弾
			for (auto& beam : beams)
			{
				//プレイヤーと弾のプレイヤータイプが違えば抜ける
				if (IsEqual(player->GetPlayerType(), beam->GetPlayerType()))
				{
					break;
				}
				//弾がアクティブ状態でない、弾と当たってないなら戻る
				if (!(beam->IsActive()) ||
					!player->GetCapsule().Collision(beam->GetModelId()))
				{
					continue;
				}			
				beam->Hit();

				//無敵時間が終わっていたらスーパーアーマーを削る
				if (!player->IsSafeTimeSufficient())
				{
					continue;
				}
				player->DamageSuperArmor();

				//当たった時にスーパーアーマーが無く、格闘移行状態だったら
				if (player->IsSuperArmor())
				{
					continue;
				}
				//格闘移行状態なら大ダメージとダウン状態
				if(player->GetState() == Player::STATE::COMBAT_RUN)
				{
					//ダメージを反映しプレイヤーをダメージステートに変更
					player->Damage(beam->GetBigDamage());
					player->ChangeDwonState();
				}
				//その他なら通常ダメージ
				else
				{
					//ダメージを反映しプレイヤーをダメージステートに変更
					player->Damage(beam->GetDamage());
					player->ChangeDamageState();
				}
			}
		}
	}
}

void ActorManager::DamegeSwordCollision(void)
{
	for (auto& player : players_)
	{
		for (auto& sabers : players_)
		{
			//攻撃側のサーベル
			auto& saber = sabers->GetBeamSaber().GetSaber();			
			//①プレイヤーと剣のプレイヤータイプが違う、
			//②無敵時間が終わってない
			//③剣がアクティブ状態でない
			//一つでも当てはまっていたら戻る
			if (IsEqual(player->GetPlayerType() , sabers->GetPlayerType()) ||!(saber.IsActive())
				|| !player->IsSafeTimeSufficient())
			{	
				continue;
			}
			//サーベルとカプセル当たり判定が接触していたら
			if (player->GetCapsule().Collision(saber.GetModelId()))
			{
				//サーベルを非アクティブ化
				saber.Hit();
				//ダメージ
				player->Damage(saber.GetDamage());
				//ダウン状態に移行
				player->ChangeDwonState();
			}
		}
	}
}

void ActorManager::DmageBossSpMoveCollision(void)
{
	auto& spMoves = bossEnemy_->GetSpMove();
	for (auto& spmove : spMoves)
	{
		//ボスの技がアクティブ状態の時もしくはプレイヤーの無敵時間が終わっていなかったら
		if (!spmove.second->IsActive()|| !players_[SINGLE_PLAY]->IsSafeTimeSufficient())
		{
			continue;
		}
		//技とプレイヤーが当たっていたらプレイヤーをダウン状態にしてHPを減らす
		if (players_[SINGLE_PLAY]->GetCapsule().Collision(spmove.second->GetModelId()))
		{
			//無敵時間を設定
			players_[SINGLE_PLAY]->SetSafeTime(30.0f);
			//ダウン状態に移行
			players_[SINGLE_PLAY]->ChangeDwonState();
			//ダメージ
			players_[SINGLE_PLAY]->Damage(spmove.second->GetDamage());
		}
	}
}

void ActorManager::DmageBossBeamCollision(void)
{
	auto& beams= players_[SINGLE_PLAY]->GetBeamRifle().GetBeams();
	for (auto& beam : beams)
	{
		//弾がアクティブ状態じゃないもしくは、クールタイムが終わっていなければ
		if (!(beam->IsActive()))
		{
			continue;
		}
		//あたっていたら
		if (bossEnemy_->GetCapsule().Collision(beam->GetModelId()))
		{
			//ビームを非アクティブ化
			beam->Hit();
			//ダメージ
			bossEnemy_->Damage(beam->GetDamage());
		}
	}
}

const bool& ActorManager::IsDeadAnyPlayer(void)const
{
	return isDeadAnyPlayer_;
}

void ActorManager::IsWin(void)
{	//どちらかのプレイヤーが死んでいたら戻る
	if (isDeadAnyPlayer_)
	{
		return;
	}
	for (auto& player : players_)
	{
		//体力がゼロになるもしくはステージから落ちたら負けとする
		if (0.0f>= player->GetHP() || StageOut(player->GetPlayerPos()))
		{
			//敗北側プレイヤータイプを格納
			loserPlayerType_ = player->GetPlayerType();
			//勝利側プレイヤータイプを格納
			winnerPlayerType_ = 1 - loserPlayerType_;
			//各プレイヤーを勝利状態、敗北状態に移行
			ChangeStateGameSet();
			//どちらか死亡済みフラグをtrue
			isDeadAnyPlayer_ = true;
		}
	}
}

void ActorManager::IsSingleModeWin(void)
{	//どちらかのプレイヤーが死んでいたら戻る
	if (isDeadAnyPlayer_)
	{
		return;
	}
	//体力がゼロになるもしくはステージから落ちたら負けとする
	if (0.0f >= players_[SINGLE_PLAY]->GetHP() || StageOut(players_[SINGLE_PLAY]->GetPlayerPos()))
	{	//敗北状態に移行
		players_[SINGLE_PLAY]->Lose();
		isDeadAnyPlayer_ = true;
	}
	else if(0.0f >= bossEnemy_->GetHP())
	{
		isDeadAnyPlayer_ = true;
		//勝利状態に移行
		players_[SINGLE_PLAY]->Win();
		//ボスを死亡状態に移行
		bossEnemy_->ChangeDeathState();
	}
	else
	{
	}
}

bool ActorManager::StageOut(const VECTOR& pos) 
{
	//プレイヤーY座標が一定以下になったら
	if (pos.y < STAGE_OUT_Y)
	{	
		return true;
	}
	return false;
}

void ActorManager::ChangeStateGameSet(void)
{	//敗北状態、勝利状態に移行させる
	players_[loserPlayerType_]->Lose();
	players_[winnerPlayerType_]->Win();
}

bool ActorManager::IsEqual(int Type1, int Type2)
{	
	//タイプ１とタイプ２が同タイプかどうかを判定
	if (Type1 == Type2)
	{
		return true;
	}
	return false;
}

void ActorManager::SetEnemyInfo(ActorBase& player, ActorBase& target)
{
	//敵プレイヤーの座標、HP、ステートを設定
	if (player.GetActorType() != ActorBase::ACTOR_TYPE::BOSS)
	{
		//敵のHP情報をセット
		player.SetEnemyHp(&target.GetHP());
		//敵のステート情報をセット
		player.SetEnemyState(&target.GetState());
	}
	//敵の座標情報をセット
	player.SetEnemyPosition(&target.GetTransform().pos);
}

void ActorManager::CreateUserInterface(ActorBase& player, ActorBase& target)
{

	//プレイヤータイプ
	int playerType = static_cast<int>(player.GetActorType());

	//UIを生成
	userInterfaces_.emplace_back(
		std::make_unique<UserInterface>(
			playMode_,
			playerType,
			player.GetEnemyDistance(),
			player.GetNumnberOfBullets(),
			player.GetBoostFuel(),
			player.GetHP(),
			player.IsWin(),
			target.GetTransform().pos,
			target.GetHP()
			)
		);
}


