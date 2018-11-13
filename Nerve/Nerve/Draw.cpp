#include "Draw.hpp"


// �R���X�g���N�^
Draw::Draw()
{
	ZeroMemory(this, sizeof(Draw));
}


// �f�X�g���N�^
Draw::~Draw()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
}


// Direct3D������
HRESULT Draw::Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height, LPCWSTR* p_fileName, const int num, float xSize, float ySize)
{
	//�f�o�C�X�ƃR���e�L�X�g���R�s�[
	m_pDeviceContext = pContext;
	m_pDeviceContext->GetDevice(&m_pDevice);
	//window �T�C�Y
	m_dwWindowWidth = width;
	m_dwWindowHeight = height;

	// hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	// �o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(MakeShader("SpriteTrans.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader))) return E_FAIL;
	// ���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	SAFE_RELEASE(pCompiledShader);
	// �s�N�Z���V�F�[�_�[�쐬
	if (FAILED(MakeShader("SpriteTrans.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);
	// �R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}

	// �o�[�e�b�N�X�o�b�t�@�[�쐬
	if (FAILED(InitModel(xSize, ySize)))
	{
		return E_FAIL;
	}
	// �e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampler);
	// �e�N�X�`���[�ǂݍ���
	m_pTexture.resize(num);
	for (int i = 0; i != num; ++i)
	{
		if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, p_fileName[i], NULL, NULL, &m_pTexture[i], NULL)))
		{
			MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
			return E_FAIL;
		}
	}
	//}
	//if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, fileName, NULL, NULL, &m_pTexture[0], NULL)))
	//{
	//	MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
	//	return E_FAIL;
	//}
	//// �e�N�X�`���[�ǂݍ���
	//if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, fileName, NULL, NULL, &m_pTexture[1], NULL)))
	//{
	//	MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
	//	return E_FAIL;
	//}
	// �A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	// png�t�@�C�����ɃA���t�@��񂪂���B�A���t�@�ɂ�蓧�߂���悤�w�肵�Ă���
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = true;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendState)))
	{
		return E_FAIL;
	}

	return S_OK;
}


// hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT Draw::MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)// Vertex Shader
	{
		if (FAILED(m_pDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ps") == 0)// Pixel Shader
	{
		if (FAILED(m_pDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "gs") == 0)// Geometry Shader
	{
		if (FAILED(m_pDevice->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "hs") == 0)// Hull Shader
	{
		if (FAILED(m_pDevice->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ds") == 0)// Domain Shader
	{
		if (FAILED(m_pDevice->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "cs") == 0)// Compute Shader
	{
		if (FAILED(m_pDevice->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader))) return E_FAIL;
	}
	return S_OK;
}


// �|���S���A���b�V���Ȃǂ̃W�I���g���֘A��������
HRESULT Draw::InitModel(float xSize, float ySize)
{
	// �o�[�e�b�N�X�o�b�t�@�[�쐬
	// �C�����邱�ƁBz�l���P�ȏ�ɂ��Ȃ��B�N���b�v��Ԃ�z=1�͍ł������Ӗ�����B���������ĕ`�悳��Ȃ��B
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(0		,0		,0),D3DXVECTOR2(0 ,0),//���_1,
		D3DXVECTOR3(0		,ySize	,0),D3DXVECTOR2(0 ,1),//���_2
		D3DXVECTOR3(xSize	,0		,0),D3DXVECTOR2(1 ,0), //���_3
		D3DXVECTOR3(xSize	,ySize	,0),D3DXVECTOR2(1 ,1), //���_4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}


// �V�[������ʂɃ����_�����O
void Draw::Render(int num, float x, float y, float xSize, float ySize, bool flipHorizontal, bool flipVertical)
{
	D3DXMATRIX World;
	D3DXMATRIX Turn;
	D3DXMatrixScaling(&Turn, flipHorizontal ? -1.0f : 1.0f, flipVertical ? -1.0f : 1.0f, 1.0f);
	D3DXMATRIX Tran;
	D3DXMatrixTranslation(&Tran, flipHorizontal ? x + 64 : x, flipVertical ? y + 64 : y, 0);
	World = Turn * Tran;
	RenderSprite(World, m_pTexture[num]);
}


void Draw::RenderSprite(D3DXMATRIX& World, ID3D11ShaderResourceView* pTexture)
{
	// �g�p����V�F�[�_�[�̃Z�b�g
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ���[���h�s���n��
		cb.mW = World;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		// �r���[�|�[�g�T�C�Y��n���i�N���C�A���g�̈�̉��Əc�j
		cb.ViewPortWidth = static_cast<float>(WINDOW_WIDTH);
		cb.ViewPortHeight = static_cast<float>(WINDOW_HEIGHT);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	// ���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	// ���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	// �v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// �e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pDeviceContext->PSSetShaderResources(0, 1, &pTexture);


	// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�������_�����O
	UINT ColorKey = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
	m_pDeviceContext->Draw(4, 0);
	m_pDeviceContext->OMSetBlendState(0, NULL, ColorKey);
}