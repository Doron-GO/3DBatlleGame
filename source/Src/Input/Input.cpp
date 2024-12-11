#include"../Object/Time/DeltaTime.h"
#include "Input.h"
#include<cassert>
#include<algorithm>

bool Input::IsTriggerd(const std::string& input) const
{
	return !GetLastInput(input) && GetCurrentInput(input);
}

bool Input::IsPrassed(const std::string& input) const
{

	return GetCurrentInput(input);
}

bool Input::IsDoublePrassed(const std::string& input) const
{
	if (GetCurrentDoubleTap(input,30.0f))
	{
		if (IsTriggerd(input))
		{
			return true;
		}
	}
	return false;
}

bool Input::WasPrassed(const std::string& input) const
{
	return GetLastInput(input);
}

bool Input::IsHold(const std::string& input) const
{
	if (!GetLastDoubleTap(input, 5.0f))
	{
		if (IsPrassed(input))
		{
			return true;
		}
	}
	return false;
}

bool Input::GetCurrentInput(const std::string& input) const
{
	auto it = currentState_.find(input);
	if (it == currentState_.end())
	{
		return false;
	}
	return it->second;
}

bool Input::GetCurrentDoubleTap(const std::string& input, float timeOutLimit) const
{
	auto it = testCurrentState_.find(input);
	auto ltsIt = testLastState_.find(input);
	if (it == testCurrentState_.end()|| it->second.second == false)
	{
		return false;
	}
	float count = testInputTable_.find(input)->second.second;
	if (ltsIt->second.second == true &&count < timeOutLimit)
	{
		return true;
	}
	return false;
}

bool Input::GetLastInput(const std::string& input) const
{
	auto it = lastState_.find(input);
	if (it == lastState_.end())
	{
		return false;
	}
	return it->second;
}

bool Input::GetLastDoubleTap(const std::string& input, float timeOutLimit) const
{
	auto it = testLastState_.find(input);
	if (it == testLastState_.end())
	{
		return false;
	}
	float count = testInputTable_.find(input)->second.second;
	if (count<timeOutLimit)
	{
		return true;
	}

	return false;
}

const DINPUT_JOYSTATE& Input::GetPadStickRight() const
{
	return joyDInState_;
}

const int& Input::StickSet(JOYPAD_TYPE type, XY xy)
{	
	switch (type)
	{
	case JOYPAD_TYPE::XBOX_360:

		if (xy == XY::X){ return joyDInState_.Rx;}
		else		    { return joyDInState_.Ry;}
		break;
	case JOYPAD_TYPE::XBOX_ONE:

		if (xy == XY::X) { return joyDInState_.Rx; }
		else			 { return joyDInState_.Ry; }
		break;
	case JOYPAD_TYPE::DUAL_SHOCK_4:

		if (xy == XY::X) { return joyDInState_.Z; }
		else			 { return joyDInState_.Rz; }
		break;
	case JOYPAD_TYPE::DUAL_SENSE:

		if (xy == XY::X) { return joyDInState_.Z; }
		else { return joyDInState_.Rz; }
		break;
	default:
		if (xy == XY::X) { return joyDInState_.Rx; }
		else { return joyDInState_.Ry; }
		break;
	}
}

