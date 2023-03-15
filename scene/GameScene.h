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
	uint32_t textureHandle_ = 0;
	Sprite* sprite_ = new Sprite;
	Sprite* stage1Sprite_ = new Sprite;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	PlayerArm* playerArm_ = nullptr;
	Model* modelPlayerArm_ = nullptr;
	uint32_t textureHandlePlayerArm_ = 0;
	//Model* skyDome_ = nullptr;

	int scene = 0;
	int shakeFlag = 0;
	int shakeTimer = 30;
	bool stop = false;
	int stopSecond_ = 1;
	int stopTimer_ = stopSecond_ * 60;
	Vector3 SpeedBuffer = { 0,0,0 };
};