#pragma once
#include "../Object/Common/Transform.h"
#include<memory>
#include<vector>
#include<map>
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
        TRIANGLE
    };

    //�Q�[�����[�h
    enum class GAME_MODE
    {
        SINGLE,
        MULTI
    };

    //���[�h�Z���N�g
    int playMode_;

    //�^�C�g����ʂ���I����ʂɍs�������𔻒�
    bool startFlag_;
    
    // �V���O���g���Q��
    ResourceManager& resMng_;

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

    //�u�{�^���������āv�\��
    void DrawPleaseButton(void);

    //���[�h�Z���N�g�摜�`��
    void DrawModeSelect(void);

    //����{�^������(XBOX�R���ł�PS�R���ł������ꏊ�̃{�^���ɂ��邽��)
    bool ButtonPush(void);

    //���[�h�I��p�̃J�[�\��
    bool SelectCursor(void);

    //���[�h��I�����ăQ�[���V�[���Ɉڍs����
    bool SelectDecide(void);

    //�摜�̓ǂݍ���
    void InitImage(void);

    //���f���̏����ݒ�
    void InitModel(void);

    //�J�����̏����ݒ�
    void InitCamera(void);

    //�G�t�F�N�g�̏����ݒ�
    void InitEffect(void);

};

