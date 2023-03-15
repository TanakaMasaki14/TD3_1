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
	void Initialize(Model* model, uint32_t textureHandle);

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

	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;

private:
	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0;

	Vector3 Armspeed_ = { 0.3f,0.3f,0.0f };

	//回転角
	Vector3 radius_ = { 0.0f,0.0f, -30.0f };

	bool movement_ = true;

	//ブロック
	void Block();
	bool block_ = false;

	int blockStartmotionFrame_ = 6;
	int blockChanceFrame_ = 60;
	int blockEndmotionFlame_ = 6;




	float motionspeedX = 0.0f;
	float motionspeedY = 0.0f;

	float bufferpointX = 0.0f;
	float bufferpointY = 0.0f;

	//弱攻撃
	void WeakAttack();
	bool weakAttack_ = false;

	int weakStartmotionFrame_ = 3;
	int weakAttackingFrame_ = 10;
	int weakEndmotionFrame_ = 6;

	//強攻撃
	void HeavyAttack();
	bool heavyAttack_ = false;

	int heavyStartmotionFrame_ = 12;
	int heavyAttackingFrame_ = 10;
	int heavyEndmotionFrame_ = 6;

	//スタン
	void StunAttack();
	bool stunAttack_ = false;

	int stunStartmotionFrame_ = 20;
	int stunAttackingFrame_ = 60;
	int stunEndmotionFrame_ = 12;

};