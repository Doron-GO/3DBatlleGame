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

    //画像タイプ
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

    //ゲームモード
    enum class GAME_MODE
    {
        SINGLE,
        MULTI
    };

    //ゲームモード
    enum class TUTORIAL_MODE
    {
        NON,
        TUTORIAL
    };

    //ゲームモードセレクト
    int playMode_;

    //チュートリアルモードセレクト
    int tutorialMode_;

    //タイトル画面から選択画面に行くかを判定
    bool isTitleStart_;

    //ゲームモードが選択されているかを判定
    bool isGameModeSelect_;

    //チュートリアルを見るかどうかを判定
    bool isTutorialSelect_;
    
    // シングルトン参照
    ResourceManager& resMng_;

    //アップデート
    std::function<void(void)>funcUpdate_;

    //描画
    std::function<void(void)>funcDraw_;

    //フォントハンドル
    int fontHandle_;

    //カメラ
    const std::unique_ptr<Camera> camera_;

    //ステージ
    const std::unique_ptr<Stage> stage_;

    //スカイドーム
    const std::unique_ptr<SkyDome> skyDome_;

    //エフェクト再生
    std::map<int,std::unique_ptr<EffectManager>> effectManagers_;

    //手前のロボット
    Transform frontTransform_;

    //奥のほうのロボット
    Transform backTransform_;

    //ビームサーベル
    std::vector< std::unique_ptr<BeamSaber>> beamSabers_;

    //画像タイプ配列
    std::map<IMG_TYPE, int> imgType_;

    //ゲームシーンに移行
    void ChangeGameScene(void);

    //チュートリアルシーンに移行
    void ChangeTutorialScene(void);

    //「ボタンを押して」表示
    void DrawPleaseButton(void);

    //決定ボタン処理(XBOXコンでもPSコンでも同じ場所のボタンにするため)
    bool ButtonPush(void);

    //モード選択用のカーソル
    bool SelectCursorGameMode(void);

    //モード選択用のカーソル
    bool SelectCursorTutorialMode(void);

    //ゲームモードを選択して、選択済み状態にする
    void SelectGameMode(void);

    //チュートリアルを見るかを選択して、シーン遷移を行う
    void SelectTutorialMode(void);

    //画像の読み込み
    void InitImage(void);

    //モデルの初期設定
    void InitModel(void);

    //カメラの初期設定
    void InitCamera(void);

    //エフェクトの初期設定
    void InitEffect(void);

    //
    void TitleUpdate(void);

    //ゲームモード選択アップデート
    void UpdateSelectGameMode(void);

    //チュートリアル選択アップデート
    void UpdateSelectTutorial(void);

    //タイトル描画
    void DrawTitleLogo(void);

    //ゲームモード選択描画
    void DrawSelectGameMode(void);

    //チュートリアル選択描画
    void DrawSelectTutorial(void);

};

