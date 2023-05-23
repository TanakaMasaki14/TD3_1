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

	//�����蔻��

	void GetBlock();

	void GetWeak();

	void GetHeavy();

	void GetStun();

	///�Q�b�^�[
	//�J�X
	Vector3 GetWorldTransformEnemyCollision0();
	Vector3 GetWorldTransformEnemyCollision1();
	Vector3 GetWorldTransformEnemyCollision2();
	Vector3 GetWorldTransformEnemyCollision3();
	Vector3 GetWorldTransformEnemyCollision4();
	Vector3 GetWorldTransformEnemyCollision5();
	Vector3 GetWorldTransformEnemyCollision6();

	//�J�X
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

	//�U���̓Q�b�^�[
	int GetWeakPower() { return weakAttackPower_; }

	int GetHeavyPower() { return heavyAttackPower_; }

	/// <summary>
	/// �x�[�X�^�C�}�[�Đݒ�(�����͕b��(�����_��))
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

	//��]�p
	Vector3 radius_{ 0.0f,0.0f,30.0f };

	//����
	float r[EnemyCollisionquantity];

	//���W�A���p�x
	double radian_;


	//�s��
	int movementFase_ = 0; //0:�ҋ@�� 1:�s������@2:�s���� 3:�ҋ@�ڍs


	//�ҋ@��b����
	float waitbaseTime_ = 1;
	float waitrandomTime_ = 0;

	float waitTime_ = 0;

	int attackPattern_ = 0; //1:�u���b�N 2:��U�� 3:���U�� 4:�X�^��

	//�ړ�
	Vector2 velocity_ = { 0.0f,0.0f }; //x:���E y:�㉺

	Vector2 amountMovementX_ = { 0.0f,0.0f }; //x:�� y:�E

	Vector2 amountMovementY_ = { 0.0f,0.0f }; //x:�� y:��

	Vector2 movementscaleX_ = { 1.0f,1.0f }; //x:�� y:�E

	Vector2 movementscaleY_ = { 1.0f,1.0f }; //x:�� y:��


	//�U���͈�
	bool attackrange_ = false;

	float attackbufferX_ = 0.0f;
	float attackbufferY_ = 0.0f;

	//�u���b�N
	void Block();
	bool block_ = false;

	int blockStartmotionFrame_ = 6;
	int blockChanceFrame_ = 60;
	int blockEndmotionFlame_ = 6;

	float motionspeedX = 0.0f;
	float motionspeedY = 0.0f;

	float bufferpointX = 0.0f;
	float bufferpointY = 0.0f;

	//�u���b�N���ꂽ
	void GetBlockMotion();
	bool getblock_ = false;

	Vector2 getblockbufferpoint_;

	int getblockFrame_ = 60;


	//��U��
	void WeakAttack();
	bool weakAttack_ = false;

	int weakStartmotionFrame_ = 3;
	int weakAttackingFrame_ = 10;
	int weakEndmotionFrame_ = 6;

	//���U��
	void HeavyAttack();
	bool heavyAttack_ = false;

	int heavyStartmotionFrame_ = 12;
	int heavyAttackingFrame_ = 10;
	int heavyEndmotionFrame_ = 6;

	//�X�^��
	void StunAttack();
	bool stunAttack_ = false;
	bool prevstunAttack_ = false;

	int stunStartmotionFrame_ = 20;
	int stunAttackingFrame_ = 60;
	int stunEndmotionFrame_ = 12;

	void GetStunMotion();
	bool getstun_ = false;
	bool checkUpDown = false; //false:���@true:��
	int stunSecond_ = 1;
	int stunTime_ = stunSecond_ * 60;


	//�̗�
	int hp = 0;
	//��U���З�
	int weakAttackPower_;
	//���U���З�
	int heavyAttackPower_;
	//
	int invincibleTime_ = 0;
};








