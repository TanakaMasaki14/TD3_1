#include "PlayerArm.h"
#include <cassert>
#include <cmath>

#include "Procession.h"
#include <corecrt_math_defines.h>
using namespace MathUtility;
using namespace DirectX;
using namespace std;

PlayerArm::~PlayerArm()
{
	SafeDelete(modelAttackRange_);
	SafeDelete(modelPlayerCollision_);
	SafeDelete(modelAttackCollision_);
}

void PlayerArm::Initialize(Model* model, Model* modelFace, uint32_t textureHandle)
{
	assert(model);
	assert(modelFace);

	model_ = model;
	modelFace_ = modelFace;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	modelAttackRange_ = Model::Create();

	modelPlayerCollision_ = Model::CreateFromOBJ("C");
	modelAttackCollision_ = Model::CreateFromOBJ("C");

	///腕
	{
		worldTransform_.scale_ = { 20.0f,2.5f,2.5f };
		worldTransform_.rotation_ = { 0.0f,0.0f, XMConvertToRadians(radius_.z) };
		worldTransform_.translation_ = { 25.0f,-5.0f,0.0f };

		worldTransform_.Initialize();

		worldTransform_.matWorld_ = Mat_Identity();
		worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

		worldTransform_.TransferMatrix();
	}

	///顔
	{
		worldTransformFace_.scale_ = { 4.0f,4.0f,4.0f };
		worldTransformFace_.rotation_ = { 0.0f,0.0f,0.0f };
		worldTransformFace_.translation_ = { 44.0f,27.0f,0.0f };

		worldTransformFace_.Initialize();

		worldTransformFace_.matWorld_ = Mat_Identity();
		worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);

		worldTransformFace_.TransferMatrix();
	}

	///腕当たり判定エリア
	{
		radian = radius_.z * M_PI / 180.0;
		//0
		worldTransformPlayerCollision_[0].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[0].rotation_ = { 0.0f,0.0f,0.0f };
		worldTransformPlayerCollision_[0].translation_ = { worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z };



		//1
		worldTransformPlayerCollision_[1].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[1].rotation_ = { 0.0f,0.0f,0.0f };
		r[0] = 5.0f; //距離
		worldTransformPlayerCollision_[1].translation_.x = worldTransform_.translation_.x + r[0] * cos(static_cast<float> (radian));
		worldTransformPlayerCollision_[1].translation_.y = worldTransform_.translation_.y + r[0] * sin(static_cast<float> (radian));
		worldTransformPlayerCollision_[1].translation_.z = 0.0f;


		//2
		worldTransformPlayerCollision_[2].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[2].rotation_ = { 0.0f,0.0f,0.0f };
		r[1] = -5.0f;
		worldTransformPlayerCollision_[2].translation_.x = worldTransform_.translation_.x + r[1] * cos(static_cast<float>(radian));
		worldTransformPlayerCollision_[2].translation_.y = worldTransform_.translation_.y + r[1] * sin(static_cast<float>(radian));
		worldTransformPlayerCollision_[2].translation_.z = 0.0f;

		//3
		worldTransformPlayerCollision_[3].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[3].rotation_ = { 0.0f,0.0f,0.0f };
		r[2] = 10.0f;
		worldTransformPlayerCollision_[3].translation_.x = worldTransform_.translation_.x + r[2] * cos(static_cast<float>(radian));
		worldTransformPlayerCollision_[3].translation_.y = worldTransform_.translation_.y + r[2] * sin(static_cast<float>(radian));
		worldTransformPlayerCollision_[3].translation_.z = 0.0f;

		//4
		worldTransformPlayerCollision_[4].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[4].rotation_ = { 0.0f,0.0f,0.0f };
		r[3] = -10.0f;
		worldTransformPlayerCollision_[4].translation_.x = worldTransform_.translation_.x + r[3] * cos(static_cast<float>(radian));
		worldTransformPlayerCollision_[4].translation_.y = worldTransform_.translation_.y + r[3] * sin(static_cast<float>(radian));
		worldTransformPlayerCollision_[4].translation_.z = 0.0f;

		//5
		worldTransformPlayerCollision_[5].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[5].rotation_ = { 0.0f,0.0f,0.0f };
		r[4] = 15.0f;
		worldTransformPlayerCollision_[5].translation_.x = worldTransform_.translation_.x + r[4] * cos(static_cast<float>(radian));
		worldTransformPlayerCollision_[5].translation_.y = worldTransform_.translation_.y + r[4] * sin(static_cast<float>(radian));
		worldTransformPlayerCollision_[5].translation_.z = 0.0f;

		//6
		worldTransformPlayerCollision_[6].scale_ = { 2.5f,2.5f,2.5f };
		worldTransformPlayerCollision_[6].rotation_ = { 0.0f,0.0f,0.0f };
		r[5] = -15.0f;
		worldTransformPlayerCollision_[6].translation_.x = worldTransform_.translation_.x + r[5] * cos(static_cast<float>(radian));
		worldTransformPlayerCollision_[6].translation_.y = worldTransform_.translation_.y + r[5] * sin(static_cast<float>(radian));
		worldTransformPlayerCollision_[6].translation_.z = 0.0f;


		for (int i = 0; i < PlayerCollisionquantity; i++) {
			worldTransformPlayerCollision_[i].Initialize();

			worldTransformPlayerCollision_[i].matWorld_ = Mat_Identity();
			worldTransformPlayerCollision_[i].matWorld_ = MatWorld(worldTransformPlayerCollision_[i].scale_, worldTransformPlayerCollision_[i].rotation_, worldTransformPlayerCollision_[i].translation_);

			worldTransformPlayerCollision_[i].TransferMatrix();
		}
	}


	///攻撃当たり判定エリア
	{
		//
		worldTransformAttackrange_.scale_ = { 0.0f,0.0f,0.0f };
		worldTransformAttackrange_.rotation_ = { 0.0f,0.0f,0.0f };
		worldTransformAttackrange_.translation_ = { 0.0f,0.0f,0.0f };

		worldTransformAttackrange_.Initialize();

		worldTransformAttackrange_.matWorld_ = Mat_Identity();
		worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);

		worldTransformAttackrange_.TransferMatrix();

		//
		for (int i = 0; i < 10; i++) {
			worldTransformAttackCollision_[i].scale_ = {0.0f,0.0f,0.0f};
			worldTransformAttackCollision_[i].rotation_ = {0.0f,0.0f,0.0f};
			worldTransformAttackCollision_[i].translation_ = {1000.0f,0.0f,0.0f};

			worldTransformAttackCollision_[i].Initialize();

			worldTransformAttackCollision_[i].matWorld_ = Mat_Identity();
			worldTransformAttackCollision_[i].matWorld_ = MatWorld(worldTransformAttackCollision_[i].scale_, worldTransformAttackCollision_[i].rotation_, worldTransformAttackCollision_[i].translation_);

			worldTransformAttackCollision_[i].TransferMatrix();
		}
	}

}

