#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"Src/Scene/SceneManager.h"
#include"Src/Manager/ResourceManager.h"
#include"Src/Scene/Transition/IrisTransitor.h"
#include"Src/Scene/GameScene.h"
#include"Src/Scene/TutorialScene.h"
#include"Src/Scene/TitleScene.h"
#include"Src/Object/Time/DeltaTime.h"

LPCSTR  FONT_PATH = "Data/Font/nicokaku_v2-5.ttf";



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region DxLib初期化

	SetGraphMode(1600, 1000, 32);
	//ウィンドウサイズを変更できるようにする
	SetWindowSizeChangeEnableFlag(true);

	ChangeWindowMode(true);

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	if (AddFontResourceEx(FONT_PATH, FR_PRIVATE, NULL) > 0) 
	{
	}
	else
	{
		int p = 0;
	}


#pragma endregion

#pragma region Effekseer初期化

	if (Effekseer_Init(8000) == -1)
	{
		return -1;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

#pragma endregion


	//操作するコントローラ番号
	int padNum = 1;

	//シーンマネージャーの生成
	SceneManager sceneManager;

	//リソースマネージャーを生成
	ResourceManager::CreateInstance();

	//トラんじったーの生成
	IrisTransitor irisTransitor;

	//インプットの生成
	Input input(padNum);

	//初期シーンはタイトルシーンに設定
	sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager,irisTransitor, input));
	//sceneManager.ChangeScene(std::make_shared<TutorialScene>(sceneManager,1,irisTransitor, input));

	//デルタタイムの計測スタート
	DeltaTime::GetInstsnce().SetStart();

	// ゲームループ
	while (ProcessMessage() ==0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//デルタタイムの更新
		DeltaTime::GetInstsnce().Update();	
		//現在のシーンの更新
		sceneManager.Update();
		//現在のシーンの描画
		sceneManager.Draw();
		ScreenFlip();
	}
	// 後始末
	Effkseer_End();
	DxLib_End();
	return 0;

}