#include "Enemy.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

#include "Procession.h"
#include "TextureManager.h"
#include "PlayerArm.h"
#include <corecrt_math_defines.h>
using namespace MathUtility;
using namespace DirectX;
using namespace std;


Enemy::~Enemy()
{
	SafeDelete(modelAttackRange_);
	SafeDelete(modelEnemyCollision_);
	SafeDelete(modelAttackCollision_);
}

void Enemy::Initialize(Model* model, Model* modelFace, uint32_t textureHandle)
{
	assert(model);
	assert(modelFace);

	model_ = model;
	modelFace_ = modelFace;
	textureHandle_ = textureHandle;

	TesttextureHandle_ = TextureManager::Load("blocktest.png");

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	modelAttackRange_ = Model::Create();

	modelEnemyCollision_ = Model::CreateFromOBJ("C");

	modelAttackCollision_ = Model::CreateFromOBJ("C");


	//
	srand((unsigned)time(NULL));

	waitrandomTime_ = rand() % 61;

	waitTime_ = waitbaseTime_ + waitrandomTime_;

	//ファイル読み込み
	ifstream enemyfile("Text/Enemy.txt");

	if (enemyfile.is_open()) {
		string stringhp, stringweakPower, stringheavyPower;

		getline(enemyfile, stringhp);
		getline(enemyfile, stringweakPower);
		getline(enemyfile, stringheavyPower);

		int pos1 = stringhp.find(":");
		int pos2 = stringweakPower.find(":");
		int pos3 = stringheavyPower.find(":");

		if (pos1 != string::npos) {
			stringhp = stringhp.substr(pos1 + 1);
		}
		if (pos2 != string::npos) {
			stringweakPower = stringweakPower.substr(pos2 + 1);
		}
		if (pos3 != string::npos) {
			stringheavyPower = stringheavyPower.substr(pos3 + 1);
		}

		hp = stoi(stringhp);
		weakAttackPower_ = stoi(stringweakPower);
		heavyAttackPower_ = stoi(stringheavyPower);

		enemyfile.close();
	}




	///腕
	{
		worldTransform_.scale_ = { 20.0f,2.5f,2.5f };
		worldTransform_.rotation_ = { 0.0f,0.0f, XMConvertToRadians(30.0f) };
		worldTransform_.translation_ = { -25.0f,-5.0f,0.0f };

		worldTransform_.Initialize();

		worldTransform_.matWorld_ = Mat_Identity();
		worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

		worldTransform_.TransferMatrix();
	}

	///顔
	{
		worldTransformFace_.scale_ = { 4.0f,4.0f,4.0f };
		worldTransformFace_.rotation_ = { 0.0f,0.0f,0.0f };
		worldTransformFace_.translation_ = { -44.0f,27.0f,0.0f };

		worldTransformFace_.Initialize();

		worldTransformFace_.matWorld_ = Mat_Identity();
		worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);

		worldTransformFace_.TransferMatrix();
	}

	///腕当たり判定エリア
	{
		radian_ = radius_.z * M_PI / 180.0;
		//0
		worldTransformEnemyCollision_[0].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[0].rotation_ = { 0.0f,0.0f,0.0f };
		worldTransformEnemyCollision_[0].translation_ = { worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z };

		//1
		worldTransformEnemyCollision_[1].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[1].rotation_ = { 0.0f,0.0f,0.0f };
		r[0] = 5.0f;
		worldTransformEnemyCollision_[1].translation_.x = worldTransform_.translation_.x + r[0] * cos(static_cast<float>(radian_));
		worldTransformEnemyCollision_[1].translation_.y = worldTransform_.translation_.y + r[0] * sin(static_cast<float>(radian_));

		//2
		worldTransformEnemyCollision_[2].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[2].rotation_ = { 0.0f,0.0f,0.0f };
		r[1] = -5.0f;
		worldTransformEnemyCollision_[2].translation_.x = worldTransform_.translation_.x + r[1] * cos(static_cast<float>(radian_));
		worldTransformEnemyCollision_[2].translation_.y = worldTransform_.translation_.y + r[1] * sin(static_cast<float>(radian_));

		//3
		worldTransformEnemyCollision_[3].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[3].rotation_ = { 0.0f,0.0f,0.0f };
		r[2] = 10.0f;
		worldTransformEnemyCollision_[3].translation_.x = worldTransform_.translation_.x + r[2] * cos(static_cast<float>(radian_));
		worldTransformEnemyCollision_[3].translation_.y = worldTransform_.translation_.y + r[2] * sin(static_cast<float>(radian_));

		//4
		worldTransformEnemyCollision_[4].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[4].rotation_ = { 0.0f,0.0f,0.0f };
		r[3] = -10.0f;
		worldTransformEnemyCollision_[4].translation_.x = worldTransform_.translation_.x + r[3] * cos(static_cast<float>(radian_));
		worldTransformEnemyCollision_[4].translation_.y = worldTransform_.translation_.y + r[3] * sin(static_cast<float>(radian_));

		//5
		worldTransformEnemyCollision_[5].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[5].rotation_ = { 0.0f,0.0f,0.0f };
		r[4] = 15.0f;
		worldTransformEnemyCollision_[5].translation_.x = worldTransform_.translation_.x + r[4] * cos(static_cast<float>(radian_));
		worldTransformEnemyCollision_[5].translation_.y = worldTransform_.translation_.y + r[4] * sin(static_cast<float>(radian_));

		//6
		worldTransformEnemyCollision_[6].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformEnemyCollision_[6].rotation_ = { 0.0f,0.0f,0.0f };
		r[5] = -15.0f;
		worldTransformEnemyCollision_[6].translation_.x = worldTransform_.translation_.x + r[5] * cos(static_cast<float>(radian_));
		worldTransformEnemyCollision_[6].translation_.y = worldTransform_.translation_.y + r[5] * sin(static_cast<float>(radian_));



		for (int i = 0; i < EnemyCollisionquantity; i++) {
			worldTransformEnemyCollision_[i].Initialize();

			worldTransformEnemyCollision_[i].matWorld_ = Mat_Identity();
			worldTransformEnemyCollision_[i].matWorld_ = MatWorld(worldTransformEnemyCollision_[i].scale_, worldTransformEnemyCollision_[i].rotation_, worldTransformEnemyCollision_[i].translation_);

			worldTransformEnemyCollision_[i].TransferMatrix();
		}
	}

	///攻撃当たり判定エリア
	{
		worldTransformAttackrange_.scale_ = { 0.0f,0.0f,0.0f };
		worldTransformAttackrange_.rotation_ = { 0.0f,0.0f,0.0f };
		worldTransformAttackrange_.translation_ = { 0.0f,0.0f,0.0f };

		worldTransformAttackrange_.Initialize();

		worldTransformAttackrange_.matWorld_ = Mat_Identity();
		worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);

		worldTransformAttackrange_.TransferMatrix();

		//
		for (int i = 0; i < 10; i++) {
			worldTransformAttackCollision_[i].scale_ = { 0.0f,0.0f,0.0f };
			worldTransformAttackCollision_[i].rotation_ = { 0.0f,0.0f,0.0f };
			worldTransformAttackCollision_[i].translation_ = { 1000.0f,0.0f,0.0f };

			worldTransformAttackCollision_[i].Initialize();

			worldTransformAttackCollision_[i].matWorld_ = Mat_Identity();
			worldTransformAttackCollision_[i].matWorld_ = MatWorld(worldTransformAttackCollision_[i].scale_, worldTransformAttackCollision_[i].rotation_, worldTransformAttackCollision_[i].translation_);

			worldTransformAttackCollision_[i].TransferMatrix();
		}
	}
}

