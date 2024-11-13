#include<EffekseerForDXLib.h>
#include"../Manager/ResourceManager.h"
#include"../Object/Weapon/BeamSaber.h"
#include "../Object/Camera/Camera.h"
#include "../Scene/SceneManager.h"
#include"../Utility/AsoUtility.h"
#include"Transition/Transitor.h"
#include"../Object/Stage/Stage.h"
#include"../Object/Stage/SkyDome.h"
#include"../Manager/EffectManager.h"
#include "../Input/Input.h"
#include"../../Config.h"
#include "TitleScene.h"
#include"GameScene.h"
#include<DxLib.h>

#pragma region Parameter


constexpr int TYPE_ROBOT_FRONT = 0;
constexpr int TYPE_ROBOT_BACK = 1;

//���f���̍��W
constexpr VECTOR FRONT_ROBOT_POS = { 0.0f, -25.0f, 0.0f };
constexpr VECTOR BACK_ROBOT_POS = { 0.0f, -450.0f, 5000.0f };

//�J�������W
constexpr VECTOR CAMERA_POS = { 100.0f, 20.0f, -130.0f };
constexpr VECTOR CAMERA_TARGET_POS = { 100.0f, 15.0f, 0.0f };

//�^�C�g�����S�摜�֌W�̐��l(���W�A���S���W�A�傫��)
constexpr int TITLE_LOGO_POS_X = 775;
constexpr int TITLE_LOGO_POS_Y = 400;
constexpr int TITLE_LOGO_CENTER_X = 320;
constexpr int TITLE_LOGO_CENTER_Y = 60;
constexpr double TITLE_LOGO_SCALE = 2.5;

//�~�{�^���������Ăˉ摜�̐��l(���W�A���S���W�A�傫��)
constexpr int PLEASE_CROSS_BUTTON_POS_X = 800;
constexpr int PLEASE_CROSS_BUTTON_POS_Y = 800;
constexpr int PLEASE_CROSS_BUTTON_CENTER_X = 480;
constexpr int PLEASE_CROSS_BUTTON_CENTER_Y = 52;
constexpr double PLEASE_CROSS_BUTTON_SCALE = 1.2;

//A�{�^���������Ăˉ摜�̐��l(���W�A���S���W�A�傫��)
constexpr int PLEASE_A_BUTTON_POS_X = 800;
constexpr int PLEASE_A_BUTTON_POS_Y = 800;
constexpr int PLEASE_A_BUTTON_CENTER_X = 395;
constexpr int PLEASE_A_BUTTON_CENTER_Y = 34;
constexpr double PLEASE_A_BUTTON_SCALE = 1.2;

//���[�h�I���摜�̐��l(���W�A���S���W�A�傫��)
//��l�p
constexpr int SINGLE_MODE_POS_X = 775;
constexpr int SINGLE_MODE_POS_Y = 400;
constexpr int SINGLE_MODE_CENTER_X = 165;
constexpr int SINGLE_MODE_CENTER_Y = 30;
constexpr double SINGLE_MODE_SCALE = 2.0;
//��l�p
constexpr int MULTI_PLAY_POS_X = 775;
constexpr int MULTI_PLAY_POS_Y = 600;
constexpr int MULTI_PLAY_CENTER_X = 165;
constexpr int MULTI_PLAY_CENTER_Y = 52;
constexpr double MULTI_PLAY_SCALE = 2.0;

//�I���J�[�\���摜�̐��l(���W�A���S���W�A�傫��)
constexpr int TRIANGLE_POS_X = 1200;
constexpr int TRIANGLE_POS_Y = 400;
constexpr int TRIANGLE_CENTER_X = 280;
constexpr int TRIANGLE_CENTER_Y = 63;
constexpr int TRIANGLE_OFFSET = 200;
constexpr double TRIANGLE_SCALE = 1.2;


const int EFFECT_TYPE_LIGHTNING = 0;
const int EFFECT_TYPE_ATOMOSPHERE = 1;

constexpr VECTOR EFFECT_LIGHTNING_FRONT_SCALE = { 5.0f,5.0f,10.0f };

constexpr VECTOR EFFECT_LIGHTNING_FRONT_OFFSET = { 30.0f, 10.0f, 20.0f };

constexpr VECTOR EFFECT_LIGHTNING_FRONT_ROT = { 0.0f, 0.0f, 0.0f };


