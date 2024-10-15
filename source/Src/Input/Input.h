#pragma once
#include<unordered_map>
#include<DxLib.h>
#include<string>
#include<vector>


	// �Q�[���R���g���[���[�^�C�v
	// DxLib�萔�ADX_OTHER���ɑΉ�
	enum class JOYPAD_TYPE
	{
		OTHER = 0,
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		SWITCH_JOY_CON_L,
		SWITCH_JOY_CON_R,
		SWITCH_PRO_CTRL,
		MAX
	};


class KeyConfigScene;

class Input
{
public:

	//�X�e�B�b�N�̕���
	enum class STICK_DIR {
		UP,
		DW,
		R,
		L
	};
	//�ǂ���X�e�B�b�N��
	enum class STICK_LR {
		R,
		L
	};



	enum class XY {
		X,
		Y
	};

	//���͎��
	enum class INPUT_TYPE {
		KEYBORD,//�L�[�{�[�h
		GAMEPAD,//�Q�[���p�b�h
		MOUSE	//�}�E�X
	};


	//���͏��(�������)
	struct INPUT_INFO {
		INPUT_TYPE type;//���͎��
		//���ۂɒ��ׂ�l(PAD��}�E�X�Ȃ�r�b�g��\���L�[�{�[�h�Ȃ�L�[�R�[�h��)
		//�\��
		int inputID;
	};


	Input(int& num);
	/// <summary>
	/// �L�[���͏����X�V����(���t���[��)
	/// </summary>
	/// <summary>
	/// �L�[���͏����X�V����(���t���[��)
	/// </summary>
	void Update(void);

	/// <summary>
	/// ���������u�Ԃ��ǂ�����Ԃ�(�������ςȂ���false)
	/// </summary>
	/// <param name="input">���͎���(next��pause)</param>
	/// <returns>������: false �����ĂȂ�</returns>
	bool IsTriggerd(const std::string& input)const;
	/// <summary>
	/// �����������ǂ�����Ԃ�
	/// </summary>
	/// <param name="str">���͎���(next��pause)</param>
	/// <returns>true ������: false �����ĂȂ�</returns>
	bool IsPrassed(const std::string& input)const;
	/// <summary>
	/// ��񉟂��ꂽ���ǂ�����Ԃ�(20.0f�̊�)
	/// </summary>
	/// <param name="input">���͎���(next��pause)</param>
	/// <param name="timeOutLimit">�C�ӂ̎���</param>
	/// <returns>true ������: false �����ĂȂ�</returns>
	bool IsDoublePrassed(const std::string& input) const;
	/// <summary>
	/// �O�t���[���{�^����������Ă������ǂ�����Ԃ�
	/// </summary>
	/// <param name="input">���͎���(next��pause)</param>
	/// <returns>������: false �����ĂȂ�</returns>
	bool WasPrassed(const std::string& input) const;
	/// <summary>
	/// �O�t���[����������Ă��č��������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <param name="input">���͎���(next��pause)</param>
	/// <returns>������: false �����ĂȂ�</returns>
	bool IsHold(const std::string& input) const;

	/// <summary>
	/// ���̓e�[�u���̏�������
	/// </summary>
	/// <param name="key">���ߕ�����</param>
	/// <param name="inputinfo">�@�킩��̓��͏��(�n�[�h)</param>
	void RewriteInput(const std::string& key, const INPUT_INFO& inputinfo);

	void ComitInputConfig(void);

	/// <summary>
	/// ���݂̓��͂����O���͂ɓ�����̂��ǂ������`�F�b�N����
	/// </summary>
	/// <returns>true:���O���͂ɓ�����,false:������Ȃ�</returns>
	bool CheckExclusiveInput(void);


	// �ڑ����ꂽ�R���g���[���̎�ʂ��擾����
	JOYPAD_TYPE GetJPadType(void);

	//L��������R�X�e�B�b�N���C�ӂ̕����ɓ|����Ă��邩�ǂ�������
	const bool isStickTilted(STICK_DIR dir, STICK_LR lr)const;
	//L��������R�X�e�B�b�N���|����Ă��Ȃ�������
	//�|��Ă�����true
	const bool isStickTilted(STICK_LR lr)const;

