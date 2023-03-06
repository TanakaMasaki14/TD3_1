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

	//アームのゲッター,セッター
	Vector3 GetWorldTransform();

	void SetWorldTransform(Vector3 WorldTransform);

	//アームスピードのゲッター,セッター
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

	Vector3 radius_ = { 0.0f,0.0f, -30.0f };

	//ブロック
	void Block();
	bool block_ = false;

	float blockstartmotionframe_ = 6;
	float blockchanceframe_ = 60;
	float blockendmotionflame_ = 6;

};