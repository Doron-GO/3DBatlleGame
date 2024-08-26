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

    VECTOR enemyPos_;

    VECTOR hormingVec_;

    float offsetEnemy_;
    //�z�[�~���O���邩�ǂ����𔻒�
   const bool& isHorming_;

   //�e�������ł��鎞�Ԃ��v��
   float deathCount_;

   int testShotNUm_;

   float bigDamage_;

   float hormingCount_;

};

