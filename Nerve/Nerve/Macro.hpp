#ifndef _MACRO_HPP
#define _MACRO_HPP
// 定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME L"スプライト 2Dスクリーン空間 背景色を抜く"


// マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define ALIGN16 _declspec(align(16))
#endif // _MACRO_HPP