void Enemy::Update()
{
	///手動動作用
	{
		if (input_->TriggerKey(DIK_U)) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				block_ = true;
			}
		}

		if (input_->TriggerKey(DIK_I)) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				motionspeedX = 2.0f;
				motionspeedY = 4.0f;


				attackbufferX_ = worldTransform_.translation_.x;
				attackbufferY_ = worldTransform_.translation_.y;

				worldTransformAttackrange_.scale_ = { 2.0f,10.0f,2.0f };
				for (int i = 0; i < 10; i++) {
					worldTransformAttackCollision_[i].scale_ = { 2.0f,2.0f,2.0f };
				}

				weakAttack_ = true;
			}
		}

		if (input_->TriggerKey(DIK_O)) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				motionspeedX = 0.5f;
				motionspeedY = 1.0f;

				attackbufferX_ = worldTransform_.translation_.x;
				attackbufferY_ = worldTransform_.translation_.y;

				worldTransformAttackrange_.scale_ = { 3.0f,10.0f,3.0f };

				for (int i = 0; i < 10; i++) {
					worldTransformAttackCollision_[i].scale_ = { 3.0f,3.0f,3.0f };
				}

				heavyAttack_ = true;

			}
		}

		if (input_->TriggerKey(DIK_P)) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				attackbufferX_ = -43.0f;
				attackbufferY_ = -5.0f;

				worldTransformAttackrange_.translation_.x = attackbufferX_;
				worldTransformAttackrange_.translation_.y = attackbufferY_;

				worldTransformAttackrange_.scale_ = { 2.0f,20.0f,2.0f };

				for (int i = 0; i < 10; i++) {
					worldTransformAttackCollision_[i].scale_ = { 2.0f,2.0f,2.0f };
				}

				stunAttack_ = true;
			}
		}
	}



	if (movementFase_ == 0) {
		waitTime_ -= 1;
		if (waitTime_ == 0) {
			attackPattern_ = rand() % 4 + 1;
		}
		if (waitTime_ < 0) {
			movementFase_ = 1;
		}
	}

	//行動決定
	if (movementFase_ == 1) {

		///ブロック
		if (attackPattern_ == 1) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				block_ = true;
			}

			movementFase_ = 2;
		}

		///弱攻撃
		if (attackPattern_ == 2) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				motionspeedX = 2.0f;
				motionspeedY = 4.0f;


				attackbufferX_ = worldTransform_.translation_.x;
				attackbufferY_ = worldTransform_.translation_.y;

				worldTransformAttackrange_.scale_ = { 2.0f,10.0f,2.0f };

				for (int i = 0; i < 10; i++) {
					worldTransformAttackCollision_[i].scale_ = { 2.0f,2.0f,2.0f };
				}

				weakAttack_ = true;
			}

			movementFase_ = 2;
		}

		///強攻撃
		if (attackPattern_ == 3) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				motionspeedX = 0.5f;
				motionspeedY = 1.0f;

				attackbufferX_ = worldTransform_.translation_.x;
				attackbufferY_ = worldTransform_.translation_.y;

				worldTransformAttackrange_.scale_ = { 3.0f,10.0f,3.0f };
				for (int i = 0; i < 10; i++) {
					worldTransformAttackCollision_[i].scale_ = { 3.0f,3.0f,3.0f };
				}

				heavyAttack_ = true;

			}

			movementFase_ = 2;
		}

		///スタン
		if (attackPattern_ == 4) {
			if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
				attackbufferX_ = -43.0f;
				attackbufferY_ = -5.0f;

				worldTransformAttackrange_.translation_.x = attackbufferX_;
				worldTransformAttackrange_.translation_.y = attackbufferY_;

				worldTransformAttackrange_.scale_ = { 2.0f,20.0f,2.0f };

				for (int i = 0; i < 10; i++) {
					worldTransformAttackCollision_[i].scale_ = { 2.0f,2.0f,2.0f };
				}

				stunAttack_ = true;
			}

			movementFase_ = 2;
		}

	}

	//行動中
	if (movementFase_ == 2) {

	}

	//行動終了後、初期化
	if (movementFase_ == 3) {
		waitrandomTime_ = rand() % 61;

		waitTime_ = waitbaseTime_ + waitrandomTime_;

		movementFase_ = 0;
	}

	debugText_->SetPos(0, 120);
	debugText_->Printf("rotationZ:%f", worldTransform_.rotation_.z);

	debugText_->SetPos(0, 140);
	debugText_->Printf("hp:%d", hp);



	///モーションまとめ
	Motion();

	//当たり判定テスト
	if (testhit == true) {
		testTime -= 1;
		if (testTime < 0) {
			testhit = false;
			testTime = 30;
		}
	}


	///当たり判定エリア更新
	//0
	worldTransformEnemyCollision_[0].translation_ = worldTransform_.translation_;
	//1以降
	for (int i = 1; i < EnemyCollisionquantity; i++) {
		worldTransformEnemyCollision_[i].translation_.x = worldTransform_.translation_.x + r[i - 1] * cos(worldTransform_.rotation_.z);
		worldTransformEnemyCollision_[i].translation_.y = worldTransform_.translation_.y + r[i - 1] * sin(worldTransform_.rotation_.z);
	}

	///座標更新
	//
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	//
	worldTransformFace_.matWorld_ = Mat_Identity();
	worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);
	worldTransformFace_.TransferMatrix();

	//
	for (int i = 0; i < EnemyCollisionquantity; i++) {
		worldTransformEnemyCollision_[i].matWorld_ = Mat_Identity();
		worldTransformEnemyCollision_[i].matWorld_ = MatWorld(worldTransformEnemyCollision_[i].scale_, worldTransformEnemyCollision_[i].rotation_, worldTransformEnemyCollision_[i].translation_);
		worldTransformEnemyCollision_[i].TransferMatrix();
	}

	//
	worldTransformAttackrange_.matWorld_ = Mat_Identity();
	worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);
	worldTransformAttackrange_.TransferMatrix();

	for (int i = 0; i < 10; i++) {
		worldTransformAttackCollision_[i].matWorld_ = Mat_Identity();
		worldTransformAttackCollision_[i].matWorld_ = MatWorld(worldTransformAttackCollision_[i].scale_, worldTransformAttackCollision_[i].rotation_, worldTransformAttackCollision_[i].translation_);

		worldTransformAttackCollision_[i].TransferMatrix();
	}
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	if (testhit == false) {
		if (input_->PushKey(DIK_Q) == 0) {
			model_->Draw(worldTransform_, viewProjection, textureHandle_);
		}
	}

	//テスト用
	if (testhit == true) {
		model_->Draw(worldTransform_, viewProjection, TesttextureHandle_);
	}

	modelFace_->Draw(worldTransformFace_, viewProjection, textureHandle_);

	for (int i = 0; i < EnemyCollisionquantity; i++) {
		modelEnemyCollision_->Draw(worldTransformEnemyCollision_[i], viewProjection);
	}

	if (attackrange_ == true) {
		if (input_->PushKey(DIK_Q) == 0) {
			modelAttackRange_->Draw(worldTransformAttackrange_, viewProjection, textureHandle_);
		}

		for (int i = 0; i < 10; i++) {
			modelAttackCollision_->Draw(worldTransformAttackCollision_[i], viewProjection);
		}
	}
}

