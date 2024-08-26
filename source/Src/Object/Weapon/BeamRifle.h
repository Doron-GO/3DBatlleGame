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

    //’e‚ğ”­Ë‚·‚é
    void Trigger(void);

    //’e‚Ì’Ç”öó‘Ô‚ğ‰ğœ
    void InActivateHorming(void);

    //BeamShot‚Ì”z—ñ‚ğæ“¾
   const std::vector<std::unique_ptr<BeamShot>>& GetBeams()const;

   //c’e”‚ğæ“¾
   const int& GetNumnberOfBullets(void);

private:

    Player& player_;

    std::vector<std::unique_ptr<BeamShot>> beams_;

    //’e‚ğˆê”­‘•“U‚·‚é‚Ü‚Å‚ÌŠÔ
    float coolTime_;

    //c’e”
    int numberofBullets_;

    float deltaTime_;
    
    //c’e‚ÌŒvZ
    void NumnberOfBullets(void);

    //’e‚ÌƒN[ƒ‹ƒ^ƒCƒ€‚ğŒv‘ª‚·‚é
    void CoolTimeCount(void);
    
    //’e‚ğˆê”­‘•“U‚·‚é
    void Reload(void);

};

