#include "DebugText.hpp"



DebugText::DebugText()
{
	ZeroMemory(this, sizeof(DebugText));
	m_fScale = 1.0f;
}


DebugText::~DebugText()
{
}


HRESULT DebugText::Init(ID3D11DeviceContext* pContext, DWORD width, DWORD height, float size, D3DXVECTOR4 vColor)
{
	m_fAlpha = vColor.w;
	m_vColor = vColor;
	m_fScale = size / 25;

	for (int i = 0; i < 95; i++)
	{
		m_fKerning[i] = 10;
	}
	//�f�o�C�X�ƃR���e�L�X�g���R�s�[
	m_pDeviceContext = pContext;
	m_pDeviceContext->GetDevice(&m_pDevice);
	//window �T�C�Y
	m_dwWindowWidth = width;
	m_dwWindowHeight = height;
	//�t�H���g���ƂɃN�A�b�h�쐬
	float left = 0, top = 0, right = 0, bottom = 0;
	int cnt = 0;
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 10; i++)
		{
			left = k * WDIMENSION;
			top = i * DIMENSION;
			right = left + m_fKerning[cnt];
			bottom = top + DIMENSION;
			left /= TEX_DIMENSION;
			top /= TEX_DIMENSION;
			right /= TEX_DIMENSION;
			bottom /= TEX_DIMENSION;
			TextVertex vertices[] =
			{
				D3DXVECTOR3(0,DIMENSION,0),D3DXVECTOR2(left,bottom),//���_1,
				D3DXVECTOR3(0,0,0), D3DXVECTOR2(left,top),//���_2
				D3DXVECTOR3(WDIMENSION,DIMENSION,0),D3DXVECTOR2(right,bottom), //���_3
				D3DXVECTOR3(WDIMENSION,0,0),D3DXVECTOR2(right,top), //���_4
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(TextVertex) * 4;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer[cnt])))
			{
				return E_FAIL;
			}
			cnt++;
		}
	}
	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
	//�t�H���g�̃e�N�X�`���[���쐬
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, L"media\\ascii.png", NULL, NULL, &m_pAsciiTexture, NULL)))
	{
		return E_FAIL;
	}
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"SpriteText.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return FALSE;
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"SpriteText.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(TEXT_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	//�����F�ݒ�
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;
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


void DebugText::Render(char* text, int x, int y)
{
	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1); //�J�����i���_�j�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&m_mView, &vEyePt, &vLookatPt, &vUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DMATRIX mOtho = {
		2.0f / (float)(m_dwWindowWidth), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(m_dwWindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
	m_mProj = mOtho;
	//�g�|���W�[
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//�g�p����V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pAsciiTexture);

	//render
	for (int i = 0; i < strlen(text); i++)
	{
		char font = text[i];
		int index = font - 32;
		RenderFont(index, x, y);
		x += m_fKerning[index];
	}
}


void DebugText::RenderFont(int FontIndex, int x, int y)
{
	//���[���h�ϊ�
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mWorld, x, y, -100);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	TEXT_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = mWorld * m_mView*m_mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//�J���[��n��
		cb.vColor = m_vColor;
		//�����x��n��
		cb.fAlpha.x = m_fAlpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(TextVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer[FontIndex], &stride, &offset);

	//�����F
	UINT ColorKey = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
	m_pDeviceContext->Draw(4, 0);
	m_pDeviceContext->OMSetBlendState(0, NULL, ColorKey);
}