void Enemy::Motion()
{
	Block();
	GetBlockMotion();
	WeakAttack();
	HeavyAttack();
	StunAttack();
	GetStunMotion();
}





void Enemy::GetBlock()
{
	if (weakAttack_ == true) {
		weakStartmotionFrame_ = 3;
		weakAttackingFrame_ = 10;
		weakEndmotionFrame_ = 6;

		motionspeedX = 0.0f;
		motionspeedY = 0.0f;

		bufferpointX = 0.0f;
		bufferpointY = 0.0f;
		attackbufferX_ = 0.0f;
		attackbufferY_ = 0.0f;

		weakAttack_ = false;
	}
	if (heavyAttack_ == true) {
		heavyStartmotionFrame_ = 12;
		heavyAttackingFrame_ = 10;
		heavyEndmotionFrame_ = 6;

		motionspeedX = 0.0f;
		motionspeedY = 0.0f;

		bufferpointX = 0.0f;
		bufferpointY = 0.0f;
		attackbufferX_ = 0.0f;
		attackbufferY_ = 0.0f;

		heavyAttack_ = false;
	}
	if (stunAttack_ == true) {
		prevstunAttack_ = stunAttack_;

		stunStartmotionFrame_ = 20;
		stunAttackingFrame_ = 60;
		stunEndmotionFrame_ = 12;

		worldTransformFace_.translation_ = { -44.0f,27.0f,0.0f };

		stunAttack_ = false;
	}

	for (int i = 0; i < 10; i++) {
		worldTransformAttackCollision_[i].translation_ = { 1000.f,0.0f,0.0f };
		worldTransformAttackCollision_[i].scale_ = { 0.0f,0.0f,0.0f };
	}

	attackrange_ = false;

	getblockbufferpoint_.x = worldTransform_.translation_.x;
	getblockbufferpoint_.y = worldTransform_.translation_.y;

	worldTransform_.rotation_ = { 0,0,0.5f };

	getblock_ = true;
}

