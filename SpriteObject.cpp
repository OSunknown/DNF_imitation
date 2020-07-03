#include "stdafx.h"
#include "SpriteObject.h"
SpriteObject::SpriteObject()
{
	_isTransfromChange = false;
	UIType = false;
	_vertexBuffer = NULL;
	_indexBuffer = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
	left = 0; 
	right = 0; 
	top = 0; 
	bottom = 0;
	_currentsprite = NULL;
}

SpriteObject::~SpriteObject()
{
}

void SpriteObject::Init()
{
	InitializeBuffers();
	_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	_isTransfromChange = true;
}

void SpriteObject::Init(string name, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	InitializeBuffers();
	_currentsprite = IMAGEMANAGER.GetSceneSprite(name);
	_position = position;
	_rotation = rotation;
	_scale = scale;

	_isTransfromChange = true;
	assert(_currentsprite);
}

void SpriteObject::Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	InitializeBuffers();
	_position = position;
	_rotation = rotation;
	_scale = scale;

	_isTransfromChange = true;
}

void SpriteObject::Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale, Animator controller)
{
	InitializeBuffers();
	_position = position;
	_rotation = rotation;
	_scale = scale;
	
	_isTransfromChange = true;
}

bool SpriteObject::Render()
{
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	SetRect();
	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

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
	
	
	// Lock the vertex buffer so it can be written to.
	result = DxM.GetDeviceContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	DxM.GetDeviceContext()->Unmap(_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	RenderBuffers();

	Renderer.TextureShader(GetIndexCount(), GetWorldMatrix(),GetTexture());

	return true;
}

void SpriteObject::Shutdown()
{
	_vertexBuffer->Release();
	_indexBuffer->Release();
}

void SpriteObject::SetPosition(D3DXVECTOR3 position)
{
	_isTransfromChange = true;
	_position = position;
}

void SpriteObject::SetRotation(D3DXVECTOR3 rotation)
{
	_isTransfromChange = true;
	_rotation = rotation;
}

void SpriteObject::SetScale(D3DXVECTOR3 scale)
{
	_isTransfromChange = true;
	_scale = scale;
}

void SpriteObject::SetSprite(sprite* texture)
{
	_currentsprite = texture;
}

void SpriteObject::AddOriginalPosition(D3DXVECTOR3 position)
{
	_isTransfromChange = true;
	_position += position;
}
int SpriteObject::GetIndexCount()
{
	return m_indexCount;
}

D3DXVECTOR3 SpriteObject::GetPosition()
{
	return _position;
}

D3DXMATRIX SpriteObject::GetWorldMatrix()
{
	if (_isTransfromChange == false)
		return _worldMatrix;
	
	D3DXMatrixIdentity(&_worldMatrix);

	D3DXMATRIX SMatrix, RMatrix, TMatrix;
	
	D3DXMatrixScaling(&SMatrix, _scale.x, _scale.y, _scale.z);
	D3DXVECTOR3 rotation = _rotation * 3.141592f / 180.0f;
	D3DXMatrixRotationYawPitchRoll(&RMatrix, rotation.y, rotation.x, rotation.z);
	
	
	//
	if (UIType == false)
	{
		D3DXMatrixTranslation(&TMatrix, _position.x, _position.y, _position.z);
	}
	else
	{
		D3DXMatrixTranslation(&TMatrix, 0, 0, 0);
	}
	_worldMatrix = SMatrix * RMatrix * TMatrix;
	_isTransfromChange = false;

	return _worldMatrix;
}

ID3D11ShaderResourceView * SpriteObject::GetTexture()
{
	return _currentsprite->texture;
}

bool SpriteObject::InitializeBuffers()
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = DxM.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
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

void SpriteObject::RenderBuffers()
{
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
	return;
}

void SpriteObject::SetRect()
{
	float width = _currentsprite->width;// *_scale.x;
	float height = _currentsprite->height;// *_scale.y;

	if (Filp)
	{
		// Calculate the screen coordinates of the left side of the bitmap.
		left = ((width *(1-_currentsprite->pivot.x))*-1) - _currentsprite->offset.x;

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
	top = ((height* _currentsprite->pivot.y))-_currentsprite->offset.y;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - height;

	//cout << _position.x << "/" << _position.y << "//" << left << "/" << right << "/"<< top << "/" << bottom << endl;

	if (UIType == true)
	{
		//UI로 전환될때 화면의 왼쪽 아래 부분을 기준(0,0)으로 잡는다.
		float screenWidth = App.DxscreenWidth / 2.0f;
		float screenHeight = App.DxscreenHeight / 2.0f;
		left += MainCamera.GetPosition().x + _position.x - screenWidth;
		right += MainCamera.GetPosition().x + _position.x - screenWidth;
		top += MainCamera.GetPosition().y + _position.y - screenHeight;
		bottom += MainCamera.GetPosition().y + _position.y - screenHeight;
	}
	
}
