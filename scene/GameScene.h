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

class GameScene {

  public: // メンバ関数
	GameScene();
	~GameScene();
	//初期化
	void Initialize();
	//終了
	void Finalize();
	//毎フレーム処理
	void Update();
	//描画
	void Draw();
	void Stop();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	uint32_t soundHandleTitle_ = 0;
	uint32_t soundHandleLoop_ = 0;
	uint32_t textureHandleStage1_ = 0;
	uint32_t textureHandleStage2_ = 0;
	uint32_t textureHandleStage3_ = 0;
	Sprite* stage1Sprite_ = new Sprite;
	Sprite* stage2Sprite_ = new Sprite;
	Sprite* stage3Sprite_ = new Sprite;
	uint32_t textureHandlePlayerArm_ = 0;
	PlayerArm* playerArm_ = nullptr;
	Model* modelPlayerArm_ = nullptr;
	Model* modelPlayerFace_ = nullptr;
	//WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//Model* skyDome_ = nullptr;

	int scene = 0;
	int shakeFlag = 0;
	int shakeTimer = 30;
	bool soundloop = false;
	bool stop = false;
	int stopSecond_ = 1;
	int stopTimer_ = stopSecond_ * 60;
	Vector3 SpeedBuffer = { 0,0,0 };
};