void Enemy::GetWeak()
{
	testhit = true;
	hp = hp - playerarm_->GetWeakPower();
}

void Enemy::GetHeavy()
{
	testhit = true;
	hp = hp - playerarm_->GetHeavyPower();
}

void Enemy::GetStun()
{
	//testhit = true;
	if (weakAttack_ == true) {
		weakStartmotionFrame_ = 3;
		weakAttackingFrame_ = 10;
		weakEndmotionFrame_ = 6;

		motionspeedX = 0.0f;
		motionspeedY = 0.0f;

		bufferpointX = 0.0f;
		bufferpointY = 0.0f;
		attackbufferX_ = 0.0f;
		attackbufferY_ = 0.0f;

		weakAttack_ = false;
	}

	if (heavyAttack_ == true) {
		heavyStartmotionFrame_ = 12;
		heavyAttackingFrame_ = 10;
		heavyEndmotionFrame_ = 6;

		motionspeedX = 0.0f;
		motionspeedY = 0.0f;

		bufferpointX = 0.0f;
		bufferpointY = 0.0f;
		attackbufferX_ = 0.0f;
		attackbufferY_ = 0.0f;

		heavyAttack_ = false;
	}

	if (stunAttack_ == true) {

		stunStartmotionFrame_ = 20;
		stunAttackingFrame_ = 60;
		stunEndmotionFrame_ = 12;

		worldTransformFace_.translation_ = { 44.0f,27.0f,0.0f };

		stunAttack_ = false;
	}

	for (int i = 0; i < 10; i++) {
		worldTransformAttackCollision_[i].translation_ = { 1000.f,0.0f,0.0f };
		worldTransformAttackCollision_[i].scale_ = { 0.0f,0.0f,0.0f };
	}
	attackrange_ = false;

	//下
	if (worldTransform_.rotation_.z < 0.5f) {
		checkUpDown = false;
	}
	//上
	if (worldTransform_.rotation_.z > 0.5f) {
		checkUpDown = true;
	}


	if (movementFase_ == 0) {
		movementFase_ = 2;
	}

	getstun_ = true;
}

