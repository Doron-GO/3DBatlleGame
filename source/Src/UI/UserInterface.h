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
		FIGHT,
		HP_BACK,
		BULLET_NUM

	};

	struct Info
	{
		VECTOR pos_;
		double scale_;
		int centerX_;
		int centerY_;
		float offset_;
	};

	//画像のパラメータの配列
	std::map<IMG_TYPE, Info> imgInfo_;

	// シングルトン参照
	ResourceManager& resMng_;

	//UI画像ハンドル
	std::map<IMG_TYPE, int> uiImgH_;

	//数字画像
	int* numberImg_;

	//ブーストゲージの数値
	const float& boostGauge_;
	//HPの数値
	const float& hpGauge_;

	//HP数字の大きさ
	float hpValueScale_;
	//HP数字のオフセット
	int hpValueOffset_;

	//敵HPの数値
 	const float& enemyHpGauge_;

	//残弾数
	const int * numnberofBullets_;

	//敵との距離
	const float& enemyDistance_;

	//プレイモード
	int playMode_;

	//プレイヤーが勝利したか敗北したかの判定
	const bool& isWin_;

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

