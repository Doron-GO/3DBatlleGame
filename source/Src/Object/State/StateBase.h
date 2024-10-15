#pragma once
#include"../../Common/Quaternion.h"
#include<DxLib.h>

class Input;

class StateBase
{
public:

	virtual ~ StateBase(void){};
	virtual void Update(void)=0;

};