/// <summary>
/// ブロック
/// </summary>
void Enemy::Block()
{
	if (block_) {
		blockStartmotionFrame_ -= 1;

		///ブロック前モーション
		if (blockStartmotionFrame_ > 0) {
			//移動
			worldTransform_.translation_.x += 3.0f;
			worldTransform_.translation_.y += 1.0f;
			//回転
			worldTransform_.rotation_.z += 0.3f;
			if (worldTransform_.rotation_.z > 1.55f) {
				worldTransform_.rotation_.z = 1.55f;
			}
		}

		if (blockStartmotionFrame_ < 0) {
			blockChanceFrame_ -= 1;
		}


		//ブロック中モーション
		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ > 0) {

		}

		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ < 0) {
			blockEndmotionFlame_ -= 1;
		}


		///ブロック後モーション
		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ < 0 && blockEndmotionFlame_ > 0) {
			//移動
			worldTransform_.translation_.x -= 3.0f;
			worldTransform_.translation_.y -= 1.0f;
			//回転
			worldTransform_.rotation_.z -= 0.3f;
			if (worldTransform_.rotation_.z < 0.5f) {
				worldTransform_.rotation_.z = 0.5f;
			}
		}

		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ < 0 && blockEndmotionFlame_ < 0) {
			blockStartmotionFrame_ = 6;
			blockChanceFrame_ = 60;
			blockEndmotionFlame_ = 6;
			block_ = false;

			if (movementFase_ == 2) {
				movementFase_ = 3;
			}
		}
	}
}

/// <summary>
/// ブロックされた
/// </summary>
void Enemy::GetBlockMotion()
{
	if (getblock_ == true) {
		getblockFrame_ -= 1;
		if (getblockFrame_ > 10) {
			//移動
			if (prevstunAttack_ == false) {
				if (abs(worldTransform_.translation_.x - getblockbufferpoint_.x) < 20) {
					worldTransform_.translation_.x -= 4.0f;
				}
			}

			//回転
			if (worldTransform_.rotation_.z < 2.0f) {
				worldTransform_.rotation_.z += 0.5f;
			}
		}

		if (getblockFrame_ <= 10 && getblockFrame_ > 0) {
			if (worldTransform_.rotation_.z > 0.5) {
				worldTransform_.rotation_.z -= 0.5f;
			}
		}

		if (getblockFrame_ == 0) {
			if (prevstunAttack_ == true) {
				prevstunAttack_ = false;
			}

			getblockbufferpoint_ = { 0,0 };
			getblockFrame_ = 60;
			getblock_ = false;

			if (movementFase_ == 2) {
				movementFase_ = 3;
			}
		}
	}
}

/// <summary>
/// 弱攻撃
/// </summary>
void Enemy::WeakAttack()
{
	if (weakAttack_) {
		weakStartmotionFrame_ -= 1;

		///弱攻撃前モーション
		if (weakStartmotionFrame_ > 0) {
			//移動
			worldTransform_.translation_.y += motionspeedY; //初期値4.0f
			worldTransform_.translation_.x += motionspeedX; //初期値2.0f
			motionspeedX += 1.0f;
			//回転
			worldTransform_.rotation_.z += 0.3f;
			if (worldTransform_.rotation_.z > 1.4f) {
				worldTransform_.rotation_.z = 1.4f;
			}
		}

		if (weakStartmotionFrame_ == 0) {
			motionspeedX = 0.0f;
			motionspeedY = -10.0f;
			bufferpointY = worldTransform_.translation_.y;
		}

		if (weakStartmotionFrame_ < 0) {
			weakAttackingFrame_ -= 1;
		}


		///弱攻撃中モーション
		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ > 0) {
			///移動
			//縦
			worldTransform_.translation_.y += motionspeedY;
			//横
			if (weakAttackingFrame_ >= 9) {
				worldTransform_.translation_.x += 9.8f;
			}
			if (weakAttackingFrame_ == 8) {

			}
			if (weakAttackingFrame_ <= 7 && weakAttackingFrame_ > 0) {
				worldTransform_.translation_.x -= 1.4f;
			}

			//移動制限(-13)
			if ((worldTransform_.translation_.y - bufferpointY) < -13) {
				motionspeedY = 0;
				worldTransform_.translation_.y = bufferpointY - 13.0f;
			}

			//回転
			worldTransform_.rotation_.z -= 0.4f;
			if (worldTransform_.rotation_.z < -0.6f) {
				worldTransform_.rotation_.z = -0.6f;
			}

			//攻撃判定位置
			attackrange_ = true;

			worldTransformAttackrange_.translation_.x = attackbufferX_ + 30.0f;
			worldTransformAttackrange_.translation_.y = attackbufferY_;

			worldTransformAttackCollision_[0].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[0].translation_.y = worldTransformAttackrange_.translation_.y;

			worldTransformAttackCollision_[1].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[1].translation_.y = worldTransformAttackrange_.translation_.y + 4.0f;

			worldTransformAttackCollision_[2].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[2].translation_.y = worldTransformAttackrange_.translation_.y - 4.0f;

			worldTransformAttackCollision_[3].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[3].translation_.y = worldTransformAttackrange_.translation_.y + 8.0f;

			worldTransformAttackCollision_[4].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[4].translation_.y = worldTransformAttackrange_.translation_.y - 8.0f;


		}


		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ == 0) {
			bufferpointX = worldTransform_.translation_.x;
			bufferpointY = worldTransform_.translation_.y;

			for (int i = 0; i < 10; i++) {
				worldTransformAttackCollision_[i].translation_ = { 1000.f,0.0f,0.0f };
				worldTransformAttackCollision_[i].scale_ = { 0.0f,0.0f,0.0f };
			}

			//攻撃判定消滅
			attackrange_ = false;
		}


		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ < 0) {
			weakEndmotionFrame_ -= 1;
		}



		///弱攻撃後モーション
		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ < 0 && weakEndmotionFrame_ > 0) {
			//移動
			worldTransform_.translation_.x -= 2.0f;
			worldTransform_.translation_.y += 2.0f;
			if ((worldTransform_.translation_.x - bufferpointX) < 5) {
				worldTransform_.translation_.x = bufferpointX - 5;
			}
			if ((worldTransform_.translation_.y - bufferpointY) > 5) {
				worldTransform_.translation_.y = bufferpointY + 5;
			}

			//回転
			worldTransform_.rotation_.z += 0.5f;
			if (worldTransform_.rotation_.z > 0.5f) {
				worldTransform_.rotation_.z = 0.5f;
			}
		}

		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ < 0 && weakEndmotionFrame_ < 0) {
			weakStartmotionFrame_ = 3;
			weakAttackingFrame_ = 10;
			weakEndmotionFrame_ = 6;

			motionspeedX = 0.0f;
			motionspeedY = 0.0f;

			bufferpointX = 0.0f;
			bufferpointY = 0.0f;
			attackbufferX_ = 0.0f;
			attackbufferY_ = 0.0f;

			weakAttack_ = false;
			if (movementFase_ == 2) {
				movementFase_ = 3;
			}
		}
	}
}