	//�X�e�B�b�N���΂߂ɓ����Ă��邩�ǂ����A�����P�ɍ��E�A�����Q�ɏ㉺������
	const bool IsStickSlanted()const;

	DINPUT_JOYSTATE joyDInState_;
	const int& padNum_;

	//Xbox�ƃv���X�e�R�����Ή��̃A�i���O�X�e�B�b�N(-1000�`1000�̐��l�������Ă���)
	const int& keyRx_;
	const int& keyRy_;
	const int& keyLx_;
	const int& keyLy_;

private:
	
	//�Q�[�����̈Ӗ��Ɠ��͂��ꂽ���ǂ������y�A�ɂ�������
//�Ƃɂ����Ώۂ́u�Ӗ��v�������ꂽ�炠true�ɂȂ�
	using InputState_t = std::unordered_map<std::string, bool>;

	//�Q�[�����̈Ӗ��Ɠ��͂��ꂽ���ǂ������y�A�ɂ�������
	//�Ƃɂ����Ώۂ́u�Ӗ��v�������ꂽ�炠true�ɂȂ�
	//�f������񉟂��ꂽ�������邽�߂̃e�X�g�p
	//pair��first:������Ă��邩�ǂ���,second:�O�ɂ�������Ă��邩�ǂ���
	using DoubleTapInputState_t = std::unordered_map<std::string, std::pair<bool, bool>>;

	//�Q�[������"�Ӗ�"�Ǝ��ۂ̃L�[�R�[�h�̃y�A
	//�L�[(first)�͖��߂̈Ӗ��A
	//�Q�[�����̈Ӗ��Ǝ��ۂ̈Ӗ��̓���(�n�[�h����)�̑Ή��\��\���u�^�v
	using InputTable_t =
		std::unordered_map<std::string, std::vector<INPUT_INFO>>;

	//�Q�[������"�Ӗ�"�Ǝ��ۂ̃L�[�R�[�h�̃y�A
	//�L�[(first)�͖��߂̈Ӗ��A
	//�Q�[�����̈Ӗ��Ǝ��ۂ̈Ӗ��̓���(�n�[�h����)�̑Ή��\��\���u�^�v
	using DoubleTapInputTable_t =
		std::unordered_map<std::string, std::pair< std::vector<INPUT_INFO>, float>>;

	std::vector<std::string>excludeOnCongigList_;//�L�[�R���t�B�O���O���X�g

	//�Ӗ��ƃn�[�h���͂̑Ή��\
	InputTable_t  inputTable_;
	InputTable_t  tempTable_;

	DoubleTapInputTable_t testInputTable_;

	//�������L�^
	InputState_t lastState_={};//������
	InputState_t currentState_={};//��

	//�f������񉟂��ꂽ�������邽�߂̃e�X�g�p
	DoubleTapInputState_t testLastState_={};//������
	DoubleTapInputState_t testCurrentState_={};//��


	//�l�̑���ƃR�s�[�̋֎~
	Input(const Input&) = delete;
	void operator = (const Input&) = delete;

	//���݂��̃L�[��������Ă��邩
	bool GetCurrentInput(const std::string& input)const;
	//���݂��̃L�[��������Ă��邩 �f������񉟂��ꂽ�������邽�߂̃e�X�g�p
	bool GetCurrentDoubleTap(const std::string& input, float timeOutLimit)const;
	//���O�ɂ��̃L�[��������Ă��邩
	bool GetLastInput(const std::string& input)const;
	//���O�ɂ��̃L�[��������Ă��邩 �f������񉟂��ꂽ�������邽�߂̃e�X�g�p
	bool GetLastDoubleTap(const std::string& input,float timeOutLimit)const;

	const DINPUT_JOYSTATE& GetPadStickRight() const;
	const int& StickSet(JOYPAD_TYPE type, XY xy);	

	
	//�֐��̎g�p��0���傫����Stick����܂��͉E�ɓ|��Ă��邱�ƂɂȂ�
	const bool IsStickTiltedPlusSign(int stick)const;
	//�֐��̎g�p��0��菬������Stick�����܂��͍��ɓ|��Ă��邱�ƂɂȂ�
	const bool IsStickTiltedMinusSign(int stick)const;


};

