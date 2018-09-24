#pragma once

#ifndef _MY_INPUT_H

#include <Windows.h>
#include <Xinput.h>
#include <math.h>

#pragma comment(lib, "xinput.lib")

#endif // !_MY_INPUT_H

namespace XINPUT_PAD
{
	// 番号
	const unsigned __int8 PAD_NUM01 = 0;
	const unsigned __int8 PAD_NUM02 = 1;
	const unsigned __int8 PAD_NUM03 = 2;
	const unsigned __int8 PAD_NUM04 = 3;

	// ボタン
	const unsigned __int8 CROSS_BUTTON_UP = 0;
	const unsigned __int8 CROSS_BUTTON_DOWN = 1;
	const unsigned __int8 CROSS_BUTTON_LEFT = 2;
	const unsigned __int8 CROSS_BUTTON_RIGHT = 3;
	const unsigned __int8 BUTTON_START = 4;
	const unsigned __int8 BUTTON_BACK = 5;
	const unsigned __int8 STICK_PUSH_LEFT = 6;
	const unsigned __int8 STICK_PUSH_RIGHT = 7;
	const unsigned __int8 SHOULDER_LEFT = 8;
	const unsigned __int8 SHOULDER_RIGHT = 9;
	const unsigned __int8 BUTTON_A = 12;
	const unsigned __int8 BUTTON_B = 13;
	const unsigned __int8 BUTTON_X = 14;
	const unsigned __int8 BUTTON_Y = 15;

	// トリガ
	const bool TRIGGER_RIGHT = 0;
	const bool TRIGGER_LEFT = 1;

	// スティック
	const unsigned __int8 STICK_RIGHT_AXIS_X = 0;
	const unsigned __int8 STICK_RIGHT_AXIS_Y = 1;
	const unsigned __int8 STICK_LEFT_AXIS_X = 2;
	const unsigned __int8 STICK_LEFT_AXIS_Y = 3;
}

struct XINPUT_STICK_MY_DEADZONE
{
	// 左スティックの左右
	short LEFT_AXIS_X_RIGHT = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	short LEFT_AXIS_X_LEFT = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	// 左スティックの上下
	short LEFT_AXIS_Y_UP = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	short LEFT_AXIS_Y_DOWN = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	// 右スティックの左右
	short RIGHT_AXIS_X_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	short RIGHT_AXIS_X_LEFT = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

	// 右スティックの上下
	short RIGHT_AXIS_Y_UP = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	short RIGHT_AXIS_Y_DOWN = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
};

class Input
{
private:
	static unsigned __int8 controllerNum;		// 接続している最大の個数

	static int button[4][16];					// wButtonの対応
	static int stick[4][4];						// stickの対応(公式だとthumb)

	static XINPUT_STATE state[4];				// xinputの中身

	static XINPUT_STICK_MY_DEADZONE stickDeadZone;			// スティックのデッドゾーン値

public:
	Input();		// コンストラクタ
	~Input();		// デストラクタ

	static void Update();		// 操作更新



	static int GetPadNum();																// コントローラの数
	static int GetPadButtonData(unsigned __int8 use_padnum, unsigned __int8 use_button);			// コントローラのボタン操作
	static int GetPadTriggerData(unsigned __int8 use_padnum, bool use_Is_triggerLeft);				// コントローラのトリガー操作
	static int GetPadThumbData(unsigned __int8 use_padnum, unsigned __int8 use_stick);			// コントローラのスティック操作
	static void SetPadDeadZone(short leftPad_right = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, short leftPad_left = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		, short leftPad_up = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, short leftPad_down = -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		, short rightPad_right = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, short rightPad_left = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
		, short rightPad_up = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, short rightPad_down = -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);		// デッドゾーンの設定値変更  // (ある程度楽したいので一応デフォルト引数乱用)
};