/// <summary>
/// 強攻撃
/// </summary>
void Enemy::HeavyAttack()
{
	if (heavyAttack_) {
		heavyStartmotionFrame_ -= 1;

		///強攻撃前モーション
		if (heavyStartmotionFrame_ > 0) {
			worldTransform_.translation_.y += motionspeedY;
			worldTransform_.translation_.x += motionspeedX;
			motionspeedX += 0.1f;

			//回転
			worldTransform_.rotation_.z += 0.07f;
			if (worldTransform_.rotation_.z > 1.4f) {
				worldTransform_.rotation_.z = 1.4f;
			}
		}

		if (heavyStartmotionFrame_ == 0) {
			motionspeedX = 0.0f;
			motionspeedY = -10.0f;
			bufferpointY = worldTransform_.translation_.y;
		}

		if (heavyStartmotionFrame_ < 0) {
			heavyAttackingFrame_ -= 1;
		}

		//強攻撃中モーション
		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ > 0) {
			///移動
			//縦
			worldTransform_.translation_.y += motionspeedY;
			//横
			if (heavyAttackingFrame_ >= 9) {
				worldTransform_.translation_.x += 9.8f;
			}
			if (heavyAttackingFrame_ == 8) {

			}
			if (heavyAttackingFrame_ <= 7 && heavyAttackingFrame_ > 0) {
				worldTransform_.translation_.x -= 1.4f;
			}

			//移動制限(-13)
			if ((worldTransform_.translation_.y - bufferpointY) < -13) {
				motionspeedY = 0;
				worldTransform_.translation_.y = bufferpointY - 13.0f;
			}

			//回転
			worldTransform_.rotation_.z -= 0.4f;
			if (worldTransform_.rotation_.z < -0.6f) {
				worldTransform_.rotation_.z = -0.6f;
			}

			//攻撃判定位置
			attackrange_ = true;

			worldTransformAttackrange_.translation_.x = attackbufferX_ + 35.0f;
			worldTransformAttackrange_.translation_.y = attackbufferY_;

			worldTransformAttackCollision_[0].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[0].translation_.y = worldTransformAttackrange_.translation_.y;

			worldTransformAttackCollision_[1].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[1].translation_.y = worldTransformAttackrange_.translation_.y + 4.0f;

			worldTransformAttackCollision_[2].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[2].translation_.y = worldTransformAttackrange_.translation_.y - 4.0f;

			worldTransformAttackCollision_[3].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[3].translation_.y = worldTransformAttackrange_.translation_.y + 8.0f;

			worldTransformAttackCollision_[4].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[4].translation_.y = worldTransformAttackrange_.translation_.y - 8.0f;
		}

		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ == 0) {
			bufferpointX = worldTransform_.translation_.x;
			bufferpointY = worldTransform_.translation_.y;

			for (int i = 0; i < 10; i++) {
				worldTransformAttackCollision_[i].translation_ = { 1000.f,0.0f,0.0f };
				worldTransformAttackCollision_[i].scale_ = { 0.0f,0.0f,0.0f };
			}

			//攻撃判定消滅
			attackrange_ = false;
		}


		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ < 0) {
			heavyEndmotionFrame_ -= 1;
		}


		///強攻撃後モーション
		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ < 0 && heavyEndmotionFrame_ > 0) {
			//移動
			worldTransform_.translation_.x -= 2.5f;
			worldTransform_.translation_.y += 2.0f;
			if ((worldTransform_.translation_.x - bufferpointX) < 11) {
				worldTransform_.translation_.x = bufferpointX - 11;
			}
			if ((worldTransform_.translation_.y - bufferpointY) > 2) {
				worldTransform_.translation_.y = bufferpointY + 2;
			}

			//回転
			worldTransform_.rotation_.z += 0.5f;
			if (worldTransform_.rotation_.z > 0.5f) {
				worldTransform_.rotation_.z = 0.5f;
			}
		}

		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ < 0 && heavyEndmotionFrame_ < 0) {
			heavyStartmotionFrame_ = 12;
			heavyAttackingFrame_ = 10;
			heavyEndmotionFrame_ = 6;

			motionspeedX = 0.0f;
			motionspeedY = 0.0f;

			bufferpointX = 0.0f;
			bufferpointY = 0.0f;
			attackbufferX_ = 0.0f;
			attackbufferY_ = 0.0f;

			heavyAttack_ = false;
			if (movementFase_ == 2) {
				movementFase_ = 3;
			}
		}

	}
}

