#pragma once
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Audio.h"

class PlayerArm {
public:
	void Initialize(Model* model,uint32_t textureHandle);

	void Update();
	
	void Draw(ViewProjection& viewProjection);

	void Motion();

	//�A�[���̃Q�b�^�[,�Z�b�^�[
	Vector3 GetWorldTransform();

	void SetWorldTransform(Vector3 WorldTransform);

	//�A�[���X�s�[�h�̃Q�b�^�[,�Z�b�^�[
	Vector3 GetSpeed() { return Armspeed_; }

	void SetSpeed(Vector3 Speed);

private:
	Model* model_ = nullptr;
	
	Input* input_ = nullptr;
	
	DebugText* debugText_=nullptr;

	Audio* audio_ = nullptr;

private:
	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0;

	Vector3 Armspeed_ = { 0.3f,0.3f,0.0f };

	//��]�p
	Vector3 radius_ = { 0.0f,0.0f, -30.0f };

	bool movement_ = true;

	//�u���b�N
	void Block();
	bool block_ = false;

	float blockstartmotionframe_ = 6.0f;
	float blockchanceframe_ = 60.0f;
	float blockendmotionflame_ = 6.0f;

	//��U��
	void WeakAttack();
	bool weakAttack_ = false;

	float weakstartmotionframe_ = 3.0f;
	float weakAttackingframe_ = 10.0f;
	float weakendmotionframe_ = 6.0f;

	float motionspeedX = 2.0f;
	float motionspeedY = 4.0f;

	float bufferpointX = 0.0f;
	float bufferpointY = 0.0f;

	//���U��
	void HeavyAttack();
	bool heavyAttack_ = false;


	//�X�^��
	void StunAttack();
	bool stanAttack_ = false;

};