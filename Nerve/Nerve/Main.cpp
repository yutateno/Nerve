#include "Main.hpp"


// -----------------------------------------------------------------------------------------------------------
/// �O���[�o���ϐ�
Main* g_pMain = NULL;


// -----------------------------------------------------------------------------------------------------------
/// �֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// -----------------------------------------------------------------------------------------------------------
// �A�v���P�[�V�����̃G���g���[�֐� 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_pMain = new Main;
	InputPad::InputPad();
	InputPad::SetPlayerPadNum(0);


	if (g_pMain != NULL)
	{
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))
		{
			if (SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Run();
			}
		}


		/// �A�v���I��
		g_pMain->DestroyD3D();
		delete g_pMain;
	}


	return 0;
}


// -----------------------------------------------------------------------------------------------------------
// OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����MAIN�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}



// -----------------------------------------------------------------------------------------------------------
Main::Main()
{
	ZeroMemory(this, sizeof(Main));
}


// -----------------------------------------------------------------------------------------------------------
Main::~Main()
{
}


// -----------------------------------------------------------------------------------------------------------
HRESULT Main::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	/// �E�B���h�E�̒�`
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize				 = sizeof(wc);
	wc.style				 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc			 = WndProc;
	wc.hInstance			 = hInstance;
	wc.hIcon				 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor				 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground		 = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName		 = WindowName;
	wc.hIconSm				 = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	RECT rec;
	m_hWnd = GetDesktopWindow();
	GetClientRect(m_hWnd, &rec);
	WINDOW_WIDTH = rec.right;
	WINDOW_HEIGHT = rec.bottom;


	/// �E�B���h�E�̍쐬
	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, rec.right, rec.bottom, 0, 0, hInstance, 0);
	if (!m_hWnd)
	{
		return E_FAIL;
	}


	/// �E�C���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}


// -----------------------------------------------------------------------------------------------------------
LRESULT Main::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}


// -----------------------------------------------------------------------------------------------------------
void Main::Run()
{
	/// ���b�Z�[�W���[�v
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/// �A�v���P�[�V�����̏����͂��������ԁB
			App();
		}
	}
	/// �A�v���P�[�V�����̏I��
}


// -----------------------------------------------------------------------------------------------------------
void Main::App()
{
	InputPad::FirstUpdate();
	Render();
	m_pCharacter->Process();
}


// -----------------------------------------------------------------------------------------------------------
HRESULT Main::InitD3D()
{
	/// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							 = 1;
	sd.BufferDesc.Width						 = WINDOW_WIDTH;
	sd.BufferDesc.Height					 = WINDOW_HEIGHT;
	sd.BufferDesc.Format					 = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		 = 60;
	sd.BufferDesc.RefreshRate.Denominator	 = 1;
	sd.BufferUsage							 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							 = m_hWnd;
	sd.SampleDesc.Count						 = 1;
	sd.SampleDesc.Quality					 = 0;
	sd.Windowed								 = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}


	// �e��e�N�X�`���[�ƁA����ɕt�т���e��r���[���쐬

	/// �o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	/// ���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	/// �f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					 = WINDOW_WIDTH;
	descDepth.Height				 = WINDOW_HEIGHT;
	descDepth.MipLevels				 = 1;
	descDepth.ArraySize				 = 1;
	descDepth.Format				 = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count		 = 1;
	descDepth.SampleDesc.Quality	 = 0;
	descDepth.Usage					 = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags				 = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags		 = 0;
	descDepth.MiscFlags				 = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBackBuffer_DSTex);
	/// ���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);


	/// �����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	/// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width		 = static_cast<FLOAT>(WINDOW_WIDTH);
	vp.Height		 = static_cast<FLOAT>(WINDOW_HEIGHT);
	vp.MinDepth		 = 0.0f;
	vp.MaxDepth		 = 1.0f;
	vp.TopLeftX		 = 0;
	vp.TopLeftY		 = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	/// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode				 = D3D11_CULL_NONE;
	rdc.FillMode				 = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise	 = TRUE;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);

	
	/// �����񃌃��_�����O�̏�����
	m_pText = new DebugText;
	D3DXVECTOR4 vColor(0.9, 0.6, 0, 1);
	if (FAILED(m_pText->Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT, 50, vColor)))
	{
		return E_FAIL;
	}


	m_pCharacter = new Character;


	/// �Q�[���̑f�ނ̏�����
	m_pDraw = new Draw;
	const int fileNum = 24;
	LPCWSTR fileName[] = { L"media\\clph_2d\\clph\\1_1.png" , L"media\\clph_2d\\clph\\1_2.png", L"media\\clph_2d\\clph\\1_3.png"
						 , L"media\\clph_2d\\clph\\2_1.png" , L"media\\clph_2d\\clph\\2_2.png", L"media\\clph_2d\\clph\\2_3.png" 
						 , L"media\\clph_2d\\clph\\3_1.png" , L"media\\clph_2d\\clph\\3_2.png", L"media\\clph_2d\\clph\\3_3.png" 
						 , L"media\\clph_2d\\clph\\4_1.png" , L"media\\clph_2d\\clph\\4_2.png", L"media\\clph_2d\\clph\\4_3.png" 
						 , L"media\\clph_2d\\clph\\6_1.png" , L"media\\clph_2d\\clph\\6_2.png", L"media\\clph_2d\\clph\\6_3.png" 
						 , L"media\\clph_2d\\clph\\7_1.png" , L"media\\clph_2d\\clph\\7_2.png", L"media\\clph_2d\\clph\\7_3.png" 
						 , L"media\\clph_2d\\clph\\8_1.png" , L"media\\clph_2d\\clph\\8_2.png", L"media\\clph_2d\\clph\\8_3.png" 
						 , L"media\\clph_2d\\clph\\9_1.png" , L"media\\clph_2d\\clph\\9_2.png", L"media\\clph_2d\\clph\\9_3.png" };
	if (FAILED(m_pDraw->Init(m_pDeviceContext, WINDOW_WIDTH, WINDOW_HEIGHT, fileName, fileNum, 64, 64, false, false)))
	{
		return E_FAIL;
	}


	/// �t���X�N���[���ɂ���iDirectX11�Ȃ̂�ALT+Enter�Ő؂�ւ���
	m_pSwapChain->SetFullscreenState(true, 0);


	return S_OK;
}


// -----------------------------------------------------------------------------------------------------------
void Main::Render()
{
	/// ��ʃN���A
	float ClearColor[4] = { 0,1,1,1 };	/// �N���A�F�쐬�@RGBA�̏�
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);						/// �J���[�o�b�t�@�N���A
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);	/// �f�v�X�X�e���V���o�b�t�@�N���A

	m_pDraw->Render(m_pCharacter->GetID(), m_pCharacter->GetX(), m_pCharacter->GetY());

	char str[256];
	sprintf(str, "%d", InputPad::GetPadThumbData(XINPUT_PAD::NUM01, XINPUT_PAD::STICK_LEFT_X));
	m_pText->Render(str, 0, 10);


	m_pSwapChain->Present(0, 0);/// ��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
}


// -----------------------------------------------------------------------------------------------------------
void Main::DestroyD3D()
{
	SAFE_DEL_RELEASE(m_pText);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
