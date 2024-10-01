#pragma once
#include"../Object/Actor/Enemy/BossEnemy.h"
#include"../Object/Actor/Player/Player.h"
#include<memory>
#include<vector>
#include<DxLib.h>

class Collider;
class DamageObject;
class UserInterface;

class ActorManager
{
public:


	ActorManager(int numberofPlayers);

	void Update();
	void UpdateBattleMode(void);
	void UpdateSingleMode(void);

	void Draw(void);
	void DrawBattleMode(void);
	void DrawSingleMode(void);
	void DrawDebug(int playerType);
	void DrawAnimeDebug(int playerType);
	void DrawCamera(int playerType);
	//残弾数やHPゲージ等の表示
	void DrawUI(int playerType);

	//ゲームスタートなどの共通UI
	void DrawCommonUI(const float&startCount, const bool& isGameSet,const float& rematchMode);

	void AddClliders(Collider* collider);
	void SetEnemyInfo(void);
	void SetBossEnemyInfo(void);
	void DamageObjectInit(void);
	void DamegeBeamCollision(void);
	void DamegeSwordCollision(void);
	void DmageBossSpMoveCollision(void);
	void DmageBossBeamCollision(void);
	void InitUI(void);
	const bool& IsDeadAnyPlayer(void)const;

private:

	void (ActorManager::* _update)();

	void (ActorManager::* _draw)();

	static constexpr int PLAYER_NUM_MAX = 2;

	static constexpr int PLAYER_NUM_MIN = 1;

	static constexpr int SINGLE_PLAY = 0;


	static constexpr float STAGE_OUT_Y = -1000.0f;

	std::vector< std::unique_ptr<Player>>players_;

	//ビームライフルやビームサーベル
	std::vector<std::unique_ptr<DamageObject>>damageObject_;

	//HPやブーストゲージの表示
	std::vector< std::unique_ptr<UserInterface>>userInterface_;

	//一人用モード敵
	std::unique_ptr<BossEnemy> bossEnemy_;

	//どちらかのプレイヤーが死んだかどうかを判定
	bool isDeadAnyPlayer_;

	int loserNum_;

	int winnerNum_;
	void IsWin(void);

	void IsSingleModeWin(void);

	void InitBattleMode(int numberofPlayers);

	void InitSIngleMode(int numberofPlayers);

	bool StageOut(const VECTOR& pos);

	void ChangeStateGameSet(void);

	bool IsEqual(int playerNum, int damageNum);
};

