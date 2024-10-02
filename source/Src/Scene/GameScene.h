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

    // プレイヤー番号
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

    //読み込み画像の種類
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

    // 再戦モード
    enum class REMATCH_MODE
    {
        BACK_TO_TITLE,
        ONE_MORE_FIGHT
    };


    ResourceManager& resMng_;

    void (GameScene::* draw_)();

    //ステージ
    std::unique_ptr<Stage> stage_;

    //プレイヤーマネージャー
    std::unique_ptr<ActorManager>actorManager_;

    //プレイモード(シングル、対戦)
    int playMode_;

    //再戦モード
    int rematchMode_;

    //ゲームコントローラタイプ(PS、XBOX想定)
    JOYPAD_TYPE joyPadType_;

    //UI
    std::vector<UserInterface>userInterface_;

    //プレイヤーごとの描画スクリーン
    std::vector<int>cameraScreens_;

    //統合描画スクリーン
    int integrationScreen_;

    //描画スクリーンの座標
    std::map<PLAYER_NUM, VECTOR> screenPos_;

    //画像ハンドル
    std::map<IMG_TYPE, int> imgType_;

    //スタート計測変数
    float startCount_;

    float deltaTime_;

    //画像の読み込み
    void InitImage(void);

    void DrawSIngleMode(void);

    void DrawBattleMode(void);

    void ChangeGameScene();
    void ChangeTitleScene();

    //ゲーム開始時のカウントダウン終了判定
    bool IsGameStart(void);
    bool IsGameSet(void);

    bool SelectCursor(void);
    bool SelectDecide(void);
    void TitleOrGame(void);

};
