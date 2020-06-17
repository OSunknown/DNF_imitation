#include "stdafx.h"
#include "BackgroundAniObject.h"


BackgroundAniObject::BackgroundAniObject()
{
	 _currentsprite = NULL;

	_vertexCount = 0;
	_indexCount = 0;
	_vertexBuffer = NULL;
	_indexBuffer = NULL;
	left = 0.0f;
	right = 0.0f;
	top = 0.0f;
	bottom = 0.0f;

	_currentAnimation = NULL;
	_frameCount = 0;
	_spriteNumber = 0;
}


BackgroundAniObject::~BackgroundAniObject()
{
}

void BackgroundAniObject::Awake()
{
}

void BackgroundAniObject::Start()
{
}

void BackgroundAniObject::Update()
{
}

void BackgroundAniObject::FixedUpdate()
{
}

void BackgroundAniObject::Frame()
{
	if (_spriteNumber == _currentAnimation->frame.size() - 1 && _currentAnimation->isLoop == false)
		return;

	if (_spriteNumber == _currentAnimation->frame.size() - 1 && _currentAnimation->isLoop == true)
	{
		_spriteNumber = 0;
	}
	if (_currentAnimation->frame[_spriteNumber] < _frameCount)
	{
		_frameCount = _frameCount - _currentAnimation->frame[_spriteNumber];
		_spriteNumber++;
	}
	_currentsprite = _currentAnimation->texture[_spriteNumber];
	_frameCount += Time.GetDeltaTime();

	///

	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[1];

	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = _vertexBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	DxM.GetDeviceContext()->IASetVertexBuffers(0, 1, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	DxM.GetDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	DxM.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer.TextureShader(GetIndexCount(),GetWorldMatrix(), GetTexture());
}

void BackgroundAniObject::Shutdown()
{
	// Release the index buffer.
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}
}

bool BackgroundAniObject::InitializeBuffers()
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	_vertexCount = 6;

	_indexCount = _vertexCount;

	vertices = new VertexType[_vertexCount];
	if (!vertices)
	{
		return false;
	}

	float width = _currentsprite->width;// *_scale.x;
	float height = _currentsprite->height;// *_scale.y;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = ((width *_currentsprite->pivot.x)*-1) + _currentsprite->offset.x;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + width;


	// Calculate the screen coordinates of the top of the bitmap.
	top = ((height* _currentsprite->pivot.y)) - _currentsprite->offset.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - height;

	memset(vertices, 0, (sizeof(VertexType) * _vertexCount));

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);


	//정점 버퍼에 대한 설정
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	//하위 리소스 구조에 정점 데이터에 대한 포인터를 제공합니다.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	//정점 버퍼 생성
	result = DxM.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indices = new unsigned long[_indexCount];

	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < _indexCount; i++)
	{
		indices[i] = i;
	}
	//정적 인덱스 버퍼를 설정
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	//서브 리소스 구조에 인덱스 데이터에 대한 포인터를 제공
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = DxM.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

int BackgroundAniObject::GetVertexCount()
{
	return _vertexCount;
}

int BackgroundAniObject::GetIndexCount()
{
	return _indexCount;
}

void BackgroundAniObject::SetAnimation(string name, int depth)
{
	_currentAnimation = IMAGEMANAGER.GetBgAnimation(name);
	_currentsprite = _currentAnimation->texture[0];
	_depth = depth;	
}

ID3D11ShaderResourceView * BackgroundAniObject::GetTexture()
{
	return _currentsprite->texture;
}
