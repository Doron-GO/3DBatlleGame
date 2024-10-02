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

    //�X�V
    virtual void Update(void) override;

    //�`��
    virtual void Draw(void)override;

    //�����蔻��N���XSaber���擾����
    Saber& GetSaber(void) const;

private:

    std::unique_ptr<Saber> saber_;

};

