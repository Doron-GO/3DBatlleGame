#pragma once
#include<DxLib.h>
#include<vector>
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

    enum class PLAYER_NUM
    {
        P_1,
        P_2,
    };

    GameScene(SceneManager& manager, int playMode, Transitor& transit, Input& input);
    ~GameScene();

    virtual void Update() override;
    virtual void Draw() override;

private:

    enum class IMG_H
    {
        ONE_MOR_FIGHT,
        BACK_TO_TITLE,
        PLEASE_CROSS,
        PLEASE_A,
        TRIANGLE,
        READY,
        FIGHT
    };

    enum class SELECT
    {
        BACK_TO_TITLE,
        ONE_MORE_FIGHT
    };

    static constexpr int SINGLE_PLEY_MODE = 0;

    ResourceManager& resMng_;

    void (GameScene::* _draw)();
    //カメラのポインタ
    //std::unique_ptr<Camera> camera_;

    //プレイヤーのポインタ
    std::unique_ptr<Stage> stage_;

    //プレイヤーマネージャー
    std::unique_ptr<ActorManager>ActorManager_;

    int playMode_;
    int selectNum_;

    JOYPAD_TYPE joyPadType_;

    //UI
    std::vector<UserInterface>userInterface_;

    std::vector<int>cameraScreens_;

    int integrationScreen_;

    std::vector<VECTOR> screenPos_;

    //画像ハンドル
    std::map<IMG_H, int> imgH_;

    //スタート計測変数
    float startCount_;

    float deltaTime_;

    void DrawSIngleMode(void);

    void DrawBattleMode(void);

    void ChangeGameScene();
    void ChangeTitleScene();

    bool IsStart(void);

    void GameUI(void);
    void StartUI(void);
    void GameSetUI(void);
    bool SelectCursor(void);
    bool SelectDecide(void);
    void TitleOrGame(void);

};
