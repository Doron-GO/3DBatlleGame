#pragma once
#include<functional>
#include<memory>
#include<vector>
#include<DxLib.h>

class Collider;
class DamageObject;
class UserInterface;
class ActorBase;
class Player;
class BossEnemy;

class ActorManager
{
public:


	ActorManager(int playMode);
	
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
	void AddColliders(Collider* collider);	
	
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

	//UIを生成するときに使うゲット関数
	const std::vector<std::unique_ptr<Player>>& GetPlayers(void)const;
	//UIを生成するときに使うゲット関数
	const std::unique_ptr<BossEnemy>& GetBossEnemy(void)const;

	std::vector<std::unique_ptr<UserInterface>>MoveUI(void);
	
	bool& IsDeadAnyPlayer(void);

private:

	//状態管理(更新ステップ)
	std::function<void(void)>update_;

	std::vector<std::unique_ptr<Player>> players_;

	//ビームライフルやビームサーベル
	std::vector<std::unique_ptr<DamageObject>> damageObject_;

	//HPやブーストゲージの表示
	std::vector<std::unique_ptr<UserInterface>> userInterfaces_;

	//一人用モード敵
	std::unique_ptr<BossEnemy> bossEnemy_;

	//どちらかのプレイヤーが死んだかどうかを判定
	bool isDeadAnyPlayer_;

	//敗北プレイヤータイプ
	int loserPlayerType_;

	//勝利プレイヤータイプ
	int winnerPlayerType_;

	//プレイモード
	int playMode_;

	//勝者が決まったかどうかを判定
	void IsWin(void);

	//勝者が決まったかどうかを判定(シングルモード)
	void IsSingleModeWin(void);

	//アクターの初期化
	void InitActor(void);

	//場外に落ちたかどうか
	bool StageOut(const VECTOR& pos);

	//各プレイヤーを勝利、敗北状態に変える
	void ChangeStateGameSet(void);

	//タイプ１とタイプ２が同タイプかどうかを判定
	bool IsEqual(int Type1, int Type2);

	//敵の情報をセットする
	void SetEnemyInfo(ActorBase& player, ActorBase& target);


};
