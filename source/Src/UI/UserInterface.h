#pragma once
#include"../Input/Input.h"
#include<map>

class ResourceManager;

class UserInterface
{
public:

	UserInterface(
		int playMode,
		int playerType,
		const float& distance,
		const int& bulletNum,
		const float& boostGauge, 
		const float& hpGauge, 
		const bool& isWin,
		const VECTOR& enemyPos,
		const float& enemyHp);

	~UserInterface(void);

	void Draw(bool& gameSet);

	//共通UI(ゲームスタート等)
	void DrawCommonUI(const float& startCount, const bool& isGameSet, const int& rematchMode);

private:

	static constexpr  int MAX_HP_IMAGE_NUM = 10;

	//画像タイプ
	enum class IMG_TYPE
	{
		BOOST_GAUGE,
		BOOST_GAUGE_CASE,
		ENEMY_HP_GAUGE,
		WIN,
		LOSE,
		TARGERT,
		TARGET_RED,
		RIFLE_IMAGE,
		ONE_MOR_FIGHT,
		BACK_TO_TITLE,
		PLEASE_CROSS,
		PLEASE_A,
		TRIANGLE,
		READY,
		FIGHT

	};

	// シングルトン参照
	ResourceManager& resMng_;

	//UI画像ハンドル
	std::map<IMG_TYPE, int> uiImgH_;

	int* hpImg_;

	//ブーストゲージの数値
	const float& boostGauge_;

	//HPの数値
	const float& hpGauge_;

	float hpScale_;

	float hpOffset_;

	//敵HPの数値
 	const float& enemyHpGauge_;

	//残弾数
	const int * numnberofBullets_;

	//敵との距離
	const float& enemyDistance_;

	//プレイモード
	int playMode_;

	//Hp表示
	int fontHp_;

	//残弾数表示表示
	int fontBullets_;

	//プレイヤーが勝利したか敗北したかの判定
	const bool& isWin_;

	//勝利、敗北画像の座標
	VECTOR victoryOrDefeatPos_;

	//ブーストゲージ画像の座標
	VECTOR boostGaugePos_;

	//ブーストゲージ画像座標調整用
	int boostGaugeOffsetX_;
	int boostGaugeOffsetY_;

	//ブーストゲージ画像の座標
	double boostGaugeCaseScale_;

	//HPの座標
	VECTOR hpGaugePos_;

	//残弾表示画像の座標
	VECTOR numberOfBulletsPos_;

	//残弾表示画像の大きさ
	double numberOfBulletsScale_;

	//プレイヤー１のパッドの種類
	JOYPAD_TYPE joyPadType_;

	//再戦モード
	int rematchMode_;

	//敵の座標
	const VECTOR& enemyPos_;

	//スタート計測変数
	float startCount_;

	//画像の読み込み
	void InitImage(void);

	//ゲームスタート表示などの共通UIの初期化
	void InitCommonUI(void);

	//ブーストゲージの表示
	void DrawBoostGauge(void);

	//残弾数の表示
	void DrawNumnberOfBullets(void);

	//HPゲージの表示
	void DrawHPGauge(void);

	//WINもしくはLOSEを表示する
	void VictoryOrDefeat(void);

	//敵へのターゲット表示
	void DrawTarget(void);

	//敵のHP表示
	void DrawEnemyHp(void);

	//ゲームスタート時の表示
	void DrawGameUIStart(const float& startCount);
	
	//ゲーム終了時の表示
	void DrawUIGameSet(const bool& isGameSet, const int& rematchMode);

};

