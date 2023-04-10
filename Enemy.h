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


class Enemy
{
public:
	~Enemy();

	void Initialize(Model* model, Model* modelFace, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Motion();

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

private:
	Model* model_ = nullptr;

	Model* modelFace_ = nullptr;

	Model* modelEnemyCollision_ = nullptr;

	Model* modelAttackRange_ = nullptr;

	Model* modelAttackCollision_ = nullptr;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;
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


	int waitbaseTime_ = 120;
	int waitrandomTime_ = 0;

	int waitTime_ = 0;

	int attackPattern_ = 0; //1:�u���b�N 2:��U�� 3:���U�� 4:�X�^��


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

	int stunStartmotionFrame_ = 20;
	int stunAttackingFrame_ = 60;
	int stunEndmotionFrame_ = 12;


};

//���S���WB����x���W��X cm�Ay���W��Y cm����Ă���_A��B�𒆐S�ɐ���90�����������A1�����̓_A�̈ړ��ʂ����߂鎮�͈ȉ��̒ʂ�ł��B

//�܂��A�_A�̏����ʒu���璆�S���WB�܂ł̋���r�����߂܂��B r = ��(X ^ 2 + Y ^ 2)

//���ɁA�_A��1�x���������̈ړ��ʃ�x�ƃ�y�����߂܂��B ��x = r * sin(1��) ��y = r * cos(1��)

//�ȏ�̎��ŋ��߂��܂��B1