Input::Input(int& num):padNum_(num),
keyRx_(StickSet(GetJPadType(),XY::X)), keyRy_(StickSet(GetJPadType(), XY::Y)),
keyLx_(joyDInState_.X), keyLy_(joyDInState_.Y)
{
	//左辺がゲームの命令(文字列)
	inputTable_["next"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_RETURN} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_R } ,//padのstart
							{ INPUT_TYPE::MOUSE,MOUSE_INPUT_LEFT }
							};

	inputTable_["cancel"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_ESCAPE} 
							};

	inputTable_["pause"] = { 
							{INPUT_TYPE::KEYBORD, KEY_INPUT_P},
							{ INPUT_TYPE::GAMEPAD,PAD_INPUT_L } //padのselect
							};
	
	//ジャンプ
	inputTable_["jump"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_Z} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_A} //padのA　プレステの□
							};
	
	//
	inputTable_["b"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_X} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_B} //プレステの×
							};
	//攻撃
	inputTable_["shot"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_V} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_C} //padのX プレステの〇
							};

	//格闘
	inputTable_["combat"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_C} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_X} //padのY　プレステの△

	};

	//上
	inputTable_["up"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_UP} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_UP} ,//padのA
	};

	//下
	inputTable_["down"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_DOWN} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_DOWN} ,//padのA
	};
	//右
	inputTable_["right"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_RIGHT} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_RIGHT} ,//padのA
	};
	//左
	inputTable_["left"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_LEFT} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_LEFT} ,//padのA
	};
	//左
	inputTable_["Rleft"] = {{INPUT_TYPE::GAMEPAD,1} ,//padのA
	};
	//M
	inputTable_["m"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_M} 
	};
	//N
	inputTable_["n"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_N} 
	};
	//キーコンフィグ
	inputTable_["keyconfig"] = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_K} ,
	};


	//左辺がゲームの命令(文字列)
	testInputTable_["next"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_RETURN} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_R } ,//padのstart
							{ INPUT_TYPE::MOUSE,MOUSE_INPUT_LEFT }
							};

	testInputTable_["cancel"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_ESCAPE} 
							};

	testInputTable_["pause"].first = {
							{INPUT_TYPE::KEYBORD, KEY_INPUT_P},
							{ INPUT_TYPE::GAMEPAD,PAD_INPUT_L } //padのselect
							};

	//ジャンプ
	testInputTable_["jump"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_Z} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_A} //padのA　プレステの□
							};
	
	//
	testInputTable_["b"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_X} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_B} //padのB プレステの×
							};
	//攻撃
	testInputTable_["shot"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_V} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_C} //padのY プレステの×
							};

	//スライディング
	testInputTable_["combat"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_C} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_X} //padのX　プレステの△

	};

	//上
	testInputTable_["up"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_UP} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_UP} ,//padのA
	};

	//下
	testInputTable_["down"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_DOWN} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_DOWN} ,//padのA
	};
	//右
	testInputTable_["right"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_RIGHT} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_RIGHT} ,//padのA
	};
	//左
	testInputTable_["left"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_LEFT} ,
							{INPUT_TYPE::GAMEPAD,PAD_INPUT_LEFT} ,//padのA
	};
	//左
	testInputTable_["Rleft"].first = {{INPUT_TYPE::GAMEPAD,1} ,//padのA
	};
	//M
	testInputTable_["m"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_M} 
	};
	//N
	testInputTable_["n"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_N} 
	};
	//キーコンフィグ
	testInputTable_["keyconfig"].first = {
							{ INPUT_TYPE::KEYBORD, KEY_INPUT_K} ,
	};


	tempTable_ = inputTable_;
	currentState_.clear();
	testCurrentState_.clear();
	for (const auto& record: inputTable_)
	{
		currentState_[record.first] = false;
		testCurrentState_[record.first].first = false;
		testCurrentState_[record.first].second = false;
	}
	for (const auto& record: inputTable_)
	{
		testInputTable_[record.first].second = 0.0f;
	}
	excludeOnCongigList_.reserve(5);
	excludeOnCongigList_.push_back("left");
	excludeOnCongigList_.push_back("right");
	excludeOnCongigList_.push_back("up");
	excludeOnCongigList_.push_back("down");
	excludeOnCongigList_.push_back("c");

}

void Input::Update(void)
{
	float delta = DeltaTime::GetInstsnce().GetDeltaTime();
	//ハードの入力自体はチェックします
	char keycode[256];
	GetHitKeyStateAll(keycode);//キーボード入力
	auto a= 0x0000 + padNum_;
	int padInfo = GetJoypadInputState(a);//パッドpadNum番入力
	auto padInfo2 = GetJoypadDirectInputState(a, &joyDInState_);
	int mouseInfo = GetMouseInput();//マウス入力

	lastState_ = currentState_;//更新前の情報を記録
	testLastState_ = testCurrentState_;//更新前の情報を記録
	//すべての登録済みの入力をチェックする
	for (const auto& record : inputTable_)
	{
		bool pressed = false;
		bool padDpressed = false;
		for (const auto& inputInfo : record.second)
		{
			//入力種別によて調べ方が違う
			switch (inputInfo.type)
			{
			case INPUT_TYPE::KEYBORD:
				pressed = keycode[inputInfo.inputID];
				break;
			case INPUT_TYPE::GAMEPAD:
				pressed = padInfo &inputInfo.inputID;
				padDpressed = padInfo2 & inputInfo.inputID;
				break;
			case INPUT_TYPE::MOUSE:
				pressed = mouseInfo & inputInfo.inputID;
				break;
			}
			//１つでも押されていたらループが止まる
			if (pressed|| padDpressed)//ここ大事
			{
				break;
			}
		}
		if (!(padDpressed==0))
		{
			currentState_[record.first] = padDpressed;
		}
		else
		{
			currentState_[record.first] = pressed;
			testCurrentState_[record.first].first = pressed;
			if (testCurrentState_[record.first].first ==true)
			{
				testCurrentState_[record.first].second = true;
			}
		}
		if (testInputTable_[record.first].second<15.0f)
		{		
			if (testCurrentState_[record.first].second ==true)
			{
				testInputTable_[record.first].second += (delta * 60.0f);
			}
		}			
		else
		{
			testCurrentState_[record.first].second = false;
			testInputTable_[record.first].second =0.0f;
		}
	}
}


