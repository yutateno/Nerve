#pragma once
#include "Base.hpp"


// ���_�̍\����
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //�ʒu
	D3DXVECTOR2 UV; //�e�N�X�`���[���W
};


// �V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX mW;
	ALIGN16 float ViewPortWidth;
	ALIGN16 float ViewPortHeight;
};


class Draw
{
private:
	HRESULT InitModel(float xSize, float ySize, bool flipHorizontal, bool flipVertical);
	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
	void RenderSprite(D3DXMATRIX& World, ID3D11ShaderResourceView* pTexture);

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pVertexBuffer;

	DWORD m_dwWindowWidth;
	DWORD m_dwWindowHeight;
	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	std::vector<ID3D11ShaderResourceView*> m_pTexture;//�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
	ID3D11BlendState* m_pBlendState;


public:
	Draw();
	~Draw();
	HRESULT Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height, LPCWSTR* p_fileName, const int num, float xSize, float ySize, bool flipHorizontal, bool flipVertical);
	void Render(int num, float x, float y);
};
