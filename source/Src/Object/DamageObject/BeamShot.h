#pragma once
#include<functional>
#include "DamageObject.h"

class BeamShot :
    public DamageObject
{
public:

    static constexpr float SPEED = 5000.0f;

    BeamShot(int playerType,const bool& isHorming,int num,int playMode);

    //�X�V
    virtual void Update(VECTOR enemyPos)override;	

    //�`��
    virtual void Draw(void) override;

    //�L����
    virtual void Activate(void)override;

    //��L����
    virtual void InActivate(void)override;

    //�Փˎ�����
    void Hit(void);

    //���f��ID�̎擾
    virtual const int& GetModelId(void)const override;

    //��_���[�W�̎擾
     const float& GetBigDamage(void)const;

    //��z�[�~���O�ł̗L����
    void InActivateHorming(void);

private:

    //�r�[���̏��
    enum class BEAM_STATE
    {
        BEAM,
        HIT
    };

    //�v���C���[�h
    enum class PLAY_MODE
    {
        SINGLE_MODE,
        BATTLE_MODE
    };


    //�ҋ@��ԃA�b�v�f�[�g
    void WaitUpdate(void);

    //�z�[�~���O�����A�b�v�f�[�g
    void NormalUpdate(void);

    //�z�[�~���O�L��A�b�v�f�[�g
    void HormingUpdate(void);

    //�r�[���̏��Ŏ��Ԃ��v�����ď��Ŏ��ԂɒB������A��A�N�e�B�u��Ԃɂ���
    void TimeToDeath(void);

    //�G���W�ւ̃x�N�g�������A�e�̐i�s������ݒ肵�Ă���
    void Horming(void);

    //�e�̃N�[���^�C�����v������
    void CoolTimeCount(void);

    std::function<void(void)> update_;

    //����v���C���[�̍��W
    VECTOR enemyPos_;

    //�ǔ�����x�N�g��
    VECTOR hormingVec_;

    //�G���W�����p���l
    float offsetEnemy_;

    //�z�[�~���O���邩�ǂ����𔻒�
   const bool& isHorming_;

   //�e�������ł��鎞�Ԃ��v��
   float deathCount_;

   //��_���[�W���̃_���[�W��
   float bigDamage_;

   //�z�[�~���O���鎞�Ԃ̑���p�ϐ�
   float hormingCount_;

};

