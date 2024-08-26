#pragma once
#include<map>

class ResourceManager;

class UserInterface
{
public:

	UserInterface(ResourceManager& resMng, const VECTOR* enemyPos,float& distance,
		float& boostGauge, float& hpGauge, const float& enemyHp, bool& IsWin, const int& numnberofBullets,int SinglPlay);

	~UserInterface();
	void Update(void);

	void Draw(bool& gameSet);

private:

	enum class IMG_H
	{
		BOOST_GAUGE,
		BOOST_GAUGE_CASE,
		ENEMY_HP_GAUGE,
		WIN,
		LOSE,
		TARGERT,
		TARGET_RED,
		RIFLE_IMAGE
	};

	// シングルトン参照
	ResourceManager& resMng_;

	//UI画像ハンドル
	std::map<IMG_H,int> uiImgH_;

	//ブーストゲージの数値
	float& boostGauge_;

	//HPの数値
	float& hpGauge_;

	//敵HPの数値
 	const float& enemyHpGauge_;

 	 float enemyHpGaugeCause_;

	//残弾数
	const int& numnberofBullets_;

	//敵との距離
	float& enemyDistance_;

	int singlePlay_;

	//Hp表示
	int fontHp_;

	//残弾数表示表示
	int fontBullets_;

	//プレイヤーが勝利したか敗北したかの判定
	bool& IsWin_;

	//勝利、敗北画像の座標
	VECTOR victoryOrDefeatPos_;

	//ブーストゲージ画像の座標
	VECTOR boostGaugePos_;

	int boostGaugeOffsetX_;
	int boostGaugeOffsetY_;

	//ブーストゲージ画像の座標
	double boostGaugeCaseScale_;

	//HPの座標
	VECTOR HpGaugePos_;

	//残弾表示画像の座標
	VECTOR numberOfBulletsPos_;

	//残弾表示画像の座標
	double numberOfBulletsScale_;


	//敵の座標
	const VECTOR* enemyPos_;

	void InitImage(void);

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

};

