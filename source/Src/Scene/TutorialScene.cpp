#include "TutorialScene.h"
#include"../Manager/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include"Transition/Transitor.h"
#include"../Input/Input.h"
#include"../../Config.h"
#include"GameScene.h"
#include<DxLib.h>

#pragma region パス

//UI説明画像
std::string IMAGE_UI = PATH_IMAGE_TUTORIAL + "UI.png";
//ジャンプ説明画像
std::string IMAGE_JUMP = PATH_IMAGE_TUTORIAL + "Tutorial_Jump.png";
//射撃説明画像
std::string IMAGE_SHOT = PATH_IMAGE_TUTORIAL + "Tutorial_Shot.png";
//ブースト説明画像
std::string IMAGE_BOOST = PATH_IMAGE_TUTORIAL + "Tutorial_Boost.png";
//ブーストダッシュ説明画像
std::string IMAGE_BOOSTDASH = PATH_IMAGE_TUTORIAL + "Tutorial_BoostDash.png";
//ホーミング説明画像
std::string IMAGE_HORMING = PATH_IMAGE_TUTORIAL + "Tutorial_Horming.png";
//射程距離内説明画像
std::string IMAGE_RANGE_IN = PATH_IMAGE_TUTORIAL + "Tutorial_InRange.png";
//射程外説明画像
std::string IMAGE_RANGE_OUT = PATH_IMAGE_TUTORIAL + "Tutorial_OutRange.png";
//ブーストホーミング説明画像
std::string IMAGE_HORMING_BOOST = PATH_IMAGE_TUTORIAL + "Tutorial_Boost_Horming.png";
//ブーストダッシュホーミング説明画像
std::string IMAGE_HORMING_BOOSTDASH = PATH_IMAGE_TUTORIAL + "Tutorial_BoostDash_Horming.png";
//格闘説明画像
std::string IMAGE_COMBAT = PATH_IMAGE_TUTORIAL + "Tutorial_Combat .png";
//格闘キャンセル画像
std::string IMAGE_COMBAT_CANCEL= PATH_IMAGE_TUTORIAL + "Tutorial_Combat_Cancel.png";
//その場格闘説明画像
std::string IMAGE_COMBAT_QUICK = PATH_IMAGE_TUTORIAL + "Tutorial_Combat_Quick.png";
//格闘移行説明画像
std::string IMAGE_COMBAT_TRANSITION = PATH_IMAGE_TUTORIAL + "Tutorial_CombatTransition.png";

#pragma endregion

#pragma region パラメータ

//最大のページ数
constexpr int PAGE_MAX = 14;
//最小のページ数
constexpr int PAGE_MIN = 0;

//最大のカーソルナンバー
constexpr int CURSOR_MAX = 1;

//選択カーソル画像の数値(座標、中心座標、大きさ)
constexpr int TRIANGLE_POS_X = 1200;
constexpr int TRIANGLE_POS_Y = 300;
constexpr int TRIANGLE_CENTER_X = 280;
constexpr int TRIANGLE_CENTER_Y = 63;
constexpr int TRIANGLE_OFFSET_MODE_SELECT = 160;
constexpr double TRIANGLE_SCALE = 1.2;


#pragma endregion



