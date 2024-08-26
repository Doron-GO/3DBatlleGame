#pragma once
#include<memory>
#include<vector>
#include"WeaponBase.h"
#include"../DamageObject/BeamShot.h"

class Player;

class BeamRifle:
    public WeaponBase
{

public:

    BeamRifle(int playerType, int playMode,Player& player);

    virtual void Update(void)override;

    virtual void Draw(void) override;

    //弾を発射する
    void Trigger(void);

    //弾の追尾状態を解除
    void InActivateHorming(void);

    //BeamShotの配列を取得
   const std::vector<std::unique_ptr<BeamShot>>& GetBeams()const;

   //残弾数を取得
   const int& GetNumnberOfBullets(void);

private:

    Player& player_;

    std::vector<std::unique_ptr<BeamShot>> beams_;

    //弾を一発装填するまでの時間
    float coolTime_;

    //残弾数
    int numberofBullets_;

    float deltaTime_;
    
    //残弾の計算
    void NumnberOfBullets(void);

    //弾のクールタイムを計測する
    void CoolTimeCount(void);
    
    //弾を一発装填する
    void Reload(void);

};

