#pragma once
#include "../Object/Comon/Transform.h"
#include<memory>
#include<vector>
#include "Scene.h"

class ResourceManager;
class Stage;
class Input;
class camera;
class BeamSaber;

class TitleScene :
    public Scene
{

public:
    TitleScene(SceneManager& manager,Transitor& transit,Input& input);
    ~TitleScene();

    virtual void Update() override;
    virtual void Draw() override;

private:

    enum class IMG_H
    {
        TITLE_LOGO,
        PLEASE_A,
        PLEASE_CROSS,
        SINGLE_MODE,
        MULTI_PLAY,
        TRIANGLE
    };

    enum class GAME_MODE
    {
        SINGLE,
        MULTI
    };

    int padNum_;

    int selectNum_;

    bool startFlag_;
    // �V���O���g���Q��
    ResourceManager& resMng_;

    //�J����
    const std::unique_ptr<Camera> camera_;

    //�X�e�[�W
    const std::unique_ptr<Stage> stage_;

    //��O�̃��{�b�g
    Transform frontTransform_;

    //���̂ق��̃��{�b�g
    Transform backTransform_;

    //�r�[���T�[�x��
   std::vector< std::unique_ptr<BeamSaber>> beamSabers_;


    //�摜�n���h��
    std::map<IMG_H, int> imgH_;
    void ChangeGameScene(void);

    void DrawPleaseButton(void);

    void DrawModeSelect(void);

    bool ButtonPush(void);

    bool SelectCursor(void);

    bool SelectDecide(void);

    void InitImage(void);

    void InitModel(void);

    void InitCamera(void);

};

