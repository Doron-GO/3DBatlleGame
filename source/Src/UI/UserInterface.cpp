#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "UserInterface.h"
#include"../../Config.h"

#pragma region Parameter

//シングルモード時の勝利または敗北画像の座標
 constexpr VECTOR SINGLEMODE_IMG_POS = { 800.0f,400.0f ,0.0f };

 constexpr VECTOR MULTI_MODE_IMG_POS = { 400.0f,400.0f ,0.0f };

//敵ターゲット画像の座標調整用
 constexpr float TARGET_OFFSET_Y = 100.0f;

//敵ターゲット画像の座標調整用
 constexpr float ENEMY_HP_GAUGE_OFFSET_Y = 400.0f;

 //ブーストゲージ画像の座標
 constexpr VECTOR BOOST_GAUGE_IMG_POS_MULTI= { 400.0f,850.0f,0.0f };

 constexpr VECTOR BOOST_GAUGE_IMG_POS_SINGLE = { 1100.0f,850.0f,0.0f };

 constexpr double BOOST_GAUGE_CASE_SCALE_SINGLE= 2.5;

 constexpr double BOOST_GAUGE_CASE_SCALE_MULTI= 2.0;

 constexpr int BOOST_GAUGE_OFFSET_X_SINGLE = 5;
 constexpr int BOOST_GAUGE_OFFSET_Y_SINGLE = 40;

 constexpr int BOOST_GAUGE_OFFSET_X_MULTI = 4;
 constexpr int BOOST_GAUGE_OFFSET_Y_MULTI = 30;

 constexpr VECTOR HP_GAUGE_POS_MULTI = {25.0f,800.0f,0.0f};
 constexpr VECTOR HP_GAUGE_POS_SINGLE = {4.0f,750.0f,0.0f};

 constexpr VECTOR NUMBER_OF_BULLETS_POS_SINGLE = {1300.0f,650.0f,0.0f};
 constexpr VECTOR NUMBER_OF_BULLETS_POS_MULTI = {600.0f,700.0f,0.0f};
 constexpr int NUMBER_OF_BULLETS_CENTER_X= 140;
 constexpr int NUMBER_OF_BULLETS_CENTER_Y=100;
 constexpr int NUMBER_OF_BULLETS_OFFSET = 10;
 constexpr double NUMBER_OF_BULLETS_SCALE_MULTI = 0.4;
 constexpr double NUMBER_OF_BULLETS_SCALE_SINGLE= 0.6;

#pragma endregion


UserInterface::UserInterface(ResourceManager& resMng, const VECTOR* enemyPos, float& distance,
	float& boostGauge, float& hpGauge, const float& enemyHp, bool& IsWin_, const int& numnberofBullets, int SinglPlay):
	resMng_(resMng), enemyPos_(enemyPos), enemyDistance_(distance), 
	boostGauge_(boostGauge), hpGauge_(hpGauge),enemyHpGauge_(enemyHp), IsWin_(IsWin_), numnberofBullets_(numnberofBullets)
{
	singlePlay_ = SinglPlay;
	InitImage();
	enemyHpGaugeCause_ = enemyHpGauge_;

}

UserInterface::~UserInterface()
{
}

void UserInterface::Update(void)
{

}

void UserInterface::Draw(bool& gameSet)
{
	if (!gameSet)
	{
		DrawBoostGauge();
		DrawHPGauge();
		DrawTarget();
		DrawNumnberOfBullets();
	}
	else
	{
		VictoryOrDefeat();
	}
}