/// <summary>
/// スタン攻撃
/// </summary>
void Enemy::StunAttack()
{
	if (stunAttack_) {
		stunStartmotionFrame_ -= 1;
		//攻撃前モーション
		if (stunStartmotionFrame_ > 0) {

		}

		if (stunStartmotionFrame_ < 0) {
			stunAttackingFrame_ -= 1;
		}


		//攻撃中
		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ > 0) {

			//37.0f, 20.0f, 0.0f x:7 y:7
			if (stunAttackingFrame_ > 55) {
				worldTransformFace_.translation_.x += 1.4f;
				worldTransformFace_.translation_.y -= 1.4f;
			}

			attackrange_ = true;
			worldTransformAttackrange_.translation_.x += 1.3f;

			worldTransformAttackCollision_[0].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[0].translation_.y = worldTransformAttackrange_.translation_.y;

			worldTransformAttackCollision_[1].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[1].translation_.y = worldTransformAttackrange_.translation_.y + 4.0f;

			worldTransformAttackCollision_[2].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[2].translation_.y = worldTransformAttackrange_.translation_.y - 4.0f;

			worldTransformAttackCollision_[3].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[3].translation_.y = worldTransformAttackrange_.translation_.y + 8.0f;

			worldTransformAttackCollision_[4].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[4].translation_.y = worldTransformAttackrange_.translation_.y - 8.0f;

			worldTransformAttackCollision_[5].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[5].translation_.y = worldTransformAttackrange_.translation_.y + 12.0f;

			worldTransformAttackCollision_[6].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[6].translation_.y = worldTransformAttackrange_.translation_.y - 12.0f;

			worldTransformAttackCollision_[7].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[7].translation_.y = worldTransformAttackrange_.translation_.y + 16.0f;

			worldTransformAttackCollision_[8].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[8].translation_.y = worldTransformAttackrange_.translation_.y - 16.0f;

		}

		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ < 0) {
			for (int i = 0; i < 10; i++) {
				worldTransformAttackCollision_[i].translation_ = { 1000.f,0.0f,0.0f };
				worldTransformAttackCollision_[i].scale_ = { 0.0f,0.0f,0.0f };
			}

			attackrange_ = false;
			stunEndmotionFrame_ -= 1;

		}


		//攻撃後モーション
		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ < 0 && stunEndmotionFrame_ > 0) {
			if (stunEndmotionFrame_ > 7) {
				worldTransformFace_.translation_.x -= 1.4f;
				worldTransformFace_.translation_.y += 1.4f;
			}
		}

		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ < 0 && stunEndmotionFrame_ < 0) {
			stunStartmotionFrame_ = 20;
			stunAttackingFrame_ = 60;
			stunEndmotionFrame_ = 12;

			stunAttack_ = false;
			if (movementFase_ == 2) {
				movementFase_ = 3;
			}

		}
	}
}

