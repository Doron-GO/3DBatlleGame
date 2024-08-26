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
	int xx=0;
	int yy=0;
	GetWindowSize(&xx, &yy);

	SetGraphMode(1600, 1000, 32);			//640×480ドットの32bitカラーに設定
	SetWindowSizeChangeEnableFlag(true);

	ChangeWindowMode(true);
	_dbgSetup(1200, 800, 256);
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	if (Effekseer_Init(8000)==-1)
	{
		return -1;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	int padnum = 1;
	SceneManager sceneManager;
	IrisTransitor irisTransitor;
	ResourceManager::CreateInstance();
	Input input(padnum);
	sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager,irisTransitor, input));
	DeltaTime::GetInstsnce().SetStart();
	while (ProcessMessage() ==0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{		
		_dbgDraw();
		DeltaTime::GetInstsnce().Update();
		sceneManager.Update();
		sceneManager.Draw();
		ScreenFlip();

	}
	Effkseer_End();
	DxLib_End();
	return 0;

}