TutorialScene::TutorialScene(SceneManager& manager, int playMode, Transitor& transit, Input& input) :Scene(manager, transit, input),
resMng_(ResourceManager::GetInstance())
{
	playMode_ = playMode;
	//チュートリアルの初期ページ
	pageIdx_ = 0;
	//画像の初期化
	InitImage();
	//説明文の初期化
	InitExplanation();
	//使用するフォントを読み込む
	explanationHandle_ = CreateFontToHandle(FONT_NAME.c_str(), 70, 3);
	//使用するフォントを読み込む
	nextHandle_ = CreateFontToHandle(FONT_NAME.c_str(), 130, 3);

	//カーソルの初期化
	cursorIdx_ = 1;

	sceneTransitor_.Start();

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update(void)
{
	input_.Update();


	if (pageIdx_ < PAGE_MAX)
	{
		if (input_.IsTriggerd("right") && pageIdx_ < PAGE_MAX)
		{
			pageIdx_++;
		}
		else if (input_.IsTriggerd("left") && pageIdx_ > PAGE_MIN)
		{
			pageIdx_--;
		}
	}
	//カーソルを移動する
	SelectCursor();
	//次のシーンに行くかを判断する
	if (SelectTutorialOrGame())
	{
		return;
	}

	sceneTransitor_.Update();
}

void TutorialScene::Draw(void)
{
	int SCX = static_cast<int>(SCREEN_SIZE.x);
	int SCY = static_cast<int>(SCREEN_SIZE.y);
	int FONT_Y = 610;
	
	//背景描画
	DrawBox(0, 0,SCX, SCY , 0x7d7d7d, true);

	if (pageIdx_ < PAGE_MAX)
	{
		//説明画像描画
		DrawRotaGraph2(SCX / 2, SCY / 3,
			863, 540,0.5,0.0,imgType_[static_cast<PAGE_TYPE>(pageIdx_)], true);
		//説明文
		DrawStringToHandle(SCY - (SCX / 2), FONT_Y,
			explanationType_[static_cast<PAGE_TYPE>(pageIdx_)].c_str(),
			GetColor(255, 255, 255), explanationHandle_);
	}
	else
	{
		DrawStringToHandle(460, 400,
			explanationType_[static_cast<PAGE_TYPE>(pageIdx_)].c_str(),
			GetColor(255, 255, 255), nextHandle_);

		//カーソル選択画像
		DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET_MODE_SELECT *cursorIdx_),
			TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, cursorImg_, true, false);

	}

	sceneTransitor_.Draw();
}

