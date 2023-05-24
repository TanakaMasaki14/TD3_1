#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "PlayerArm.h"
#include "Enemy.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	~GameScene();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void CheckCollision();
	void CollisionPlayerAttackToEnemy();
	void CollisionEnemyAttackToPlayer();
	void Stop();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	PlayerArm* playerArm_ = nullptr;
	Enemy* enemy_ = nullptr;

	ViewProjection viewProjection_;
	Model* modelPlayerArm_ = nullptr;
	uint32_t textureHandlePlayerArm_ = 0;
	Model* modelPlayerFace_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelEnemyFace_ = nullptr;

	uint32_t textureHandleTitle_ = 0;
	uint32_t textureHandleSetumei_ = 0;
	uint32_t textureHandleStage1_ = 0;
	uint32_t textureHandleStage2_ = 0;
	uint32_t textureHandleStage3_ = 0;
	//uint32_t textureHandleNya_ = 0;
	//uint32_t textureHandleBatin_ = 0;
	//uint32_t textureHandleDon_ = 0;
	uint32_t textureHandleKo_ = 0;
	uint32_t textureHandleClear_ = 0;

	uint32_t soundHandleTitle_ = 0;
	uint32_t soundHandleNext_ = 0;
	uint32_t soundHandleLoop_ = 0;
	uint32_t soundHandleKo_ = 0;
	uint32_t soundHandleClear_ = 0;

	Sprite* titleSprite_ = new Sprite;
	Sprite* setumeiSprite_ = new Sprite;
	Sprite* stage1Sprite_ = new Sprite;
	Sprite* stage2Sprite_ = new Sprite;
	Sprite* stage3Sprite_ = new Sprite;
	//Sprite* nyaSprite_ = new Sprite;
	//Sprite* batinSprite_ = new Sprite;
	//Sprite* donSprite_ = new Sprite;
	Sprite* koSprite_ = new Sprite;
	Sprite* clearSprite_ = new Sprite;

	int scene = 0;
	int shakeFlag_ = 0;
	int shakeTimer_ = 30;
	int fontFlag_ = 0;
	int fontTimer_ = 0;
	int playerKoFlag_ = 0;
	int enemyKoFlag_ = 0;
	bool soundloop = false;
	bool stop = false;
	int stopSecond_ = 1;
	int stopTimer_ = stopSecond_ * 60;
	Vector3 SpeedBuffer = { 0,0,0 };
	int PlayerAttackToEnemy = 0; //0:ヒット待機 1:ヒット 2:ヒット待機移行待ち
	int EnemyAttackToPlayer = 0; //0:ヒット待機 1:ヒット 2:ヒット待機移行待ち
	int invincibleSecond_ = 1;

	XINPUT_STATE joyState{};

	bool GamePAD_B = false;
	bool prevGamePAD_B = false;
	bool GamePAD_RIGHT_SHOULDER = false;
	bool prevGamePAD_RIGHT_SHOULDER = false;
	void PADUpdate();
};