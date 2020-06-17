#include "stdafx.h"
#include "BackgroundSpriteObject.h"

BackgroundSpriteObject::BackgroundSpriteObject()
{
	_vertexCount = 0;
	_indexCount = 0;
	_vertexBuffer = NULL;
	_indexBuffer = NULL;
	_instanceBuffer = NULL;
	left = 0;
	right = 0;
	top = 0; 
	bottom = 0;
	_instanceCount = 0;
	_vertexBuffer = 0;
	_instanceBuffer = 0;
	_currentsprite = 0;
}

BackgroundSpriteObject::~BackgroundSpriteObject()
{
}

void BackgroundSpriteObject::Awake()
{

}

void BackgroundSpriteObject::Start()
{

}

void BackgroundSpriteObject::Update()
{
}

void BackgroundSpriteObject::FixedUpdate()
{
}

void BackgroundSpriteObject::Frame()
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = _vertexBuffer;
	bufferPointers[1] = _instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	DxM.GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	DxM.GetDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	DxM.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer.SpriteShader(GetIndexCount(), GetInstanceCount(), GetWorldMatrix(), GetTexture());

	return;
}

void BackgroundSpriteObject::Shutdown()
{
	// Release the instance buffer.
	if (_instanceBuffer)
	{
		_instanceBuffer->Release();
		_instanceBuffer = 0;
	}

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

void BackgroundSpriteObject::SetTexture(string name, int depth)
{
	_currentsprite = IMAGEMANAGER.GetSceneSprite(name);
	assert(_currentsprite);
	_depth = depth;
}
//복제되는 이미지 위치들.
bool BackgroundSpriteObject::InitializeInstance(vector<D3DXVECTOR3> positions)
{
	InstanceType* instances;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	HRESULT result;

	_instanceCount = (int)positions.size();
	
	// Create the instance array.
	instances = new InstanceType[_instanceCount];
	if (!instances)
	{
		return false;
	}

	for (int i = 0; i < _instanceCount; i++)
	{
		instances[i].position = positions[i];
		instances[i].position *= _originalsize;
		instances[i].position.z = 0;
	}

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * _instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;
	
	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = DxM.GetDevice()->CreateBuffer(&instanceBufferDesc, &instanceData, &_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete[] instances;
	instances = 0;

	return true;
}

bool BackgroundSpriteObject::InitializeBuffers()
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

	if (Filp)
	{
		// Calculate the screen coordinates of the left side of the bitmap.
		left = ((width *(1 - _currentsprite->pivot.x))*-1) - _currentsprite->offset.x;

		// Calculate the screen coordinates of the right side of the bitmap.
		right = left + width;
	}
	else
	{
		// Calculate the screen coordinates of the left side of the bitmap.
		left = ((width *_currentsprite->pivot.x)*-1) + _currentsprite->offset.x;

		// Calculate the screen coordinates of the right side of the bitmap.
		right = left + width;
	}


	// Calculate the screen coordinates of the top of the bitmap.
	top = ((height* _currentsprite->pivot.y)) - _currentsprite->offset.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - height;

	memset(vertices, 0, (sizeof(VertexType) * _vertexCount));

	if (Filp)
	{
		// Load the vertex array with data.
	// First triangle.
		vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		vertices[0].texture = D3DXVECTOR2(1.0f, 0.0f);

		vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		vertices[1].texture = D3DXVECTOR2(0.0f, 1.0f);

		vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
		vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

		// Second triangle.
		vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		vertices[3].texture = D3DXVECTOR2(1.0f, 0.0f);

		vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
		vertices[4].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		vertices[5].texture = D3DXVECTOR2(0.0f, 1.0f);
	}
	else
	{
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
	}

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
int BackgroundSpriteObject::GetVertexCount()
{
	return _vertexCount;
}

int BackgroundSpriteObject::GetInstanceCount()
{
	return _instanceCount;
}

int BackgroundSpriteObject::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView * BackgroundSpriteObject::GetTexture()
{
	return _currentsprite->texture;
}

