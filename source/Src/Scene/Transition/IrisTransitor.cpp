#include <math.h>
#include <DxLib.h>
#include "../../Object/Time/DeltaTime.h"
#include "../../Utility/AsoUtility.h"
#include "IrisTransitor.h"

IrisTransitor::IrisTransitor(bool irisOut, float interval, bool isTiled, int gHandle) 
	:Transitor(interval),irisOut_(irisOut),isTiled_(isTiled),gHandle_(gHandle)
{
	//ウィンドウサイズ
	VECTOR screenSize = { 1600.0f,1000.0f };
	int X = static_cast<int>(screenSize.x);
	int Y = static_cast<int>(screenSize.y);

	//マスクレイヤーの作成
	handleForMaskScreen_ = MakeScreen(X, Y, true);
	maskH_ = CreateMaskScreen();

	//ウィンドウの対角線の長さ
	diagonalLength_ = hypotf(screenSize.x, screenSize.y) / 2.0f;

	imgMaskHandle_ = LoadGraph("Data/Image/Ui/X.png");
	imgFrameHandle_ = LoadGraph("Data/Image/Ui/X_Frame.png");
	angle_ = 0.0f;
}

IrisTransitor::~IrisTransitor()
{
	DeleteMaskScreen();
}

void IrisTransitor::Update()
{
	if (frame_ < interval_) 
	{
		frame_ += 120.0f* DeltaTime::GetInstsnce().GetDeltaTime();
		//SetDrawScreen(newRT_);
		mainScreen_ = newRT_;
	}
	else if (frame_ >= interval_) 
	{
		//SetDrawScreen(DX_SCREEN_BACK);
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

	const float SPEED_POW = 2.0f;
	const float IMG_SCALE = 15.0f;
	const float IMG_SCALE_STOP = 1.4f;
	//const float FRAME_RATE_1ST_SCALING = 0.45f;
	//const float FRAME_RATE_2ST_STOP = 0.75f;
	const float FRAME_RATE_1ST_SCALING = 0.25f;
	const float FRAME_RATE_2ST_STOP = 0.55f;

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
	//DrawCircleAA(pos.x, pos.y, radius, 32, 0xffffff, true);
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
