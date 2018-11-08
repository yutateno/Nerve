// �w�b�_�[�t�@�C���̃C���N���[�h
#include <Windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <stdio.h>
#include "Input.hpp"
#include <string>
#include <vector>


// �萔��`
#define APP_NAME L"Nerve"


// �}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define SAFE_DEL_RELEASE(x) if(x){delete x; x=NULL;}
#define ALIGN16 _declspec(align(16))
#define DIMENSION 12.0f
#define WDIMENSION 10.0f
#define TEX_DIMENSION 128.0f


// �K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")


// �x����\��
#pragma warning(disable : 4305)

#ifndef _BASE_HPP
#define _BASE_HPP
static int WINDOW_WIDTH = 1920;	//�E�B���h�E��
static int WINDOW_HEIGHT = 1080; //�E�B���h�E����
#endif // _BASE_HPP
