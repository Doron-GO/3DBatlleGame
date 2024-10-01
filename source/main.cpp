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

#pragma region DxLib������

	SetGraphMode(1600, 1000, 32);
	//�E�B���h�E�T�C�Y��ύX�ł���悤�ɂ���
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

#pragma region Effekseer������

	if (Effekseer_Init(8000) == -1)
	{
		return -1;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

#pragma endregion

	//���삷��R���g���[���ԍ�
	int padNum = 1;

	//�V�[���}�l�[�W���[�̐���
	SceneManager sceneManager;

	//�g���񂶂����[�̐���
	IrisTransitor irisTransitor;

	//���\�[�X�}�l�[�W���[�𐶐�
	ResourceManager::CreateInstance();
	//�C���v�b�g�̐���
	Input input(padNum);

	//�����V�[���̓^�C�g���V�[���ɐݒ�
	sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager,irisTransitor, input));

	//�f���^�^�C���̌v���X�^�[�g
	DeltaTime::GetInstsnce().SetStart();

	// �Q�[�����[�v
	while (ProcessMessage() ==0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{		
		_dbgDraw();
		//�f���^�^�C���̍X�V
		DeltaTime::GetInstsnce().Update();
		
		//���݂̃V�[���̍X�V
		sceneManager.Update();
		//���݂̃V�[���̕`��
		sceneManager.Draw();
		ScreenFlip();

	}

	// ��n��
	Effkseer_End();
	DxLib_End();
	
	return 0;

}