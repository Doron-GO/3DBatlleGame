#include <math.h>
#include <DxLib.h>
#include "../../Object/Time/DeltaTime.h"
#include"../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "IrisTransitor.h"


const float DELTATIME_RATE = 120.0f;

IrisTransitor::IrisTransitor(bool irisOut, float interval, bool isTiled, int gHandle) 
	:Transitor(interval),irisOut_(irisOut),isTiled_(isTiled),gHandle_(gHandle), 
	resMng_(ResourceManager::GetInstance())
{
	//ウィンドウサイズ
	VECTOR screenSize = { 1600.0f,1000.0f };
	int X = static_cast<int>(screenSize.x);
	int Y = static_cast<int>(screenSize.y);

	//マスクレイヤーの作成
	handleForMaskScreen_ = MakeScreen(X, Y, true);
	//マスクハンドル
	maskH_ = CreateMaskScreen();

	//ウィンドウの対角線の長さ
	diagonalLength_ = hypotf(screenSize.x, screenSize.y) / 2.0f;

	//トランジション画像
	imgFrameHandle_ = resMng_.Load(ResourceManager::SRC::TRANSITION_FRAME).handleId_;

	//トランジション枠画像
	imgMaskHandle_ = resMng_.Load(ResourceManager::SRC::TRANSITION).handleId_;

	//アングルの初期化
	angle_ = 0.0f;
}

IrisTransitor::~IrisTransitor()
{
	DeleteMaskScreen();
}

void IrisTransitor::Start()
{
	VECTOR size = { 1600,1000 };
	int sizeX = static_cast<int>(size.x);
	int sizeY = static_cast<int>(size.y);

	oldRT_ = MakeScreen(sizeX, sizeY);
	newRT_ = MakeScreen(sizeX, sizeY);

	int result = GetDrawScreenGraph(0, 0, sizeX, sizeY, oldRT_);
	frame_ = 0.0f;
	//アングルの初期化
	angle_ = 0.0f;
}


void IrisTransitor::Update()
{
	if (frame_ < interval_) 
	{
		frame_ += DELTATIME_RATE * DeltaTime::GetInstsnce().GetDeltaTime();
		mainScreen_ = newRT_;
	}
	else if (frame_ >= interval_) 
	{
		mainScreen_ = DX_SCREEN_BACK;
	}
	SetDrawScreen(mainScreen_);
}

void IrisTransitor::Draw()
{
	if (IsEnd()) 
	{
		return;
	}
	auto rate = frame_ / interval_;
	int backRT = oldRT_;
	int maskedRT = newRT_;
	if (irisOut_) {
		backRT = newRT_;
		maskedRT = oldRT_;
		rate = 1.0f - rate;
	}
	//
	float radius = (diagonalLength_)*rate;
	SetDrawScreen(handleForMaskScreen_);
	ClearDrawScreen();

	//
	const float SPEED_POW = 2.0f;
	const float IMG_SCALE = 15.0f;
	//維持状態中の画像の大きさ
	const float IMG_SCALE_STOP = 1.4f;

	//const float FRAME_RATE_1ST_SCALING = 0.45f;
	//const float FRAME_RATE_2ST_STOP = 0.75f;
	
	//等速倍拡大スケール
	const float FRAME_RATE_1ST_SCALING = 0.25f;
	//スケール維持時間のリミット
	const float FRAME_RATE_2ST_STOP = 0.55f;
	//画像の拡大率
	float xRate = 0.0f;

	if (rate < FRAME_RATE_1ST_SCALING)
	{
		// 等速倍
		xRate = rate / FRAME_RATE_1ST_SCALING * IMG_SCALE_STOP;
	}
	else if (rate < FRAME_RATE_2ST_STOP)
	{
		// 維持
		xRate = IMG_SCALE_STOP;
	}
	else
	{
		// 自乗倍
		angle_ += AsoUtility::Deg2RadF(3.0f);
		float scaleMax = pow(rate, SPEED_POW)* IMG_SCALE;
		float scaleRate = (rate - FRAME_RATE_2ST_STOP) / (1.0f - FRAME_RATE_2ST_STOP);
		xRate = AsoUtility::Lerp(
			IMG_SCALE_STOP,
			scaleMax,
			scaleRate);
	}

	VECTOR pos = { 1600.0f / 2.0f, 1000.0f / 2.0f };

	DrawRotaGraph(pos.x, pos.y, xRate, angle_, imgMaskHandle_, true);

	//隠し関数(現在のグラフィックハンドルをマスクスクリーンに転送)
	SetMaskScreenGraph(handleForMaskScreen_);
	//描画領域を反転する
	SetMaskReverseEffectFlag(true);

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseMaskScreenFlag(false);
	DrawGraph(0, 0, backRT, true);
	SetUseMaskScreenFlag(true);
	DrawGraph(0, 0, maskedRT, true);
	SetUseMaskScreenFlag(false);

	DrawRotaGraph(pos.x, pos.y, xRate, angle_, imgFrameHandle_, true);

}
