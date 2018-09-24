#include "Input.hpp"

// static変数の宣言
unsigned __int8 Input::controllerNum;
int Input::button[4][16];
int Input::stick[4][4];
XINPUT_STATE Input::state[4];
XINPUT_STICK_MY_DEADZONE Input::stickDeadZone;

Input::Input()
{
	// コントローラ分ループ
	controllerNum = 4;
	for (int i = 0; i != controllerNum; ++i)
	{
		ZeroMemory(&Input::state[i], sizeof(XINPUT_STATE));
		// ボタンを0に初期化(スティックはデッドゾーンの関係で行わない)
		for (int j = 0; j != 16; ++j)
		{
			Input::button[i][j] = 0;
		}
	}
}

Input::~Input()
{
	for (int i = 0; i != controllerNum; ++i)
	{
		ZeroMemory(&Input::state[i], sizeof(XINPUT_STATE));
	}
}

void Input::Update()
{
	// コントローラの数だけ行う
	for (int i = 0; i < controllerNum; i++)
	{
		ZeroMemory(&Input::state[i], sizeof(XINPUT_STATE));
		if (XInputGetState(i, &Input::state[i]) == ERROR_SUCCESS)		// 接続されている
		{
			// ボタン操作
			for (int j = 0; j != 16; ++j)
			{
				if (j == 10 || j == 11)		// xinput.h上で割り当てられていない
				{
					continue;
				}
				if ((Input::state[i].Gamepad.wButtons & (int)pow(2.0, (double)j)) != 0)		// ボタンを押したら
				{
					Input::button[i][j]++;
					if (Input::button[i][j] >= 1000)		// いつか上限値行くと思うので回避
					{
						Input::button[i][j] = 2;
					}
				}
				else if (Input::button[i][j] > 0)	// 押していたボタンを離したら
				{
					Input::button[i][j] = -1;
				}
				else		// ボタンに触れていない
				{
					Input::button[i][j] = 0;
				}
			}

			// スティック操作
			// 左スティックの左右(公式のデッドゾーンを使わせてもらうが自作する方が操作性よろしくなると思われる)
			if (Input::state[i].Gamepad.sThumbLX > stickDeadZone.LEFT_AXIS_X_RIGHT
				|| Input::state[i].Gamepad.sThumbLX < stickDeadZone.LEFT_AXIS_X_LEFT)		// スティックを操作したら
			{
				Input::stick[i][XINPUT_PAD::STICK_LEFT_AXIS_X] = Input::state[i].Gamepad.sThumbLX;
			}
			else		// スティックを操作していない
			{
				Input::stick[i][XINPUT_PAD::STICK_LEFT_AXIS_X] = 0;
			}
			// 左スティックの上下
			if (Input::state[i].Gamepad.sThumbLY > stickDeadZone.LEFT_AXIS_Y_UP
				|| Input::state[i].Gamepad.sThumbLY < stickDeadZone.LEFT_AXIS_Y_DOWN)		// スティックを操作したら
			{
				Input::stick[i][XINPUT_PAD::STICK_LEFT_AXIS_Y] = Input::state[i].Gamepad.sThumbLY;
			}
			else		// スティックを操作していない
			{
				Input::stick[i][XINPUT_PAD::STICK_LEFT_AXIS_Y] = 0;
			}
			// 右スティックの左右
			if (Input::state[i].Gamepad.sThumbRX > stickDeadZone.RIGHT_AXIS_X_RIGHT
				|| Input::state[i].Gamepad.sThumbRX < stickDeadZone.RIGHT_AXIS_X_LEFT)		// スティックを操作したら
			{
				Input::stick[i][XINPUT_PAD::STICK_RIGHT_AXIS_X] = Input::state[i].Gamepad.sThumbRX;
			}
			else		// スティックを操作していない
			{
				Input::stick[i][XINPUT_PAD::STICK_RIGHT_AXIS_X] = 0;
			}
			// 右スティックの上下
			if (Input::state[i].Gamepad.sThumbRY > stickDeadZone.RIGHT_AXIS_Y_UP
				|| Input::state[i].Gamepad.sThumbRY < stickDeadZone.RIGHT_AXIS_Y_DOWN)		// スティックを操作したら
			{
				Input::stick[i][XINPUT_PAD::STICK_RIGHT_AXIS_Y] = Input::state[i].Gamepad.sThumbRY;
			}
			else		// スティックを操作していない
			{
				Input::stick[i][XINPUT_PAD::STICK_RIGHT_AXIS_Y] = 0;
			}
		}
		else	// 接続されていない
		{
			ZeroMemory(&Input::state[i], sizeof(XINPUT_STATE));
			Input::controllerNum = i;		// その時点でコントローラの最大個数を決める
		}
	}
}

int Input::GetPadNum()
{
	return (int)Input::controllerNum;
}

int Input::GetPadButtonData(unsigned __int8 use_padnum, unsigned __int8 use_button)
{
	return Input::button[use_padnum][use_button];
}

int Input::GetPadTriggerData(unsigned __int8 use_padnum, bool use_Is_triggerLeft)
{
	if (use_Is_triggerLeft)
	{
		return Input::state[use_padnum].Gamepad.bLeftTrigger;
	}
	else
	{
		return Input::state[use_padnum].Gamepad.bRightTrigger;
	}
}

int Input::GetPadThumbData(unsigned __int8 use_padnum, unsigned __int8 use_stick)
{
	return Input::stick[use_padnum][use_stick];
}

void Input::SetPadDeadZone(short leftPad_right, short leftPad_left
	, short leftPad_up, short leftPad_down, short rightPad_right, short rightPad_left, short rightPad_up, short rightPad_down)
{
	Input::stickDeadZone.LEFT_AXIS_X_RIGHT = (leftPad_right == XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : leftPad_right;
	Input::stickDeadZone.LEFT_AXIS_X_LEFT = (leftPad_left == -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : leftPad_left;
	Input::stickDeadZone.LEFT_AXIS_Y_UP = (leftPad_up == XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : leftPad_up;
	Input::stickDeadZone.LEFT_AXIS_Y_DOWN = (leftPad_down == -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : leftPad_down;
	Input::stickDeadZone.RIGHT_AXIS_X_RIGHT = (rightPad_right == XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : rightPad_right;
	Input::stickDeadZone.RIGHT_AXIS_X_LEFT = (rightPad_left == -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : rightPad_left;
	Input::stickDeadZone.RIGHT_AXIS_Y_UP = (rightPad_up == XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : rightPad_up;
	Input::stickDeadZone.RIGHT_AXIS_Y_DOWN = (rightPad_down == -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE : rightPad_down;
}
