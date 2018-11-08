#pragma once
#include "Base.hpp"
#include "DebugText.hpp"
#include "Draw.hpp"
#include "Character.hpp"


class Main
{
private:
	DebugText* m_pText;
	Draw* m_pDraw;

	void App();			// �A�v���P�[�V���������B�A�v���̃��C���֐��B
	void Render();		// �V�[������ʂɃ����_�����O

	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;

	Character* m_pCharacter;
	

public:
	Main();		// �R���X�g���N�^
	~Main();	// �f�X�g���N�^

	HRESULT InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName);		// �E�B���h�E�쐬
	HRESULT InitD3D();																							// Direct3D������
	LRESULT MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);										// �E�B���h�E�v���V�[�W���[
	void Run();																									// ���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����
	void DestroyD3D();																							// �����[�X
};
