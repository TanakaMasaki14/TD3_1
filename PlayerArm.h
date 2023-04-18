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
	~PlayerArm();

	void Initialize(Model* model,Model* modelFace,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Motion();



	void GetBlock();

	void GetWeak();

	void GetHeavy();

	void GetStun();

	//アームのゲッター,セッター
	Vector3 GetWorldTransform();

	void SetWorldTransform(Vector3 WorldTransform);


	//ゲッター
	Vector3 GetWorldTransformPlayerCollision0();
	Vector3 GetWorldTransformPlayerCollision1();
	Vector3 GetWorldTransformPlayerCollision2();
	Vector3 GetWorldTransformPlayerCollision3();
	Vector3 GetWorldTransformPlayerCollision4();
	Vector3 GetWorldTransformPlayerCollision5();
	Vector3 GetWorldTransformPlayerCollision6();

	Vector3 GetWorldTransformPlayerAttackCollision0();
	Vector3 GetWorldTransformPlayerAttackCollision1();
	Vector3 GetWorldTransformPlayerAttackCollision2();
	Vector3 GetWorldTransformPlayerAttackCollision3();
	Vector3 GetWorldTransformPlayerAttackCollision4();
	Vector3 GetWorldTransformPlayerAttackCollision5();
	Vector3 GetWorldTransformPlayerAttackCollision6();
	Vector3 GetWorldTransformPlayerAttackCollision7();
	Vector3 GetWorldTransformPlayerAttackCollision8();
	Vector3 GetWorldTransformPlayerAttackCollision9();

	Vector3 GetScalePlayerCollision();

	Vector3 GetScalePlayerAttackCollision();

	const bool& GetIsBlock() const { return block_; }

	const bool& GetIsWeak() const { return weakAttack_; }

	const bool& GetIsHeavy() const { return heavyAttack_; }

	const bool& GetIsStun() const { return stunAttack_; }

	const bool& GetIsMovement() const { return movement_; }

	//アームスピードのゲッター,セッター
	Vector3 GetSpeed() { return Armspeed_; }

	void SetSpeed(Vector3 Speed);

private:
	Model* model_ = nullptr;

	Model* modelFace_ = nullptr;

	Model* modelPlayerCollision_ = nullptr;

	Model* modelAttackRange_ = nullptr;

	Model* modelAttackCollision_ = nullptr;

	Input* input_ = nullptr;

	DebugText* debugText_=nullptr;

	Audio* audio_ = nullptr;

private:
	WorldTransform worldTransform_;

	WorldTransform worldTransformFace_;

	const static int PlayerCollisionquantity = 7;

	WorldTransform worldTransformPlayerCollision_[PlayerCollisionquantity];

	WorldTransform worldTransformAttackrange_;

	WorldTransform worldTransformAttackCollision_[10];

	uint32_t textureHandle_ = 0;

	uint32_t TesttextureHandle_ = 0;
	bool testhit = false;
	int testTime = 30;


	Vector3 Armspeed_ = { 0.3f,0.3f,0.0f };

	//回転角
	Vector3 radius_ = { 0.0f,0.0f, -30.0f };

	//距離
	float r[PlayerCollisionquantity - 1];

	//ラジアン角度
	double radian;

	bool movement_ = true;

	//攻撃範囲
	bool attackrange_ = false;

	float attackbufferX_ = 0.0f;
	float attackbufferY_ = 0.0f;

	//ブロック
	void Block();
	bool block_ = false;

	int blockStartmotionFrame_ = 6;
	int blockChanceFrame_ = 60;
	int blockEndmotionFlame_ = 6;

	//ブロックされた
	void GetBlockMotion();
	bool getblock_ = false;

	Vector2 getblockbufferpoint_;

	int getblockFrame_ = 60;


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


	//体力
	int hp = 0;


};