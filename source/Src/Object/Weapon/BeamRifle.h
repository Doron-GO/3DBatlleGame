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

    //�e�𔭎˂���
    void Trigger(void);

    //�e�̒ǔ���Ԃ�����
    void InActivateHorming(void);

    //BeamShot�̔z����擾
   const std::vector<std::unique_ptr<BeamShot>>& GetBeams()const;

   //�c�e�����擾
   const int& GetNumnberOfBullets(void);

private:

    Player& player_;

    std::vector<std::unique_ptr<BeamShot>> beams_;

    //�e���ꔭ���U����܂ł̎���
    float coolTime_;

    //�c�e��
    int numberofBullets_;

    float deltaTime_;
    
    //�c�e�̌v�Z
    void NumnberOfBullets(void);

    //�e�̃N�[���^�C�����v������
    void CoolTimeCount(void);
    
    //�e���ꔭ���U����
    void Reload(void);

};

