#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"Src/_debug/_DebugConOut.h"
#include"Src/_debug/_DebugDispOut.h"
#include"Src/Scene/SceneManager.h"
#include"Src/Manager/ResourceManager.h"
#include"Src/Scene/Transition/IrisTransitor.h"
#include"Src/Scene/GameScene.h"
#include"Src/Scene/TitleScene.h"
#include"Src/Object/Time/DeltaTime.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region DxLib初期化

	//640×480ドットの32bitカラーに設定
	SetGraphMode(1600, 1000, 32);
	SetWindowSizeChangeEnableFlag(true);

	ChangeWindowMode(true);
	_dbgSetup(1200, 800, 256);
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		return -1;
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

	int padnum = 1;
	SceneManager sceneManager;
	IrisTransitor irisTransitor;
	ResourceManager::CreateInstance();
	Input input(padnum);
	sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager,irisTransitor, input));
	DeltaTime::GetInstsnce().SetStart();

	// ゲームループ
	while (ProcessMessage() ==0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{		
		_dbgDraw();
		DeltaTime::GetInstsnce().Update();
		sceneManager.Update();
		sceneManager.Draw();
		ScreenFlip();

	}

	// 後始末
	Effkseer_End();
	DxLib_End();
	
	return 0;

}