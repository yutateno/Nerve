// ヘッダーファイルのインクルード
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include "Input.hpp"


// 定数定義
//#define WINDOW_WIDTH 640 //ウィンドウ幅
//#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME L"スプライト 2Dスクリーン空間 背景色を抜く"


// マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define ALIGN16 _declspec(align(16))


// 必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")


// 警告非表示
#pragma warning(disable : 4305)


extern int WINDOW_WIDTH = 640;	//ウィンドウ幅
extern int WINDOW_HEIGHT = 480; //ウィンドウ高さ