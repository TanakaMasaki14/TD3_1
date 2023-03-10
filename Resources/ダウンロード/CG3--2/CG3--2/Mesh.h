#pragma once

#include "Material.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <wrl.h>

/// <summary>
/// �`��f�[�^
/// </summary>
class Mesh {
private: // �G�C���A�X
  // Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
  // ���_�f�[�^�\���́i�e�N�X�`������j
	struct VertexPosNormalUv {
		XMFLOAT3 pos;    // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;     // uv���W
	};

public: // �ÓI�����o�֐�
  /// <summary>
  /// �ÓI������
  /// </summary>
  /// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

private: // �ÓI�����o�ϐ�
  // �f�o�C�X
	static ID3D12Device* device;

public: // �����o�֐�
  /// <summary>
  /// ���O���擾
  /// </summary>
  /// <returns>���O</returns>
	const std::string& GetName() { return name; }

	/// <summary>
	/// ���O���Z�b�g
	/// </summary>
	/// <param name="name">���O</param>
	void SetName(const std::string& name);

	/// <summary>
	/// ���_�f�[�^�̒ǉ�
	/// </summary>
	/// <param name="vertex">���_�f�[�^</param>
	void AddVertex(const VertexPosNormalUv& vertex);

	/// <summary>
	/// ���_�C���f�b�N�X�̒ǉ�
	/// </summary>
	/// <param name="index">�C���f�b�N�X</param>
	void AddIndex(unsigned short index);

	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	/// <returns>�}�e���A��</returns>
	Material* GetMaterial() { return material; }

	/// <summary>
	/// �}�e���A���̊��蓖��
	/// </summary>
	/// <param name="material">�}�e���A��</param>
	void SetMaterial(Material* material);

	/// <summary>
	/// �o�b�t�@�̐���
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// ���_�o�b�t�@�擾
	/// </summary>
	/// <returns>���_�o�b�t�@</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; }

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�擾
	/// </summary>
	/// <returns>�C���f�b�N�X�o�b�t�@</returns>
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; }

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">���ߔ��s��R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
  // ���O
	std::string name;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// �}�e���A��
	Material* material = nullptr;
	// ���_�o�b�t�@�̃}�b�v
	VertexPosNormalUv* vertMap = nullptr;
	// �C���f�b�N�X�o�b�t�@�̃}�b�v
	unsigned short* indexMap = nullptr;
};
