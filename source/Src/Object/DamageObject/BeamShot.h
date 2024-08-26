#pragma once
#include "DamageObject.h"
class BeamShot :
    public DamageObject
{
public:

    static constexpr float SPEED = 50.0f;

    BeamShot(int playerType,const bool& isHorming,int num,int playMode);
    virtual void Draw(void)override;
    virtual void Update(VECTOR enemyPos)override;
    virtual void Activate(void)override;
    virtual void InActivate(void)override;
    void Hit(void);
    virtual const int& GetModelId(void)const override;

     const float& GetBigDamage(void)const;

    void InActivateHorming();

private:

    enum class BEAM_STATE
    {
        BEAM,
        HIT
    };

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

    VECTOR enemyPos_;

    VECTOR hormingVec_;

    float offsetEnemy_;
    //ホーミングするかどうかを判定
   const bool& isHorming_;

   //弾が生存できる時間を計測
   float deathCount_;

   int testShotNUm_;

   float bigDamage_;

   float hormingCount_;

};