constexpr VECTOR EFFECT_LIGHTNING_BACK_SCALE = { 10.0f,15.0f,10.0f };

constexpr VECTOR EFFECT_LIGHTNING_BACK_OFFSET = { -50.0f, 50.0f, 0.0f };

constexpr VECTOR EFFECT_LIGHTNING_BACK_ROT = { 0.0f, 0.0f, 0.0f };


constexpr VECTOR EFFECT_ATOMOSPHERE_SCALE = { 280.0f,280.0f,280.0f };

constexpr VECTOR EFFECT_ATOMOSPHERE_OFFSET = { 0.0f, -100.0f, 100.0f };

constexpr VECTOR EFFECT_ATOMOSPHERE_ROT = { 0.0f, 0.0f, 0.0f };


#pragma endregion

TitleScene::TitleScene(SceneManager& manager, Transitor& transit, Input& input) :Scene(manager,  transit,input),
resMng_(ResourceManager::GetInstance()), camera_(std::make_unique<Camera>()),stage_(std::make_unique<Stage>()),
skyDome_(std::make_unique<SkyDome>())

{
	//���[�h�I���̏����J�[�\���ʒu����l�p�ɐݒ�
	playMode_=0;
	//�{�^���������Ă˂̏�Ԃɂ���(�����ꂽ��I����ʂ�)
	startFlag_ = false;
	
	//�摜�̓ǂݍ���
	InitImage();
	//���f���̓ǂݍ���
	InitModel();
	//�J�����̏����ݒ�
	InitCamera();
	//�G�t�F�N�g�̏�����
	InitEffect();
	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);
	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 139, 139);

	sceneTransitor_.Start();

}

TitleScene::~TitleScene(void)
{

}

void TitleScene::Update(void)
{
	input_.Update();
	//�I����ʂłȂ����[�h�I��������Ă��Ȃ���Ζ߂�
	if (startFlag_&&SelectDecide())
	{
		return;
	}
	//�^�C�g����ʂœ���̃{�^�����������ƃ��[�h�I����
	if (!startFlag_ && ButtonPush())
	{
		startFlag_ = true;
	}
	skyDome_->Update();
	//�G�t�F�N�g�}�l�[�W���[�̍X�V
	for (auto& effectManager: effectManagers_)
	{
		effectManager.second->Update();
	}
	//�G�t�F�N�V�A�̃A�b�v�f�[�g
	UpdateEffekseer3D();
	sceneTransitor_.Update();
}

void TitleScene::Draw(void)
{
	ClearDrawScreen();
	//�J�����̍X�V
	camera_->SetBeforeDraw();
	//
	skyDome_->Draw();
	//�X�e�[�W�̕`��
	stage_->Draw();
	//��O���f���̕`��
	MV1DrawModel(frontTransform_.modelId);
	//�����f���̕`��
	MV1DrawModel(backTransform_.modelId);
	//�r�[���T�[�x���̕`��
	for (auto& beamSaber : beamSabers_)
	{
		beamSaber->Draw();
	}
	//�G�t�F�N�V�A�̕`��
	DrawEffekseer3D();

	//�{�^���������Ăˉ摜�`��
	DrawPleaseButton();
	//�{�^�����Ȃɂ�������Ă��Ȃ���΁A�^�C�g�����S��`��
	if (!startFlag_)
	{
		DrawRotaGraph2(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y,
			TITLE_LOGO_CENTER_X, TITLE_LOGO_CENTER_Y, TITLE_LOGO_SCALE, 0.0, imgType_[IMG_TYPE::TITLE_LOGO], true, false);
	}
	else//�{�^����������Ă���΃��[�h�Z���N�g�摜�`��
	{
		DrawModeSelect();
	}
	sceneTransitor_.Draw();
}

void TitleScene::ChangeGameScene(void)
{
	//�Q�[���V�[���ֈڍs
	sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, playMode_, sceneTransitor_, input_));
}

