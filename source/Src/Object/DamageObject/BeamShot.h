#pragma once
#include "DamageObject.h"
class BeamShot :
    public DamageObject
{
public:

    static constexpr float SPEED = 50.0f;

    BeamShot(int playerType,const bool& isHorming,int num,int playMode);

    //更新
    virtual void Update(VECTOR enemyPos)override;	

    //描画
    virtual void Draw(void) override;

    //有効化
    virtual void Activate(void)override;

    //非有効化
    virtual void InActivate(void)override;

    //衝突時処理
    void Hit(void);

    //モデルIDの取得
    virtual const int& GetModelId(void)const override;

    //大ダメージの取得
     const float& GetBigDamage(void)const;

    //非ホーミングでの有効化
    void InActivateHorming();

private:

    //ビームの状態
    enum class BEAM_STATE
    {
        BEAM,
        HIT
    };

    //プレイモード
    enum class PLAY_MODE
    {
        SINGLE_MODE,
        BATTLE_MODE
    };

    //ビームの消滅を計測するデルタタイムにかける定数
    static constexpr float DEATH_COUNT_RATE = 60.0f;

    //ビームがアクティブでいられる時間
    static constexpr float DEATH_COUNT_TIME = 140.0f;
    static constexpr float HORMING_COUNT_TIME = 120.0f;

    //ビームの再発射可能時間を計測するデルタタイムにかける定数
    static constexpr float BEAM_COUNT_RATE = 60.0f;

    //ビームが発射可能になるまでの時間
    static constexpr float BEAM_COUNT_TIME = 320.0f;

    //弾が追いかける敵の座標を少し上にずらすための補正値
    static constexpr float OFFSET_ENEMY = 100.0f;

    //弾が追いかけるボスの座標を少し上にずらすための補正値
    static constexpr float OFFSET_BOSS_ENEMY = 1500.0f;

    //待機状態アップデート
    void WaitUpdate(void);

    //ホーミング無しアップデート
    void NormalUpdate(void);

    //ホーミング有りアップデート
    void HormingUpdate(void);

    //ビームの消滅時間を計測して消滅時間に達したら、非アクティブ状態にする
    void TimeToDeath(void);

    //敵座標へのベクトルを作り、弾の進行方向を設定している
    void Horming(void);

    //弾のクールタイムを計測する
    void CoolTimeCount(void);

    void (BeamShot::* _update)();

    //相手プレイヤーの座標
    VECTOR enemyPos_;

    //追尾するベクトル
    VECTOR hormingVec_;

    //敵座標調整用数値
    float offsetEnemy_;

    //ホーミングするかどうかを判定
   const bool& isHorming_;

   //弾が生存できる時間を計測
   float deathCount_;

   //大ダメージ時のダメージ量
   float bigDamage_;

   //ホーミングする時間の測定用変数
   float hormingCount_;

};