void TutorialScene::InitImage(void)
{

	imgType_.emplace(PAGE_TYPE::UI, LoadGraph(IMAGE_UI.c_str()));
	imgType_.emplace(PAGE_TYPE::JUMP, LoadGraph(IMAGE_JUMP.c_str()));
	imgType_.emplace(PAGE_TYPE::SHOT, LoadGraph(IMAGE_SHOT.c_str()));
	imgType_.emplace(PAGE_TYPE::RANGE_IN, LoadGraph(IMAGE_RANGE_IN.c_str()));
	imgType_.emplace(PAGE_TYPE::RANGE_OUT, LoadGraph(IMAGE_RANGE_OUT.c_str()));
	imgType_.emplace(PAGE_TYPE::HORMING, LoadGraph(IMAGE_HORMING.c_str()));
	imgType_.emplace(PAGE_TYPE::BOOST, LoadGraph(IMAGE_BOOST.c_str()));
	imgType_.emplace(PAGE_TYPE::BOOST_DASH, LoadGraph(IMAGE_BOOSTDASH.c_str()));
	imgType_.emplace(PAGE_TYPE::HORMING_BOOST, LoadGraph(IMAGE_HORMING_BOOST.c_str()));
	imgType_.emplace(PAGE_TYPE::HORMING_DASH, LoadGraph(IMAGE_HORMING_BOOSTDASH.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT_TRANSITION, LoadGraph(IMAGE_COMBAT_TRANSITION.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT_QUICK, LoadGraph(IMAGE_COMBAT_QUICK.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT_CANCEL, LoadGraph(IMAGE_COMBAT_CANCEL.c_str()));
	imgType_.emplace(PAGE_TYPE::COMBAT, LoadGraph(IMAGE_COMBAT.c_str()));
	
	cursorImg_ = resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_;
}

void TutorialScene::InitExplanation(void)
{
	explanationType_.emplace(PAGE_TYPE::UI,"①自分のHP②敵のHP\n③ターゲット表示\n④残弾数\n⑤ブーストゲージ");
	explanationType_.emplace(PAGE_TYPE::JUMP,"XBOX:Aボタン PSコン:□ボタン\n押し続けるとブーストゲージが尽きるまで上がるぞ");
	explanationType_.emplace(PAGE_TYPE::SHOT,"XBOX:Xボタン PSコン:〇ボタン\n相手に向かって弾を発射");
	explanationType_.emplace(PAGE_TYPE::RANGE_IN,"ターゲット表示が白なら相手を追尾する");
	explanationType_.emplace(PAGE_TYPE::RANGE_OUT,"ターゲット表示が赤なら追尾しない");
	explanationType_.emplace(PAGE_TYPE::HORMING,"追尾する弾は通常移動じゃ避けられない");
	explanationType_.emplace(PAGE_TYPE::BOOST,"XBOX:Aボタン PSコン:□ボタン\nを素早く二回押す\nブーストゲージを消費して高速移動する");
	explanationType_.emplace(PAGE_TYPE::BOOST_DASH,"ブースト後にボタンを押し続けるとブーストゲージが\n尽きるまでブーストダッシュができる");
	explanationType_.emplace(PAGE_TYPE::HORMING_BOOST,"敵の弾の追尾はブーストで振り切ることができる");
	explanationType_.emplace(PAGE_TYPE::HORMING_DASH,"ブーストダッシュでは敵の弾の追尾を\n振り切ることはできない");
	explanationType_.emplace(PAGE_TYPE::COMBAT_TRANSITION,"XBOX:Yボタン PSコン:△ボタン\n格闘移行状態に移行\n押した時点での相手の場所\nに向かって移動する");
	explanationType_.emplace(PAGE_TYPE::COMBAT,"相手の場所に到達したら自動で斬りつける");
	explanationType_.emplace(PAGE_TYPE::COMBAT_QUICK,"敵の位置に到達する前にもう一度ボタンを\n押すとその場で斬撃を出す");
	explanationType_.emplace(PAGE_TYPE::COMBAT_CANCEL,"格闘移行状態中に\nXBOX:Aボタン PSコン:□ボタンを\n素早く二回押すと\nブーストでキャンセルができる");
	explanationType_.emplace(PAGE_TYPE::NEXT,"ゲームに進む\nまだ見る");

}

bool TutorialScene::ButtonPush(void)
{
	JOYPAD_TYPE type = input_.GetJPadType();
	//プレステコンなら×ボタン
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		if (input_.IsTriggerd("b"))
		{
			return true;
		}
	}
	//XBOXならAボタン
	else if (JOYPAD_TYPE::XBOX_360 == type || JOYPAD_TYPE::XBOX_ONE == type)
	{
		if (input_.IsTriggerd("jump"))
		{
			return true;
		}
	}
	else
	{
		if (input_.IsTriggerd("b"))
		{
			return true;
		}
	}
	return false;
}

void TutorialScene::SelectCursor(void)
{
	if (pageIdx_ < PAGE_MAX)
	{
		return;
	}
	//上ボタンか十字キー上を押したら、カーソルを上に動かす
	if (input_.IsTriggerd("up") && cursorIdx_ > 1)
	{
		cursorIdx_--;
	}
	//下ボタンか十字キー下を押したら、カーソルを下に動かす
	else if (input_.IsTriggerd("down") && cursorIdx_ < 2)
	{
		cursorIdx_++;
	}

	if (input_.IsTriggerd("up") && cursorIdx_ > 1)
	{
		cursorIdx_--;
	}

}

void TutorialScene::ChangeGameScene(void)
{
	//ゲームシーンへ移行
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

bool TutorialScene::SelectTutorialOrGame(void)
{	
	if (ButtonPush())
	{
		if (cursorIdx_ == CURSOR_MAX)
		{
			ChangeGameScene();
			return true;
		}
		else
		{
			pageIdx_ = 13;
			return false;
		}
	}
	return false;

}
