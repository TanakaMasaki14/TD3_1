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

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
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

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	ViewProjection viewProjection_;

	Model* modelPlayerArm_ = nullptr;
	uint32_t textureHandlePlayerArm_ = 0;

	Model* modelPlayerFace_ = nullptr;

	Model* modelEnemy_ = nullptr;

	Model* modelEnemyFace_ = nullptr;

	bool stop = false;
	int stopSecond_ = 1;
	int stopTimer_ = stopSecond_ * 60;

	Vector3 SpeedBuffer = { 0,0,0 };

	int PlayerAttackToEnemy = 0; //0:ヒット待機 1:ヒット 2:ヒット待機移行待ち

	int EnemyAttackToPlayer = 0; //0:ヒット待機 1:ヒット 2:ヒット待機移行待ち
};
