#pragma once
#include "Scene.h"
#include<string>
#include<map>

class ResourceManager;
class Input;

class TutorialScene :
    public Scene
{
public:

    TutorialScene(SceneManager& manager, int playMode, Transitor& transit, Input& input);
    ~TutorialScene();


    virtual void Update(void) override;
    virtual void Draw(void) override;


private:

    //読み込み画像の種類
    enum class PAGE_TYPE
    {
        UI,
        JUMP,
        SHOT,  
        RANGE_IN,
        RANGE_OUT,
        HORMING,
        BOOST,
        BOOST_DASH,
        HORMING_BOOST,
        HORMING_DASH,
        COMBAT_TRANSITION,
        COMBAT,
        COMBAT_QUICK,
        COMBAT_CANCEL,
        NEXT

    };

    //説明文ハンドル
    int explanationHandle_;

    //次の画面へハンドル
    int nextHandle_;

    //選択カーソル
    int cursorIdx_;

    //プレイモード
    int playMode_;

    //現在の説明ページ数
    int pageIdx_;

    //リソースマネージャー
    ResourceManager& resMng_;

    //カーソル画像
    int cursorImg_;

    //説明画像ハンドル配列
    std::map<PAGE_TYPE, int> imgType_;

    //説明文文字列配列
    std::map<PAGE_TYPE, std::string> explanationType_;

    //画像の読み込み
    void InitImage(void);

    //説明文読み込み
    void InitExplanation(void);

    //決定ボタン処理(XBOXコンでもPSコンでも同じ場所のボタンにするため)
    bool ButtonPush(void);
    
    void SelectCursor(void);

    //ゲームシーンに移行
    void ChangeGameScene(void);

    //
    bool SelectTutorialOrGame(void);

};

