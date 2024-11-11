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

    //画像タイプ
    enum class IMG_TYPE
    {
        TITLE_LOGO,
        PLEASE_A,
        PLEASE_CROSS,
        SINGLE_MODE,
        MULTI_PLAY,
        TRIANGLE
    };

    //ゲームモード
    enum class GAME_MODE
    {
        SINGLE,
        MULTI
    };

    //モードセレクト
    int playMode_;

    //タイトル画面から選択画面に行く下かを判定
    bool startFlag_;
    
    // シングルトン参照
    ResourceManager& resMng_;

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

    //「ボタンを押して」表示
    void DrawPleaseButton(void);

    //モードセレクト画像描画
    void DrawModeSelect(void);

    //決定ボタン処理(XBOXコンでもPSコンでも同じ場所のボタンにするため)
    bool ButtonPush(void);

    //モード選択用のカーソル
    bool SelectCursor(void);

    //モードを選択してゲームシーンに移行する
    bool SelectDecide(void);

    //画像の読み込み
    void InitImage(void);

    //モデルの初期設定
    void InitModel(void);

    //カメラの初期設定
    void InitCamera(void);

    //エフェクトの初期設定
    void InitEffect(void);

};

