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
	void DrawCommonUI(const float&startCount, const bool& isGameSet,const int& rematchMode);

	//コリジョンの追加
	void AddClliders(Collider* collider);	
	
	//弾との当たりは判定
	void DamegeShotCollision(void);
	
	//剣との当たり判定
	void DamegeSwordCollision(void);
	
	//ボスの攻撃との当たり判定
	void DmageBossSpMoveCollision(void);

	//ボスと弾との当たり判定
	void DmageBossBeamCollision(void);

	//いずれかのプレイヤーが死んだかどうかを返す
	const bool& IsDeadAnyPlayer(void)const;

	//デバッグ用
	void DrawDebug(int playerType);
	void DrawAnimeDebug(int playerType);

private:

	void (ActorManager::* _update)();

	std::vector<std::unique_ptr<Player>> players_;

	//ビームライフルやビームサーベル
	std::vector<std::unique_ptr<DamageObject>> damageObject_;

	//HPやブーストゲージの表示
	std::vector<std::unique_ptr<UserInterface>> userInterfaces_;

	//一人用モード敵
	std::unique_ptr<BossEnemy> bossEnemy_;

	//どちらかのプレイヤーが死んだかどうかを判定
	bool isDeadAnyPlayer_;

	int loserPlayerType_;

	int winnerPlayerType_;

	//プレイモード
	int playMode_;

	//勝者が決まったかどうかを判定
	void IsWin(void);

	//勝者が決まったかどうかを判定(シングルモード)
	void IsSingleModeWin(void);

	//対戦モード初期化
	void InitBattleMode(int numberofPlayers);

	//シングルモード初期化
	void InitSIngleMode(int numberofPlayers);

	//場外に落ちたかどうか
	bool StageOut(const VECTOR& pos);

	//各プレイヤーを勝利、敗北状態に変える
	void ChangeStateGameSet(void);

	//
	bool IsEqual(int playerNum, int damageNum);

	//対戦モードのUI
	void CreateBattleUI(void);
	//シングルモードのUI
	void CreateSingleUI(void);

};
