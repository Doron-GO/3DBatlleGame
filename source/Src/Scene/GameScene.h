#pragma once
#include<DxLib.h>
#include<functional>
#include<vector>
#include<map>
#include "Scene.h"
#include"../Input/Input.h"
#include"../Object/Common/Transform.h"

class TimeCount;
class Grid;
class Player;
class Stage;
class SkyDome;
class Camera;
class ActorManager;
class ActorBase;
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

    virtual void Update(void) override;
    virtual void Draw(void) override;

private:

    // �V�[�����
    enum class STATE
    {
        NONE,
        READY,
        PLAYING,
        MODE_SELECT,
    };

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

    //���\�[�X�}�l�[�W���[
    ResourceManager& resMng_;

    //��ԊǗ�(�X�V�X�e�b�v)
    std::function<void(void)>funcDraw_;

    //�X�e�[�W
    std::unique_ptr<Stage> stage_;

    //�X�J�C�h�[��
    std::unique_ptr<SkyDome>skyDome_;

    //�v���C���[�}�l�[�W���[
    std::unique_ptr<ActorManager> actorManager_;

    //HP��u�[�X�g�Q�[�W�̕\��
    std::vector<std::unique_ptr<UserInterface>> userInterfaces_;

    //�v���C���[�h(�V���O���A�ΐ�)
    int playMode_;

    //�Đ탂�[�h
    int rematchMode_;

    //�Q�[���R���g���[���^�C�v(PS�AXBOX�z��)
    JOYPAD_TYPE joyPadType_;

    //�v���C���[���Ƃ̕`��X�N���[��
    std::vector<int> cameraScreens_;

    //�����`��X�N���[��
    int integrationScreen_;

    //�`��X�N���[���̍��W
    std::map<PLAYER_NUM, VECTOR> screenPos_;

    //�摜�n���h��
    std::map<IMG_TYPE, int> imgType_;

    //�X�^�[�g�v���ϐ�
    float startCount_;

    //�f���^�^�C��
    float deltaTime_;

    //3D�ݒ�
    void Init3DSetting(void);
    //�摜�̓ǂݍ���
    void InitImage(void);
    //�`��X�N���[���ƕ`��X�e�b�v
    void InitRender(void);
    //UI�̏�����(actorManager����v���C���[�ƃ{�X���擾���Ă��āA�Q�[���V�[���ō���Ă���)
    void InitUI(void);

    void InitCollider(void);

    //UI�̐���
    void CreateUserInterface(const ActorBase& player, const ActorBase& target);

    //�V���O�����[�h�̕`��
    void DrawSingleMode(void);
    //�ΐ탂�[�h�̕`��
    void DrawBattleMode(void);

    //�Q�[���V�[���Ɉڍs����
    void ChangeGameScene(void);
    //�^�C�g���V�[���Ɉڍs����
    void ChangeTitleScene(void);

    //�Q�[���J�n���̃J�E���g�_�E���I������
    bool IsGameStart(void);
    //���s�����Ă��邩�𔻒�
    bool IsGameSet(void);

    //�ΐ��̃��[�h��I������J�[�\���𓮂����֐�
    bool SelectCursor(void);
    //�I�����ꂽ���[�h�����肷��֐�
    bool SelectDecide(void);
    //�I�����ꂽ���[�h�ɑJ�ڂ���֐�
    void SelectTitleOrGame(void);

};