void TitleScene::DrawPleaseButton(void)
{
	JOYPAD_TYPE type = input_.GetJPadType();
	//PS�R���g���[���[�Ȃ�~�{�^���\�L
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		DrawRotaGraph2(PLEASE_CROSS_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_CROSS_BUTTON_CENTER_X, PLEASE_CROSS_BUTTON_CENTER_Y,
			PLEASE_CROSS_BUTTON_SCALE, 0.0, imgType_[IMG_TYPE::PLEASE_CROSS], true, false);
	}
	else//XBOX�R���g���[���[�Ȃ�A�{�^���\�L
	{
		DrawRotaGraph2(PLEASE_A_BUTTON_POS_X, PLEASE_CROSS_BUTTON_POS_Y,
			PLEASE_A_BUTTON_CENTER_X, PLEASE_A_BUTTON_CENTER_Y,
			PLEASE_A_BUTTON_SCALE, 0.0, imgType_[IMG_TYPE::PLEASE_A], true, false);
	}
}

void TitleScene::DrawModeSelect(void)
{
	//���ݐڑ�����Ă���R���g���[���̐�
	int joyPadNum = GetJoypadNum();

	//�V���O�����[�h�I���摜
	DrawRotaGraph2(SINGLE_MODE_POS_X, SINGLE_MODE_POS_Y,
		SINGLE_MODE_CENTER_X, SINGLE_MODE_CENTER_Y, SINGLE_MODE_SCALE, 0.0, imgType_[IMG_TYPE::SINGLE_MODE], true, false);

	if (joyPadNum >= 2)
	{
		//�ΐ탂�[�h�I���摜
		DrawRotaGraph2(MULTI_PLAY_POS_X, MULTI_PLAY_POS_Y,
			MULTI_PLAY_CENTER_X, MULTI_PLAY_CENTER_Y, MULTI_PLAY_SCALE, 0.0, imgType_[IMG_TYPE::MULTI_PLAY], true, false);
	}
	
	//�J�[�\���I���摜
	DrawRotaGraph2(TRIANGLE_POS_X, TRIANGLE_POS_Y + (TRIANGLE_OFFSET * playMode_),
		TRIANGLE_CENTER_X, TRIANGLE_CENTER_Y, TRIANGLE_SCALE, 0.0, imgType_[IMG_TYPE::TRIANGLE], true, false);
}

bool TitleScene::ButtonPush(void)
{

	JOYPAD_TYPE type = input_.GetJPadType();
	//�v���X�e�R���Ȃ�~�{�^��
	if (JOYPAD_TYPE::DUAL_SHOCK_4 == type || JOYPAD_TYPE::DUAL_SENSE == type)
	{
		if (input_.IsTriggerd("b"))
		{
			return true;
		}
	}
	//XBOX�Ȃ�A�{�^��
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

bool TitleScene::SelectCursor(void)
{
	//���ݐڑ�����Ă���R���g���[���̐�
	int joyPadNum = GetJoypadNum();

	//��{�^�����\���L�[�����������A�J�[�\������ɓ�����
	if (input_.IsTriggerd("up") && playMode_ > static_cast<int>(GAME_MODE::SINGLE))
	{
		playMode_--;
	}
	//���{�^�����\���L�[������������A�J�[�\������ɓ�����
	else if (joyPadNum>=2&&
			input_.IsTriggerd("down") &&
			playMode_ < static_cast<int>(GAME_MODE::MULTI)
			)
	{
		playMode_++;
	}
	return ButtonPush();
}

bool TitleScene::SelectDecide(void)
{
	//����{�^���������ꂽ��A�Q�[���V�[���Ɉڍs
	if (SelectCursor())
	{
		ChangeGameScene();
		return true;
	}
	return false;
}

void TitleScene::InitImage(void)
{
	//�摜�ǂݍ���
	imgType_.emplace(IMG_TYPE::TITLE_LOGO, resMng_.Load(ResourceManager::SRC::TIRLE_LOGO_IMAGE).handleId_);
	imgType_.emplace(IMG_TYPE::PLEASE_A, resMng_.Load(ResourceManager::SRC::PLEASE_A).handleId_);
	imgType_.emplace(IMG_TYPE::PLEASE_CROSS, resMng_.Load(ResourceManager::SRC::PLEASE_CROSS).handleId_);
	imgType_.emplace(IMG_TYPE::SINGLE_MODE, resMng_.Load(ResourceManager::SRC::SINGLE_PLAY_LOGO).handleId_);
	imgType_.emplace(IMG_TYPE::MULTI_PLAY, resMng_.Load(ResourceManager::SRC::MULTI_PLAY_LOGO).handleId_);
	imgType_.emplace(IMG_TYPE::TRIANGLE, resMng_.Load(ResourceManager::SRC::TRIANGLE).handleId_);
}

void TitleScene::InitModel(void)
{
	//��O���{�b�g�̏����ݒ�
	//���f���̓ǂݍ���
	frontTransform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//���f���̑傫��
	frontTransform_.scl = AsoUtility::VECTOR_ONE;
	//���f���̍��W
	frontTransform_.pos = FRONT_ROBOT_POS;
	//���f���̉�]
	frontTransform_.quaRot = Quaternion();
	//���f���̃��[�J����]
	frontTransform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(160.0f), 0.0f });
	//���f���̐ݒ���X�V
	frontTransform_.Update();
	//���f���̃|�[�Y��ǂݍ��݁A�ݒ�
	std::string model = PATH_ANIMATION_PLAYER + "Win.mv1";
	int animModel = MV1LoadModel(model.c_str());
	MV1AttachAnim(frontTransform_.modelId, 0, animModel);
	//�r�[���T�[�x���̐���
	beamSabers_.emplace_back(std::make_unique<BeamSaber>(0, frontTransform_));

	//�����{�b�g�̏����ݒ�
	//���f���̓ǂݍ���
	backTransform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//���f���̑傫��
	backTransform_.scl = AsoUtility::VECTOR_ONE;
	//���f���̍��W
	backTransform_.pos = BACK_ROBOT_POS;
	//���f���̉�]
	backTransform_.quaRot = Quaternion();
	//���f���̐ݒ���X�V
	backTransform_.Update();
	//���f���̃|�[�Y��ǂݍ��݁A�ݒ�
	MV1AttachAnim(backTransform_.modelId, 0, animModel);
	//�r�[���T�[�x���̐���
	beamSabers_.emplace_back(std::make_unique<BeamSaber>(0, backTransform_));

	//�r�[���T�[�x�������{�b�g�̎�ɓ���������
	for (auto& beamSaber : beamSabers_)
	{
		beamSaber->Activate();
		beamSaber->Update();
	}

}