void Input::RewriteInput(const std::string& key, const INPUT_INFO& inputinfo){
	
	auto it = tempTable_.find(key);
	if (it == tempTable_.end())
	{
		assert(0 && "そんな命令は登録されていません");
		return;
	}
	for (auto& record : it->second)
	{
		if (inputinfo.type == record.type)
		{
			record.inputID = inputinfo.inputID;
			return;
		}
	}
	//新規登録
	it->second.push_back(inputinfo);

}

void Input::ComitInputConfig(void)
{
	inputTable_ = tempTable_;
}

bool Input::CheckExclusiveInput(void)
{
	char keycode[256];
	GetHitKeyStateAll(keycode);//キーボード入力
	int padInfo = GetJoypadInputState(DX_INPUT_PAD1);//パッド１番入力
	int mouseInfo = GetMouseInput();//マウス入力

	for (const auto& exKey: excludeOnCongigList_)
	{
		for (const auto& input : inputTable_[exKey])
		{
			switch (input.type)
			{
			case INPUT_TYPE::KEYBORD:
				if(keycode [input.inputID])
				{
					return true;//無効入力されている
				}
				break;
			case INPUT_TYPE::GAMEPAD:
				if (padInfo& input.inputID)
				{
					return true;//無効入力されている
				}
				break;
			case INPUT_TYPE::MOUSE:
				if (mouseInfo&input.inputID)
				{
					return true;//無効入力されている
				}
				break;
			default:
				break;
			}
		}
	}

	return false;
}


const bool Input::IsStickTiltedPlusSign(int stick)const
{
	if (10<stick)
	{
		return true;
	}
	return false;
}

const bool Input::IsStickTiltedMinusSign(int stick)const
{
	if (stick < -100)
	{
		return true;
	}
	return false;
}

const bool Input::IsStickSlanted() const
{
	if (IsStickTiltedPlusSign(keyRx_) && IsStickTiltedPlusSign(keyRy_) ||
		IsStickTiltedPlusSign(keyRx_) && IsStickTiltedMinusSign(keyRy_)||
		IsStickTiltedMinusSign(keyRx_)&& IsStickTiltedPlusSign(keyRy_) ||
		IsStickTiltedMinusSign(keyRx_) && IsStickTiltedMinusSign(keyRy_))
	{
		return true;
	}
	return false;
}

JOYPAD_TYPE Input::GetJPadType(void)
{
	return static_cast<JOYPAD_TYPE>(GetJoypadType(1));
}

const bool Input::isStickTilted(STICK_DIR dir, STICK_LR lr)const
{
	int x=0, y=0;
	if (lr==STICK_LR::R)
	{
		x = keyRx_,y = keyRy_;
	}
	else
	{
		x = keyLx_, y = keyLy_;
	}
	switch (dir)
	{
	case STICK_DIR::UP:
		return IsStickTiltedMinusSign(y);
		break;
	case STICK_DIR::DW:
		return IsStickTiltedPlusSign(y); 
		break;
	case STICK_DIR::R:
		return IsStickTiltedPlusSign(x);
		break;
	case STICK_DIR::L:
		return IsStickTiltedMinusSign(x);
		break;
	default:
		return false;
		break;
	}
}

const bool Input::isStickTilted(STICK_LR lr) const
{
	if (lr == STICK_LR::R)
	{
		//Rスティックが倒されていたらtrue
		if (abs(keyRx_)>400|| abs(keyRy_) > 400)
		{
			return true;
		}
	}
	else
	{
		//Lスティックが倒されていたらtrue
		if (abs(keyLx_) > 400 || abs(keyLy_) > 400)
		{
			return true;
		}
	}
	return false;
}


