// �w�b�_�[�t�@�C���̃C���N���[�h
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include "Input.hpp"


// �萔��`
//#define WINDOW_WIDTH 640 //�E�B���h�E��
//#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME L"�X�v���C�g 2D�X�N���[����� �w�i�F�𔲂�"


// �}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define ALIGN16 _declspec(align(16))


// �K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")


// �x����\��
#pragma warning(disable : 4305)


extern int WINDOW_WIDTH = 640;	//�E�B���h�E��
extern int WINDOW_HEIGHT = 480; //�E�B���h�E����