void PlayerArm::Update()
{
	if (movement_ == true) {
		///移動
		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_.y += Armspeed_.y;
		}
		if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_.y -= Armspeed_.y;
		}
		if (input_->PushKey(DIK_A)) {
			worldTransform_.translation_.x -= Armspeed_.x;
		}
		if (input_->PushKey(DIK_D)) {
			worldTransform_.translation_.x += Armspeed_.x;
		}
	}

	if (input_->TriggerKey(DIK_B)) {
		if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
			block_ = true;
			movement_ = false;
		}
	}

	if (input_->TriggerKey(DIK_K)) {
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
			movement_ = false;
		}
	}

	if (input_->TriggerKey(DIK_L)) {
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
			movement_ = false;
		}
	}

	if (input_->TriggerKey(DIK_N)) {
		if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
			attackbufferX_ = 43.0f;
			attackbufferY_ = -5.0f;

			worldTransformAttackrange_.translation_.x = attackbufferX_;
			worldTransformAttackrange_.translation_.y = attackbufferY_;

			worldTransformAttackrange_.scale_ = { 2.0f,20.0f,2.0f };

			for (int i = 0; i < 10; i++) {
				worldTransformAttackCollision_[i].scale_ = { 2.0f,2.0f,2.0f };
			}

			stunAttack_ = true;
			movement_ = false;
		}
	}


	debugText_->SetPos(0, 0);
	debugText_->Printf("rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,  worldTransform_.rotation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(0, 60);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransformFace_.translation_.x, worldTransformFace_.translation_.y, worldTransformFace_.translation_.z);

	debugText_->SetPos(0, 80);
	debugText_->Printf("block:%d,weak:%d,heavy:%d,stun:%d", block_, weakAttack_, heavyAttack_, stunAttack_);


	///回す方向忘れないように
	if (input_->PushKey(DIK_LEFTARROW)) {
		worldTransform_.rotation_.z += 0.1f;
	}

	if (input_->PushKey(DIK_RIGHTARROW)) {
		worldTransform_.rotation_.z -= 0.1f;
	}

	


	///モーションまとめ
	Motion();

	///当たり判定エリア更新(rは-1したものが正しい)
	//0
	worldTransformPlayerCollision_[0].translation_ = worldTransform_.translation_;
	//1以降
	for (int i = 1; i < PlayerCollisionquantity; i++) {
		//1 
		worldTransformPlayerCollision_[i].translation_.x = worldTransform_.translation_.x + r[i - 1] * cos(worldTransform_.rotation_.z);
		worldTransformPlayerCollision_[i].translation_.y = worldTransform_.translation_.y + r[i - 1] * sin(worldTransform_.rotation_.z);
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
	for (int i = 0; i < PlayerCollisionquantity; i++) {
		worldTransformPlayerCollision_[i].matWorld_ = Mat_Identity();
		worldTransformPlayerCollision_[i].matWorld_ = MatWorld(worldTransformPlayerCollision_[i].scale_, worldTransformPlayerCollision_[i].rotation_, worldTransformPlayerCollision_[i].translation_);
		worldTransformPlayerCollision_[i].TransferMatrix();
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

void PlayerArm::Draw(ViewProjection& viewProjection)
{
	if (input_->PushKey(DIK_Q) == 0) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
	}
	modelFace_->Draw(worldTransformFace_, viewProjection, textureHandle_);

	for (int i = 0; i < PlayerCollisionquantity; i++) {
		modelPlayerCollision_->Draw(worldTransformPlayerCollision_[i], viewProjection);
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

/// <summary>
/// モーションぶち込み場
/// </summary>
void PlayerArm::Motion()
{
	Block();
	WeakAttack();
	HeavyAttack();
	StunAttack();
}

/// <summary>
/// ブロックモーション
/// </summary>
void PlayerArm::Block()
{
	if (block_) {
		blockStartmotionFrame_ -= 1;

		///ブロック前モーション
		if (blockStartmotionFrame_ > 0) {
			//移動
			worldTransform_.translation_.x -= 3.0f;
			worldTransform_.translation_.y += 1.0f;
			//回転
			worldTransform_.rotation_.z -= 0.3f;
			if (worldTransform_.rotation_.z < -1.55f) {
				worldTransform_.rotation_.z = -1.55f;
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
			worldTransform_.translation_.x += 3.0f;
			worldTransform_.translation_.y -= 1.0f;
			//回転
			worldTransform_.rotation_.z += 0.3f;
			if (worldTransform_.rotation_.z > -0.5f) {
				worldTransform_.rotation_.z = -0.5f;
			}
		}

		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ < 0 && blockEndmotionFlame_ < 0) {
			blockStartmotionFrame_ = 6;
			blockChanceFrame_ = 60;
			blockEndmotionFlame_ = 6;
			block_ = false;
			movement_ = true;
		}
	}
}
/// <summary>
/// 弱攻撃
/// </summary>
void PlayerArm::WeakAttack()
{
	if (weakAttack_) {
		weakStartmotionFrame_ -= 1;
	
		///弱攻撃前モーション
		if (weakStartmotionFrame_ > 0) {
			//移動
			worldTransform_.translation_.y += motionspeedY; //初期値4.0f
			worldTransform_.translation_.x -= motionspeedX; //初期値2.0f
			motionspeedX += 1.0f;
			//回転
			worldTransform_.rotation_.z -= 0.3f;
			if (worldTransform_.rotation_.z < -1.4f) {
				worldTransform_.rotation_.z = -1.4f;
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


		//弱攻撃中モーション
		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ > 0) {
			///移動
			//縦
			worldTransform_.translation_.y += motionspeedY;
			//横
			if (weakAttackingFrame_ >= 9) {
				worldTransform_.translation_.x -= 9.8f;
			}
			if (weakAttackingFrame_ == 8) {

			}
			if (weakAttackingFrame_ <= 7 && weakAttackingFrame_ > 0) {
				worldTransform_.translation_.x += 1.4f;
			}

			//移動制限(-13)
			if ((worldTransform_.translation_.y - bufferpointY) < -13) {
				motionspeedY = 0;
				worldTransform_.translation_.y = bufferpointY - 13.0f;
			}
			
			//回転
			worldTransform_.rotation_.z += 0.4f;
			if (worldTransform_.rotation_.z > 0.6f) {
				worldTransform_.rotation_.z = 0.6f;
			}

			//攻撃判定位置
			attackrange_ = true;

			worldTransformAttackrange_.translation_.x = attackbufferX_ - 30.0f;
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
				worldTransformAttackCollision_[i].translation_ = {1000.f,0.0f,0.0f};
				worldTransformAttackCollision_[i].scale_ = {0.0f,0.0f,0.0f};
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
			worldTransform_.translation_.x += 2.0f;
			worldTransform_.translation_.y += 2.0f;
			if ((worldTransform_.translation_.x - bufferpointX) > 5) {
				worldTransform_.translation_.x = bufferpointX + 5;
			}
			if ((worldTransform_.translation_.y - bufferpointY) > 5) {
				worldTransform_.translation_.y = bufferpointY + 5;
			}

			//回転
			worldTransform_.rotation_.z -= 0.5f;
			if (worldTransform_.rotation_.z < -0.5f) {
				worldTransform_.rotation_.z = -0.5f;
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
			movement_ = true;
		}
	}
}
/// <summary>
/// 強攻撃
/// </summary>
void PlayerArm::HeavyAttack()
{
	if (heavyAttack_) {
		heavyStartmotionFrame_ -= 1;

		///強攻撃前モーション
		if (heavyStartmotionFrame_ > 0) {
			worldTransform_.translation_.y += motionspeedY; 
			worldTransform_.translation_.x -= motionspeedX; 
			motionspeedX += 0.1f;

			//回転
			worldTransform_.rotation_.z -= 0.07f;
			if (worldTransform_.rotation_.z < -1.4f) {
				worldTransform_.rotation_.z = -1.4f;
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
				worldTransform_.translation_.x -= 9.8f;
			}
			if (heavyAttackingFrame_ == 8) {

			}
			if (heavyAttackingFrame_ <= 7 && heavyAttackingFrame_ > 0) {
				worldTransform_.translation_.x += 1.4f;
			}

			//移動制限(-13)
			if ((worldTransform_.translation_.y - bufferpointY) < -13) {
				motionspeedY = 0;
				worldTransform_.translation_.y = bufferpointY - 13.0f;
			}

			//回転
			worldTransform_.rotation_.z += 0.4f;
			if (worldTransform_.rotation_.z > 0.6f) {
				worldTransform_.rotation_.z = 0.6f;
			}

			//攻撃判定位置
			attackrange_ = true;

			worldTransformAttackrange_.translation_.x = attackbufferX_ - 35.0f;
			worldTransformAttackrange_.translation_.y = attackbufferY_;

			worldTransformAttackCollision_[0].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[0].translation_.y = worldTransformAttackrange_.translation_.y;

			worldTransformAttackCollision_[1].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[1].translation_.y = worldTransformAttackrange_.translation_.y + 6.0f;

			worldTransformAttackCollision_[2].translation_.x = worldTransformAttackrange_.translation_.x;
			worldTransformAttackCollision_[2].translation_.y = worldTransformAttackrange_.translation_.y - 6.0f;

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
			worldTransform_.translation_.x += 2.5f;
			worldTransform_.translation_.y += 2.0f;
			if ((worldTransform_.translation_.x - bufferpointX) > 11) {
				worldTransform_.translation_.x = bufferpointX + 11;
			}
			if ((worldTransform_.translation_.y - bufferpointY) > 2) {
				worldTransform_.translation_.y = bufferpointY + 2;
			}

			//回転
			worldTransform_.rotation_.z -= 0.5f;
			if (worldTransform_.rotation_.z < -0.5f) {
				worldTransform_.rotation_.z = -0.5f;
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
			movement_ = true;
		}
	}
}
/// <summary>
/// スタン攻撃
/// </summary>
void PlayerArm::StunAttack()
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
				worldTransformFace_.translation_.x -= 1.4f;
				worldTransformFace_.translation_.y -= 1.4f;
			}

			attackrange_ = true;
			worldTransformAttackrange_.translation_.x -= 1.3f;

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
				worldTransformFace_.translation_.x += 1.4f;
				worldTransformFace_.translation_.y += 1.4f;
			}
		}

		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ < 0 && stunEndmotionFrame_ < 0) {
			stunStartmotionFrame_ = 20;
			stunAttackingFrame_ = 60;
			stunEndmotionFrame_ = 12;

			stunAttack_ = false;
			movement_ = true;
		}
	}
}




///アームのセッター,ゲッター
Vector3 PlayerArm::GetWorldTransform()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerArm::SetWorldTransform(Vector3 WorldTransform)
{
	worldTransform_.translation_.x = WorldTransform.x;
	worldTransform_.translation_.y = WorldTransform.y;
	worldTransform_.translation_.z = WorldTransform.z;
}

///スピードのセッター
void PlayerArm::SetSpeed(Vector3 Speed)
{
	Armspeed_.x = Speed.x;
	Armspeed_.y = Speed.y;
	Armspeed_.z = Speed.z;
}




