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

class PlayerArm;


class Enemy
{
public:
	~Enemy();

	void Initialize(Model* model, Model* modelFace, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Motion();

	void MoveCalculation();

	//当たり判定

	void GetBlock();

	void GetWeak();

	void GetHeavy();

	void GetStun();

	///ゲッター
	//カス
	Vector3 GetWorldTransformEnemyCollision0();
	Vector3 GetWorldTransformEnemyCollision1();
	Vector3 GetWorldTransformEnemyCollision2();
	Vector3 GetWorldTransformEnemyCollision3();
	Vector3 GetWorldTransformEnemyCollision4();
	Vector3 GetWorldTransformEnemyCollision5();
	Vector3 GetWorldTransformEnemyCollision6();

	//カス
	Vector3 GetWorldTransformEnemyAttackCollision0();
	Vector3 GetWorldTransformEnemyAttackCollision1();
	Vector3 GetWorldTransformEnemyAttackCollision2();
	Vector3 GetWorldTransformEnemyAttackCollision3();
	Vector3 GetWorldTransformEnemyAttackCollision4();
	Vector3 GetWorldTransformEnemyAttackCollision5();
	Vector3 GetWorldTransformEnemyAttackCollision6();
	Vector3 GetWorldTransformEnemyAttackCollision7();
	Vector3 GetWorldTransformEnemyAttackCollision8();
	Vector3 GetWorldTransformEnemyAttackCollision9();

	const bool& GetIsBlock() const { return block_; }

	const bool& GetIsWeak() const { return weakAttack_; }

	const bool& GetIsHeavy() const { return heavyAttack_; }

	const bool& GetIsStun() const { return stunAttack_; }

	int GetFase() { return movementFase_; }

	Vector3 GetScaleEnemyCollision();

	Vector3 GetScaleEnemyAttackCollision();

	void SetPlayerArm(PlayerArm* playerArm) {
		playerarm_ = playerArm;
	}

	int GetHp() { return hp; }

	void ReInitialize();

	//攻撃力ゲッター
	int GetWeakPower() { return weakAttackPower_; }

	int GetHeavyPower() { return heavyAttackPower_; }

	/// <summary>
	/// ベースタイマー再設定(引数は秒数(小数点可))
	/// </summary>
	void SetWaitBaseTimer(float waitBaseTime) {
		waitbaseTime_ = waitBaseTime * 60;
	}

	int GetInvincibleTime_() { return invincibleTime_; }

	void SetInvincibleTime_(int invincibleTime) {
		invincibleTime_ = invincibleTime;
	}

private:
	Model* model_ = nullptr;

	Model* modelFace_ = nullptr;

	Model* modelEnemyCollision_ = nullptr;

	Model* modelAttackRange_ = nullptr;

	Model* modelAttackCollision_ = nullptr;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;

	PlayerArm* playerarm_ = nullptr;
private:
	WorldTransform worldTransform_;

	WorldTransform worldTransformFace_;

	const static int EnemyCollisionquantity = 7;

	WorldTransform worldTransformEnemyCollision_[EnemyCollisionquantity];

	WorldTransform worldTransformAttackrange_;

	WorldTransform worldTransformAttackCollision_[10];

	uint32_t textureHandle_ = 0;

	uint32_t TesttextureHandle_ = 0;
	bool testhit = false;
	int testTime = 30;

	//回転角
	Vector3 radius_{ 0.0f,0.0f,30.0f };

	//距離
	float r[EnemyCollisionquantity];

	//ラジアン角度
	double radian_;


	//行動
	int movementFase_ = 0; //0:待機中 1:行動決定　2:行動中 3:待機移行


	//待機基礎時間
	float waitbaseTime_ = 1;
	float waitrandomTime_ = 0;

	float waitTime_ = 0;

	int attackPattern_ = 0; //1:ブロック 2:弱攻撃 3:強攻撃 4:スタン

	//移動
	Vector2 velocity_ = { 0.0f,0.0f }; //x:左右 y:上下

	Vector2 amountMovementX_ = { 0.0f,0.0f }; //x:左 y:右

	Vector2 amountMovementY_ = { 0.0f,0.0f }; //x:上 y:下

	Vector2 movementscaleX_ = { 1.0f,1.0f }; //x:左 y:右

	Vector2 movementscaleY_ = { 1.0f,1.0f }; //x:上 y:下


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

	float motionspeedX = 0.0f;
	float motionspeedY = 0.0f;

	float bufferpointX = 0.0f;
	float bufferpointY = 0.0f;

	//ブロックされた
	void GetBlockMotion();
	bool getblock_ = false;

	Vector2 getblockbufferpoint_;

	int getblockFrame_ = 60;


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
	bool prevstunAttack_ = false;

	int stunStartmotionFrame_ = 20;
	int stunAttackingFrame_ = 60;
	int stunEndmotionFrame_ = 12;

	void GetStunMotion();
	bool getstun_ = false;
	bool checkUpDown = false; //false:下　true:上
	int stunSecond_ = 1;
	int stunTime_ = stunSecond_ * 60;


	//体力
	int hp = 0;
	//弱攻撃威力
	int weakAttackPower_;
	//強攻撃威力
	int heavyAttackPower_;
	//
	int invincibleTime_ = 0;
};








