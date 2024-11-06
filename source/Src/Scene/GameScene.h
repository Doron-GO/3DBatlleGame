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

    // プレイヤー番号
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

    // シーン状態
    enum class STATE
    {
        NONE,
        READY,
        PLAYING,
        MODE_SELECT,
    };

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

    //リソースマネージャー
    ResourceManager& resMng_;

    //状態管理(更新ステップ)
    std::function<void(void)>funcDraw_;

    //ステージ
    std::unique_ptr<Stage> stage_;

    //スカイドーム
    std::unique_ptr<SkyDome>skyDome_;

    //プレイヤーマネージャー
    std::unique_ptr<ActorManager> actorManager_;

    //HPやブーストゲージの表示
    std::vector<std::unique_ptr<UserInterface>> userInterfaces_;

    //プレイモード(シングル、対戦)
    int playMode_;

    //再戦モード
    int rematchMode_;

    //ゲームコントローラタイプ(PS、XBOX想定)
    JOYPAD_TYPE joyPadType_;

    //プレイヤーごとの描画スクリーン
    std::vector<int> cameraScreens_;

    //統合描画スクリーン
    int integrationScreen_;

    //描画スクリーンの座標
    std::map<PLAYER_NUM, VECTOR> screenPos_;

    //画像ハンドル
    std::map<IMG_TYPE, int> imgType_;

    //スタート計測変数
    float startCount_;

    //デルタタイム
    float deltaTime_;

    //3D設定
    void Init3DSetting(void);
    //画像の読み込み
    void InitImage(void);
    //描画スクリーンと描画ステップ
    void InitRender(void);
    //UIの初期化(actorManagerからプレイヤーとボスを取得してきて、ゲームシーンで作っている)
    void InitUI(void);

    void InitCollider(void);

    //UIの生成
    void CreateUserInterface(const ActorBase& player, const ActorBase& target);

    //シングルモードの描画
    void DrawSingleMode(void);
    //対戦モードの描画
    void DrawBattleMode(void);

    //ゲームシーンに移行する
    void ChangeGameScene(void);
    //タイトルシーンに移行する
    void ChangeTitleScene(void);

    //ゲーム開始時のカウントダウン終了判定
    bool IsGameStart(void);
    //勝敗がついているかを判定
    bool IsGameSet(void);

    //対戦後のモードを選択するカーソルを動かす関数
    bool SelectCursor(void);
    //選択されたモードを決定する関数
    bool SelectDecide(void);
    //選択されたモードに遷移する関数
    void SelectTitleOrGame(void);

};
