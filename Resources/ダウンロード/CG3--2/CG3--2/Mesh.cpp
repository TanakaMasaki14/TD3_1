#include "Mesh.h"
#include <cassert>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Mesh::device = nullptr;

void Mesh::StaticInitialize(ID3D12Device* device) {
	Mesh::device = device;

	// �}�e���A���̐ÓI������
	Material::StaticInitialize(device);
}

void Mesh::SetName(const std::string& name) { this->name = name; }

void Mesh::AddVertex(const VertexPosNormalUv& vertex) { vertices.emplace_back(vertex); }

void Mesh::AddIndex(unsigned short index) { indices.emplace_back(index); }

void Mesh::SetMaterial(Material* material) { this->material = material; }

void Mesh::CreateBuffers() {
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapPropsVertexBuffer = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDescVertexBuffer = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&heapPropsVertexBuffer, D3D12_HEAP_FLAG_NONE, &resourceDescVertexBuffer,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�̃}�b�s���O
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	std::copy(vertices.begin(), vertices.end(), vertMap);

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapPropsIndexBuffer = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDescIndexBuffer = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&heapPropsIndexBuffer, D3D12_HEAP_FLAG_NONE, &resourceDescIndexBuffer,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�o�b�t�@�̃}�b�s���O
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	std::copy(indices.begin(), indices.end(), indexMap);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList) {
	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(2, material->GetGpuHandle());

	// �}�e���A���̒萔�o�b�t�@���Z�b�g
	ID3D12Resource* constBuff = material->GetConstantBuffer();
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
