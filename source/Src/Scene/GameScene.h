#pragma once
#include<DxLib.h>
#include<vector>
#include<map>
#include "Scene.h"
#include"../Input/Input.h"

class TimeCount;
class Grid;
class Player;
class Stage;
class Camera;
class ActorManager;
class UserInterface;
class ResourceManager;

class GameScene :
    public Scene
{

public:

    // �v���C���[�ԍ�
    enum class PLAYER_NUM
    {
        P_1,
        P_2,
        MAX,
    };

    GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input);
    ~GameScene();

    virtual void Update() override;
    virtual void Draw() override;

private:

    //�ǂݍ��݉摜�̎��
    enum class IMG_TYPE
    {
        ONE_MOR_FIGHT,
        BACK_TO_TITLE,
        PLEASE_CROSS,
        PLEASE_A,
        TRIANGLE,
        READY,
        FIGHT
    };

    // �Đ탂�[�h
    enum class REMATCH_MODE
    {
        BACK_TO_TITLE,
        ONE_MORE_FIGHT
    };


    ResourceManager& resMng_;

    void (GameScene::* draw_)();

    //�X�e�[�W
    std::unique_ptr<Stage> stage_;

    //�v���C���[�}�l�[�W���[
    std::unique_ptr<ActorManager>actorManager_;

    //�v���C���[�h(�V���O���A�ΐ�)
    int playMode_;

    //�Đ탂�[�h
    int rematchMode_;

    //�Q�[���R���g���[���^�C�v(PS�AXBOX�z��)
    JOYPAD_TYPE joyPadType_;

    //UI
    std::vector<UserInterface>userInterface_;

    //�v���C���[���Ƃ̕`��X�N���[��
    std::vector<int>cameraScreens_;

    //�����`��X�N���[��
    int integrationScreen_;

    //�`��X�N���[���̍��W
    std::map<PLAYER_NUM, VECTOR> screenPos_;

    //�摜�n���h��
    std::map<IMG_TYPE, int> imgType_;

    //�X�^�[�g�v���ϐ�
    float startCount_;

    float deltaTime_;

    //�摜�̓ǂݍ���
    void InitImage(void);

    void DrawSIngleMode(void);

    void DrawBattleMode(void);

    void ChangeGameScene();
    void ChangeTitleScene();

    //�Q�[���J�n���̃J�E���g�_�E���I������
    bool IsGameStart(void);
    bool IsGameSet(void);

    bool SelectCursor(void);
    bool SelectDecide(void);
    void TitleOrGame(void);

};