void UserInterface::InitImage(void)
{
	uiImgH_.emplace(IMG_H::WIN, resMng_.Load(ResourceManager::SRC::WIN).handleId_);
	uiImgH_.emplace(IMG_H::LOSE, resMng_.Load(ResourceManager::SRC::LOSE).handleId_);
	uiImgH_.emplace(IMG_H::TARGERT, resMng_.Load(ResourceManager::SRC::TARGET).handleId_);
	uiImgH_.emplace(IMG_H::TARGET_RED, resMng_.Load(ResourceManager::SRC::TARGET_RED).handleId_);
	uiImgH_.emplace(IMG_H::RIFLE_IMAGE, resMng_.Load(ResourceManager::SRC::RIFLE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_H::BOOST_GAUGE, resMng_.Load(ResourceManager::SRC::BOOSTGAUGE_IMAGE).handleId_);
	uiImgH_.emplace(IMG_H::ENEMY_HP_GAUGE, resMng_.Load(ResourceManager::SRC::ENEMY_HP_GAUGE).handleId_);
	uiImgH_.emplace(IMG_H::BOOST_GAUGE_CASE, resMng_.Load(ResourceManager::SRC::BOOSTGAUGE_CASE_IMAGE).handleId_);

	if (singlePlay_ == 0)
	{
		HpGaugePos_ = HP_GAUGE_POS_SINGLE;
		victoryOrDefeatPos_ = SINGLEMODE_IMG_POS;
		boostGaugePos_ = BOOST_GAUGE_IMG_POS_SINGLE;
		boostGaugeCaseScale_ = BOOST_GAUGE_CASE_SCALE_SINGLE;
		boostGaugeOffsetX_ = BOOST_GAUGE_OFFSET_X_SINGLE;
		boostGaugeOffsetY_ = BOOST_GAUGE_OFFSET_Y_SINGLE;
		numberOfBulletsPos_ = NUMBER_OF_BULLETS_POS_SINGLE;	
		fontHp_ = CreateFontToHandle(NULL, 250, 3);
		fontBullets_ = CreateFontToHandle(NULL, 150, 3);
		numberOfBulletsScale_ = NUMBER_OF_BULLETS_SCALE_SINGLE;
	}
	else
	{
		HpGaugePos_ = HP_GAUGE_POS_MULTI;
		victoryOrDefeatPos_ = MULTI_MODE_IMG_POS;
		boostGaugePos_ = BOOST_GAUGE_IMG_POS_MULTI;
		boostGaugeCaseScale_ = BOOST_GAUGE_CASE_SCALE_MULTI;
		boostGaugeOffsetX_ = BOOST_GAUGE_OFFSET_X_MULTI;
		boostGaugeOffsetY_ = BOOST_GAUGE_OFFSET_Y_MULTI;
		numberOfBulletsPos_ = NUMBER_OF_BULLETS_POS_MULTI;
		fontHp_ = CreateFontToHandle(NULL, 150, 3);
		fontBullets_ = CreateFontToHandle(NULL, 100, 3);
		numberOfBulletsScale_ = NUMBER_OF_BULLETS_SCALE_MULTI;
	}

}

void UserInterface::DrawBoostGauge(void)
{
	int x = static_cast<int>(boostGauge_);
	int PosX = static_cast<int>(boostGaugePos_.x);
	int PosY = static_cast<int>(boostGaugePos_.y);
	int color = 0;
	if (x>50)
	{
		color = GetColor(0, 0, 255);
	}
	else if (x>25)
	{
		color = GetColor(255, 255, 0);
	}
	else
	{
		color = GetColor(255, 0, 0);
	}

	DrawFillBox(PosX, PosY, PosX+ (x * boostGaugeOffsetX_), PosY + boostGaugeOffsetY_, color);
	DrawRotaGraph2(PosX, PosY, 0, 0, boostGaugeCaseScale_, 0.0, uiImgH_[IMG_H::BOOST_GAUGE_CASE], true, false);
}

void UserInterface::DrawNumnberOfBullets(void)
{
	int PosX = static_cast<int>(numberOfBulletsPos_.x);
	int PosY = static_cast<int>(numberOfBulletsPos_.y);
	int geuge = static_cast<int>(numnberofBullets_);

	DrawFormatStringToHandle(PosX, PosY - NUMBER_OF_BULLETS_OFFSET, 0xffffff, fontBullets_, "%d", static_cast<int>(numnberofBullets_));
	DrawRotaGraph2(PosX, PosY, NUMBER_OF_BULLETS_CENTER_X, NUMBER_OF_BULLETS_CENTER_Y, 
		numberOfBulletsScale_, 0.0, uiImgH_[IMG_H::RIFLE_IMAGE], true, false);

}

void UserInterface::DrawHPGauge(void)
{	
	int PosX = static_cast<int>(HpGaugePos_.x);
	int PosY = static_cast<int>(HpGaugePos_.y);

	DrawFormatStringToHandle(PosX, PosY, 0xff0000, fontHp_, "%d", static_cast<int>(hpGauge_));
}

void UserInterface::VictoryOrDefeat(void)
{
	int posX = static_cast<int>(victoryOrDefeatPos_.x);
	int posY = static_cast<int>(victoryOrDefeatPos_.y);
	if (IsWin_)
	{
		DrawRotaGraph2(posX, posY, 140, 100, 2.0, 0.0, uiImgH_[IMG_H::WIN], true, false);
	}
	else
	{
		DrawRotaGraph2(posX, posY, 140, 100, 2.0, 0.0, uiImgH_[IMG_H::LOSE],  true, false);
	}
}

void UserInterface::DrawTarget(void)
{
	VECTOR pos = *(enemyPos_);
	VECTOR TargetPos = pos;
	TargetPos.y += TARGET_OFFSET_Y;
	TargetPos = ConvWorldPosToScreenPos(TargetPos);
	int X = static_cast<int>(TargetPos.x);
	int Y = static_cast<int>(TargetPos.y);

	if (WITHIN_RANGE > (enemyDistance_))
	{
		DrawRotaGraph(X, Y,  0.2f, 0.0f, uiImgH_[IMG_H::TARGERT], true);
	}
	else
	{
		DrawRotaGraph(X, Y, 0.2f, 0.0f, uiImgH_[IMG_H::TARGET_RED], true);
	}

	DrawEnemyHp();
}

void UserInterface::DrawEnemyHp(void)
{
	int scale = 0;
	if (singlePlay_==0)
	{
		scale = 50;
	}
	else
	{
		scale = 10;
	}
	VECTOR enemyHpPos = *(enemyPos_);
	enemyHpPos.y += ENEMY_HP_GAUGE_OFFSET_Y;
	enemyHpPos = ConvWorldPosToScreenPos(enemyHpPos);
	int X = static_cast<int>(enemyHpPos.x);
	int Y = static_cast<int>(enemyHpPos.y);
	int geuge = static_cast<int>((enemyHpGauge_ * 2) / scale);
	int geuge_cause = static_cast<int>((enemyHpGaugeCause_ * 2) / scale);

	DrawRectGraph(X, Y, 0, 0, geuge_cause, 15, uiImgH_[IMG_H::BOOST_GAUGE_CASE], true);

	DrawRectGraph(X, Y, 0, 0, geuge, 15, uiImgH_[IMG_H::ENEMY_HP_GAUGE], true);

}
