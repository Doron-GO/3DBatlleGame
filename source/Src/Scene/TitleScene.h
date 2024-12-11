#pragma once
#include "../Object/Common/Transform.h"
#include<memory>
#include<vector>
#include<map>
#include<functional>
#include "Scene.h"

class ResourceManager;
class EffectManager;
class Stage;
class Input;
class Camera;
class BeamSaber;
class SkyDome;

class TitleScene :
    public Scene
{

public:
    TitleScene(SceneManager& manager,Transitor& transit,Input& input);
    ~TitleScene();

    virtual void Update(void) override;
    virtual void Draw(void) override;

private:

    //�摜�^�C�v
    enum class IMG_TYPE
    {
        TITLE_LOGO,
        PLEASE_A,
        PLEASE_CROSS,
        SINGLE_MODE,
        MULTI_PLAY,
        TRIANGLE,
        GAME_START,
        TUTORIAL,
    };

    //�Q�[�����[�h
    enum class GAME_MODE
    {
        SINGLE,
        MULTI
    };

    //�Q�[�����[�h
    enum class TUTORIAL_MODE
    {
        NON,
        TUTORIAL
    };

    //�Q�[�����[�h�Z���N�g
    int playMode_;

    //�`���[�g���A�����[�h�Z���N�g
    int tutorialMode_;

    //�^�C�g����ʂ���I����ʂɍs�����𔻒�
    bool isTitleStart_;

    //�Q�[�����[�h���I������Ă��邩�𔻒�
    bool isGameModeSelect_;

    //�`���[�g���A�������邩�ǂ����𔻒�
    bool isTutorialSelect_;
    
    // �V���O���g���Q��
    ResourceManager& resMng_;

    //�A�b�v�f�[�g
    std::function<void(void)>funcUpdate_;

    //�`��
    std::function<void(void)>funcDraw_;

    //�t�H���g�n���h��
    int fontHandle_;

    //�J����
    const std::unique_ptr<Camera> camera_;

    //�X�e�[�W
    const std::unique_ptr<Stage> stage_;

    //�X�J�C�h�[��
    const std::unique_ptr<SkyDome> skyDome_;

    //�G�t�F�N�g�Đ�
    std::map<int,std::unique_ptr<EffectManager>> effectManagers_;

    //��O�̃��{�b�g
    Transform frontTransform_;

    //���̂ق��̃��{�b�g
    Transform backTransform_;

    //�r�[���T�[�x��
    std::vector< std::unique_ptr<BeamSaber>> beamSabers_;

    //�摜�^�C�v�z��
    std::map<IMG_TYPE, int> imgType_;

    //�Q�[���V�[���Ɉڍs
    void ChangeGameScene(void);

    //�`���[�g���A���V�[���Ɉڍs
    void ChangeTutorialScene(void);

    //�u�{�^���������āv�\��
    void DrawPleaseButton(void);

    //����{�^������(XBOX�R���ł�PS�R���ł������ꏊ�̃{�^���ɂ��邽��)
    bool ButtonPush(void);

    //���[�h�I��p�̃J�[�\��
    bool SelectCursorGameMode(void);

    //���[�h�I��p�̃J�[�\��
    bool SelectCursorTutorialMode(void);

    //�Q�[�����[�h��I�����āA�I���ςݏ�Ԃɂ���
    void SelectGameMode(void);

    //�`���[�g���A�������邩��I�����āA�V�[���J�ڂ��s��
    void SelectTutorialMode(void);

    //�摜�̓ǂݍ���
    void InitImage(void);

    //���f���̏����ݒ�
    void InitModel(void);

    //�J�����̏����ݒ�
    void InitCamera(void);

    //�G�t�F�N�g�̏����ݒ�
    void InitEffect(void);

    //
    void TitleUpdate(void);

    //�Q�[�����[�h�I���A�b�v�f�[�g
    void UpdateSelectGameMode(void);

    //�`���[�g���A���I���A�b�v�f�[�g
    void UpdateSelectTutorial(void);

    //�^�C�g���`��
    void DrawTitleLogo(void);

    //�Q�[�����[�h�I��`��
    void DrawSelectGameMode(void);

    //�`���[�g���A���I��`��
    void DrawSelectTutorial(void);

};

