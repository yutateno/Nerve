#include "Base.hpp"


// 頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
	D3DXVECTOR2 UV; //テクスチャー座標
};


// シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX mW;
	ALIGN16 float ViewPortWidth;
	ALIGN16 float ViewPortHeight;
};


class Main
{
public:
	Main();
	~Main();
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT InitD3D();
	HRESULT InitModel();
	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
	void Run();
	void App();
	void Render();
	void DestroyD3D();

	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pVertexBuffer;

	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
	ID3D11BlendState* m_pBlendState;
};
