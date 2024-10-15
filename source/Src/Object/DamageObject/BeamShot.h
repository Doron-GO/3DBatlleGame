#pragma once
#include<functional>
#include "DamageObject.h"

class BeamShot :
    public DamageObject
{
public:

    static constexpr float SPEED = 50.0f;

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

    //�r�[���̏��ł��v������f���^�^�C���ɂ�����萔
    static constexpr float DEATH_COUNT_RATE = 60.0f;

    //�r�[�����A�N�e�B�u�ł����鎞��
    static constexpr float DEATH_COUNT_TIME = 140.0f;
    static constexpr float HORMING_COUNT_TIME = 120.0f;

    //�r�[���̍Ĕ��ˉ\���Ԃ��v������f���^�^�C���ɂ�����萔
    static constexpr float BEAM_COUNT_RATE = 60.0f;

    //�r�[�������ˉ\�ɂȂ�܂ł̎���
    static constexpr float BEAM_COUNT_TIME = 320.0f;

    //�e���ǂ�������G�̍��W��������ɂ��炷���߂̕␳�l
    static constexpr float OFFSET_ENEMY = 100.0f;

    //�e���ǂ�������{�X�̍��W��������ɂ��炷���߂̕␳�l
    static constexpr float OFFSET_BOSS_ENEMY = 1500.0f;

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

    void (BeamShot::* _update)();
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

