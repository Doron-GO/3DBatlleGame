#pragma once
#include "Scene.h"
#include<string>
#include<map>

class ResourceManager;
class Input;

class TutorialScene :
    public Scene
{
public:

    TutorialScene(SceneManager& manager, int playMode, Transitor& transit, Input& input);
    ~TutorialScene();


    virtual void Update(void) override;
    virtual void Draw(void) override;


private:

    //�ǂݍ��݉摜�̎��
    enum class PAGE_TYPE
    {
        UI,
        JUMP,
        SHOT,  
        RANGE_IN,
        RANGE_OUT,
        HORMING,
        BOOST,
        BOOST_DASH,
        HORMING_BOOST,
        HORMING_DASH,
        COMBAT_TRANSITION,
        COMBAT,
        COMBAT_QUICK,
        COMBAT_CANCEL,
        NEXT

    };

    //�������n���h��
    int explanationHandle_;

    //���̉�ʂփn���h��
    int nextHandle_;

    //�I���J�[�\��
    int cursorIdx_;

    //�v���C���[�h
    int playMode_;

    //���݂̐����y�[�W��
    int pageIdx_;

    //���\�[�X�}�l�[�W���[
    ResourceManager& resMng_;

    //�J�[�\���摜
    int cursorImg_;

    //�����摜�n���h���z��
    std::map<PAGE_TYPE, int> imgType_;

    //������������z��
    std::map<PAGE_TYPE, std::string> explanationType_;

    //�摜�̓ǂݍ���
    void InitImage(void);

    //�������ǂݍ���
    void InitExplanation(void);

    //����{�^������(XBOX�R���ł�PS�R���ł������ꏊ�̃{�^���ɂ��邽��)
    bool ButtonPush(void);
    
    void SelectCursor(void);

    //�Q�[���V�[���Ɉڍs
    void ChangeGameScene(void);

    //
    bool SelectTutorialOrGame(void);

};

