#pragma once
#include<memory>
#include<vector>
#include"WeaponBase.h"
#include"../Actor/Player/Player.h"
#include "../DamageObject/Saber.h"

class Player;

class BeamSaber :
    public WeaponBase
{
public:

    
    BeamSaber(int playerType, Transform& player);

    //更新
    virtual void Update(void) override;

    //描画
    virtual void Draw(void)override;

    //当たり判定クラスSaberを取得する
    Saber& GetSaber(void) const;

private:

    std::unique_ptr<Saber> saber_;

};

