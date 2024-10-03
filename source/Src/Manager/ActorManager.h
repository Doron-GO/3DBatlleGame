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
	
	//アップデート
	void Update();

	//対戦モードアップデート
	void UpdateBattleMode(void);
	//シングルモードアップデート
	void UpdateSingleMode(void);

	//描画
	void Draw(void);

	//カメラ描画
	void DrawCamera(int playerType);
	
	//残弾数やHPゲージ等の表示
	void DrawUI(int playerType);

	//ゲームスタートなどの共通UI
	void DrawCommonUI(const float&startCount, const bool& isGameSet,const float& rematchMode);

	//コリジョンの追加
	void AddClliders(Collider* collider);
	
	//
	void SetEnemyInfo(void);
	
	
	void SetBossEnemyInfo(void);
	
		
	
	void DamegeBeamCollision(void);
	
	
	void DamegeSwordCollision(void);
	
	
	void DmageBossSpMoveCollision(void);
	void DmageBossBeamCollision(void);
	void InitUI(void);
	const bool& IsDeadAnyPlayer(void)const;

	//デバッグ用
	void DrawDebug(int playerType);
	void DrawAnimeDebug(int playerType);

private:

	void (ActorManager::* _update)();

	std::vector< std::unique_ptr<Player>>players_;

	//ビームライフルやビームサーベル
	std::vector<std::unique_ptr<DamageObject>>damageObject_;

	//HPやブーストゲージの表示
	std::vector< std::unique_ptr<UserInterface>>userInterface_;

	//一人用モード敵
	std::unique_ptr<BossEnemy> bossEnemy_;

	//どちらかのプレイヤーが死んだかどうかを判定
	bool isDeadAnyPlayer_;

	int loserPlayerType_;

	int winnerPlayerType_;

	//プレイモード
	int playMode_;

	void IsWin(void);

	void IsSingleModeWin(void);

	void InitBattleMode(int numberofPlayers);

	void InitSIngleMode(int numberofPlayers);

	bool StageOut(const VECTOR& pos);

	void ChangeStateGameSet(void);

	bool IsEqual(int playerNum, int damageNum);
};