void Enemy::GetStunMotion()
{
	if (getstun_ == true) {
		stunTime_ -= 1;
		if (stunTime_ < 0) {
			if (checkUpDown == false) {
				worldTransform_.rotation_.z += 0.5f;
				if (worldTransform_.rotation_.z > 0.5f) {
					worldTransform_.rotation_.z = 0.5f;

					stunTime_ = stunSecond_ * 60;
					movementFase_ = 3;
					getstun_ = false;
				}
			}
			if (checkUpDown == true) {
				worldTransform_.rotation_.z -= 0.5f;
				if (worldTransform_.rotation_.z < 0.5f) {
					worldTransform_.rotation_.z = 0.5f;

					stunTime_ = stunSecond_ * 60;
					movementFase_ = 3;
					getstun_ = false;
				}
			}
		}
	}
}

/// <summary>
/// カス
/// </summary>
/// <returns></returns>
Vector3 Enemy::GetWorldTransformEnemyCollision0()
{
	Vector3 EnemyCollisionWorldPos;

	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[0].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[0].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[0].translation_.z;

	return EnemyCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyCollision1()
{
	Vector3 EnemyCollisionWorldPos;

	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[1].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[1].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[1].translation_.z;

	return EnemyCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyCollision2()
{
	Vector3 EnemyCollisionWorldPos;

	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[2].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[2].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[2].translation_.z;

	return EnemyCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyCollision3()
{
	Vector3 EnemyCollisionWorldPos;

	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[3].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[3].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[3].translation_.z;

	return EnemyCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyCollision4()
{
	Vector3 EnemyCollisionWorldPos;

	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[4].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[4].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[4].translation_.z;

	return EnemyCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyCollision5()
{
	Vector3 EnemyCollisionWorldPos;

	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[5].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[5].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[5].translation_.z;

	return EnemyCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyCollision6()
{
	Vector3 EnemyCollisionWorldPos;
	EnemyCollisionWorldPos.x = worldTransformEnemyCollision_[6].translation_.x;
	EnemyCollisionWorldPos.y = worldTransformEnemyCollision_[6].translation_.y;
	EnemyCollisionWorldPos.z = worldTransformEnemyCollision_[6].translation_.z;

	return EnemyCollisionWorldPos;
}

///
/// ////////////////////////////////////////////////////////////////////////////////////
///


/// <summary>
/// カス
/// </summary>
/// <returns></returns>
Vector3 Enemy::GetWorldTransformEnemyAttackCollision0()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[0].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[0].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[0].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision1()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[1].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[1].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[1].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision2()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[2].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[2].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[2].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision3()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[3].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[3].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[3].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision4()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[4].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[4].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[4].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision5()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[5].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[5].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[5].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision6()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[6].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[6].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[6].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision7()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[7].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[7].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[7].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision8()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[8].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[8].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[8].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetWorldTransformEnemyAttackCollision9()
{
	Vector3 EnemyAttackCollisionWorldPos;

	EnemyAttackCollisionWorldPos.x = worldTransformAttackCollision_[9].translation_.x;
	EnemyAttackCollisionWorldPos.y = worldTransformAttackCollision_[9].translation_.y;
	EnemyAttackCollisionWorldPos.z = worldTransformAttackCollision_[9].translation_.z;

	return EnemyAttackCollisionWorldPos;
}

Vector3 Enemy::GetScaleEnemyCollision()
{
	Vector3 EnemyCollisionScale;

	EnemyCollisionScale.x = worldTransformEnemyCollision_[0].scale_.x;
	EnemyCollisionScale.y = worldTransformEnemyCollision_[0].scale_.y;
	EnemyCollisionScale.z = worldTransformEnemyCollision_[0].scale_.z;

	return EnemyCollisionScale;
}

Vector3 Enemy::GetScaleEnemyAttackCollision()
{
	Vector3 EnemyAttackCollisionScale;

	EnemyAttackCollisionScale.x = worldTransformEnemyCollision_[0].scale_.x;
	EnemyAttackCollisionScale.y = worldTransformEnemyCollision_[0].scale_.y;
	EnemyAttackCollisionScale.z = worldTransformEnemyCollision_[0].scale_.z;

	return EnemyAttackCollisionScale;
}



//Vector3 Enemy::GetWorldTransformEnemyAttackCollision()
//{
//	Vector3 EnemyAttackCollisionWorldPos[10];
//
//	for (int i = 0; i < 10; i++) {
//		EnemyAttackCollisionWorldPos[i].x = worldTransformAttackCollision_[i].translation_.x;
//		EnemyAttackCollisionWorldPos[i].y = worldTransformAttackCollision_[i].translation_.y;
//		EnemyAttackCollisionWorldPos[i].z = worldTransformAttackCollision_[i].translation_.z;
//	}
//
//	for (int i = 0; i < 10; i++) {
//		return EnemyAttackCollisionWorldPos[i];
//	}
//}