void TitleScene::InitCamera(void)
{
	//�J�����ݒ�
	camera_->ChangeMode(Camera::MODE::FIXED_POINT);
	camera_->SetTargetPos(CAMERA_TARGET_POS);
	camera_->SetCameraPos(CAMERA_POS);
}

void TitleScene::InitEffect(void)
{

	effectManagers_.emplace(
					TYPE_ROBOT_FRONT,
					std::make_unique <EffectManager>(beamSabers_[TYPE_ROBOT_FRONT]->GetTransform()) );

	effectManagers_[TYPE_ROBOT_FRONT]->Add(EFFECT_TYPE_LIGHTNING,
		EFFECT_LIGHTNING_FRONT_SCALE,
		EFFECT_LIGHTNING_FRONT_OFFSET,	
		EFFECT_LIGHTNING_FRONT_ROT,
		true,
		true,
		resMng_.Load(ResourceManager::SRC::LIGHTNING).handleId_
	);

	effectManagers_[TYPE_ROBOT_FRONT]->Add(EFFECT_TYPE_ATOMOSPHERE,
		EFFECT_ATOMOSPHERE_SCALE,
		EFFECT_ATOMOSPHERE_OFFSET,
		EFFECT_ATOMOSPHERE_ROT,
		true,
		true,
		resMng_.Load(ResourceManager::SRC::ATOMOSPHERE).handleId_
	);

	//���G�t�F�N�g
	effectManagers_[TYPE_ROBOT_FRONT]->Play(EFFECT_TYPE_LIGHTNING);
	//���̂悤�ȃG�t�F�N�g
	effectManagers_[TYPE_ROBOT_FRONT]->Play(EFFECT_TYPE_ATOMOSPHERE);



	effectManagers_.emplace(
		TYPE_ROBOT_BACK,
		std::make_unique <EffectManager>(beamSabers_[TYPE_ROBOT_BACK]->GetTransform()));

	effectManagers_[TYPE_ROBOT_BACK]->Add(EFFECT_TYPE_LIGHTNING,
		EFFECT_LIGHTNING_BACK_SCALE,
		EFFECT_LIGHTNING_BACK_OFFSET,
		EFFECT_LIGHTNING_BACK_ROT,
		true,
		false,
		resMng_.Load(ResourceManager::SRC::LIGHTNING).handleId_
	);
	effectManagers_[TYPE_ROBOT_BACK]->Play(EFFECT_TYPE_LIGHTNING);


}

