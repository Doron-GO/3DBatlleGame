#include "ActorManager.h"
#include"../Object/DamageObject/Saber.h"
#include"../Object/Comon/Collider.h"
#include"../UI/UserInterface.h"

#pragma region Parameter

//プレイヤー１のタイプ
constexpr int PLAYER_TYPE = 0;

static constexpr int PLAYER_NUM_MAX = 2;

static constexpr int PLAYER_NUM_MIN = 1;

static constexpr int SINGLE_PLAY = 0;


static constexpr float STAGE_OUT_Y = -1000.0f;

#pragma endregion



ActorManager::ActorManager(int numberofPlayers)
{
	playMode_ = numberofPlayers;
	if (playMode_ == SINGLE_PLAY)
	{
		InitSIngleMode(numberofPlayers);
		_update = &ActorManager::UpdateSingleMode;
	}
	else
	{	
		InitBattleMode(numberofPlayers);
		_update = &ActorManager::UpdateBattleMode;
	}
	isDeadAnyPlayer_ = false;
}

void ActorManager::InitBattleMode(int numberofPlayers)
{
	// プレイヤーの生成と初期化
	for (int num = 0; num < PLAYER_NUM_MAX; num++)
	{
		players_.emplace_back(std::make_unique<Player>(num,numberofPlayers));
	}
	for (auto& player : players_)
	{
		player->Init();
	}
	//UIの生成
	CreateBattleUI();
}

void ActorManager::InitSIngleMode(int numberofPlayers)
{
	//プレイヤーを生成して、配列に格納
	players_.emplace_back(std::make_unique<Player>(SINGLE_PLAY, numberofPlayers));
	//ボスの生成
	bossEnemy_ = std::make_unique<BossEnemy>();
	//プレイヤーの初期化
	players_[SINGLE_PLAY]->Init();
	//プレイヤーのパラメータをボス側に渡す
	bossEnemy_->SetEnemyPosition(&(players_[SINGLE_PLAY]->GetPlayerPos()));
	bossEnemy_->MakeSpMoveObjects();
	//UIの生成
	CreateSingleUI();

}

void ActorManager::Update()
{
	for (auto& player : players_)
	{
		player->Update();
	}
	(this->*_update)();
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


void ActorManager::AddClliders(Collider* collider)
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
		//わざとップレイヤーが当たっていたらプレイヤーをダウン状態にしてHPを減らす
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
		if (0.0f>= player->GetPlayerHP() || StageOut(player->GetPlayerPos()))
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
	if (0.0f >= players_[SINGLE_PLAY]->GetPlayerHP() || StageOut(players_[SINGLE_PLAY]->GetPlayerPos()))
	{	//敗北状態に移行
		players_[SINGLE_PLAY]->Lose();
		isDeadAnyPlayer_ = true;
	}
	else if(0.0f >= bossEnemy_->GetPlayerHP())
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

bool ActorManager::IsEqual(int playerNum, int damageNum)
{	//攻撃する側タイプと攻撃される側タイプが同じかどうか
	if (playerNum == damageNum)
	{
		return true;
	}
	return false;
}

void ActorManager::CreateBattleUI(void)
{
	for (int idx = 0; idx < PLAYER_NUM_MAX; idx++)
	{
		// 自分
		auto& me = players_[idx];

		//敵プレイヤー側の要素番号を切り替える
		int targetIdx = (idx + 1) % PLAYER_NUM_MAX;

		//敵プレイヤーを取得
		auto& target = players_[targetIdx];

		//敵プレイヤーの座標、HP、ステートを設定
		players_[idx]->SetEnemyPosition(&(target->GetTransform().pos));
		players_[idx]->SetEnemyHp(&(target->GetPlayerHP()));
		players_[idx]->SetEnemyState(&(target->pState_));

		userInterfaces_.emplace_back(
			std::make_unique<UserInterface>(
				ResourceManager::GetInstance(),
				target->GetTransform().pos,
				me->GetEnemyDistance(),
				me->GetBoostGauge(),
				me->GetPlayerHP(),
				target->GetPlayerHP(),
				me->IsWin(),
				me->GetNumnberOfBullets(),
				playMode_,
				idx)
				);
	}
}

void ActorManager::CreateSingleUI(void)
{
	// 自分
	auto& me = players_[SINGLE_PLAY];

	//敵プレイヤーの座標、HP、ステートを設定
	me->SetEnemyPosition(& bossEnemy_->GetBossEnemyPos());
	me->SetEnemyHp(&bossEnemy_->GetPlayerHP());
	me->SetEnemyState(nullptr);

	userInterfaces_.emplace_back(
		std::make_unique<UserInterface>(
			ResourceManager::GetInstance(),
			bossEnemy_->GetBossEnemyPos(),
			me->GetEnemyDistance(),
			me->GetBoostGauge(),
			me->GetPlayerHP(),
			bossEnemy_->GetPlayerHP(),
			me->IsWin(),
			me->GetNumnberOfBullets(),
			playMode_,
			SINGLE_PLAY)
	);
}
