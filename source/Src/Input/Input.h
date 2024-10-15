#pragma once
#include<unordered_map>
#include<DxLib.h>
#include<string>
#include<vector>


	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
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

	//スティックの方向
	enum class STICK_DIR {
		UP,
		DW,
		R,
		L
	};
	//どちらスティックか
	enum class STICK_LR {
		R,
		L
	};



	enum class XY {
		X,
		Y
	};

	//入力種別
	enum class INPUT_TYPE {
		KEYBORD,//キーボード
		GAMEPAD,//ゲームパッド
		MOUSE	//マウス
	};


	//入力情報(一つ当たり)
	struct INPUT_INFO {
		INPUT_TYPE type;//入力種別
		//実際に調べる値(PADやマウスならビットを表しキーボードならキーコードを)
		//表す
		int inputID;
	};


	Input(int& num);
	/// <summary>
	/// キー入力情報を更新する(毎フレーム)
	/// </summary>
	/// <summary>
	/// キー入力情報を更新する(毎フレーム)
	/// </summary>
	void Update(void);

	/// <summary>
	/// 今押した瞬間かどうかを返す(押しっぱなしはfalse)
	/// </summary>
	/// <param name="input">入力識別(nextやpause)</param>
	/// <returns>押した: false 押してない</returns>
	bool IsTriggerd(const std::string& input)const;
	/// <summary>
	/// 今押したかどうかを返す
	/// </summary>
	/// <param name="str">入力識別(nextやpause)</param>
	/// <returns>true 押した: false 押してない</returns>
	bool IsPrassed(const std::string& input)const;
	/// <summary>
	/// 二回押されたかどうかを返す(20.0fの間)
	/// </summary>
	/// <param name="input">入力識別(nextやpause)</param>
	/// <param name="timeOutLimit">任意の時間</param>
	/// <returns>true 押した: false 押してない</returns>
	bool IsDoublePrassed(const std::string& input) const;
	/// <summary>
	/// 前フレームボタンが押されていたかどうかを返す
	/// </summary>
	/// <param name="input">入力識別(nextやpause)</param>
	/// <returns>押した: false 押してない</returns>
	bool WasPrassed(const std::string& input) const;
	/// <summary>
	/// 前フレームも押されていて今も押しているかどうかを返す
	/// </summary>
	/// <param name="input">入力識別(nextやpause)</param>
	/// <returns>押した: false 押してない</returns>
	bool IsHold(const std::string& input) const;

	/// <summary>
	/// 入力テーブルの書き換え
	/// </summary>
	/// <param name="key">命令文字列</param>
	/// <param name="inputinfo">機器からの入力情報(ハード)</param>
	void RewriteInput(const std::string& key, const INPUT_INFO& inputinfo);

	void ComitInputConfig(void);

	/// <summary>
	/// 現在の入力が除外入力に当たるのかどうかをチェックする
	/// </summary>
	/// <returns>true:除外入力に当たる,false:当たらない</returns>
	bool CheckExclusiveInput(void);


	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(void);

	//LもしくはRスティックが任意の方向に倒されているかどうか判定
	const bool isStickTilted(STICK_DIR dir, STICK_LR lr)const;
	//LもしくはRスティックが倒されていないか判定
	//倒れていたらtrue
	const bool isStickTilted(STICK_LR lr)const;

	//スティックが斜めに入っているかどうか、引数１に左右、引数２に上下を入れる
	const bool IsStickSlanted()const;

	DINPUT_JOYSTATE joyDInState_;
	const int& padNum_;

	//Xboxとプレステコン両対応のアナログスティック(-1000～1000の数値が入っている)
	const int& keyRx_;
	const int& keyRy_;
	const int& keyLx_;
	const int& keyLy_;

private:
	
	//ゲーム中の意味と入力されたかどうかをペアにしたもの
//とにかく対象の「意味」が押されたらあtrueになる
	using InputState_t = std::unordered_map<std::string, bool>;

	//ゲーム中の意味と入力されたかどうかをペアにしたもの
	//とにかく対象の「意味」が押されたらあtrueになる
	//素早く二回押された判定を取るためのテスト用
	//pairのfirst:押されているかどうか,second:前にも押されているかどうか
	using DoubleTapInputState_t = std::unordered_map<std::string, std::pair<bool, bool>>;

	//ゲーム中の"意味"と実際のキーコードのペア
	//キー(first)は命令の意味、
	//ゲーム中の意味と実際の意味の入力(ハード入力)の対応表を表す「型」
	using InputTable_t =
		std::unordered_map<std::string, std::vector<INPUT_INFO>>;

	//ゲーム中の"意味"と実際のキーコードのペア
	//キー(first)は命令の意味、
	//ゲーム中の意味と実際の意味の入力(ハード入力)の対応表を表す「型」
	using DoubleTapInputTable_t =
		std::unordered_map<std::string, std::pair< std::vector<INPUT_INFO>, float>>;

	std::vector<std::string>excludeOnCongigList_;//キーコンフィグ除外リスト

	//意味とハード入力の対応表
	InputTable_t  inputTable_;
	InputTable_t  tempTable_;

	DoubleTapInputTable_t testInputTable_;

	//押した記録
	InputState_t lastState_={};//さっき
	InputState_t currentState_={};//今

	//素早く二回押された判定を取るためのテスト用
	DoubleTapInputState_t testLastState_={};//さっき
	DoubleTapInputState_t testCurrentState_={};//今


	//値の代入とコピーの禁止
	Input(const Input&) = delete;
	void operator = (const Input&) = delete;

	//現在そのキーが押されているか
	bool GetCurrentInput(const std::string& input)const;
	//現在そのキーが押されているか 素早く二回押された判定を取るためのテスト用
	bool GetCurrentDoubleTap(const std::string& input, float timeOutLimit)const;
	//直前にそのキーが押されているか
	bool GetLastInput(const std::string& input)const;
	//直前にそのキーが押されているか 素早く二回押された判定を取るためのテスト用
	bool GetLastDoubleTap(const std::string& input,float timeOutLimit)const;

	const DINPUT_JOYSTATE& GetPadStickRight() const;
	const int& StickSet(JOYPAD_TYPE type, XY xy);	

	
	//関数の使用上0より大きいとStickが上または右に倒れていることになる
	const bool IsStickTiltedPlusSign(int stick)const;
	//関数の使用上0より小さいとStickが下または左に倒れていることになる
	const bool IsStickTiltedMinusSign(int stick)const;


};

