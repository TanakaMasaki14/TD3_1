#include "PlayerArm.h"
#include <cassert>

#include "Procession.h"
using namespace MathUtility;
using namespace DirectX;

PlayerArm::~PlayerArm()
{
	SafeDelete(modelAttackRange_);
}

void PlayerArm::Initialize(Model* model, Model* modelFace, uint32_t textureHandle)
{
	assert(model);
	assert(modelFace);

	model_ = model;
	modelFace_ = modelFace;
	textureHandle_ = textureHandle;

	modelAttackRange_ = Model::Create();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	soundHandleWeak_ = audio_->LoadWave("weak.mp3");
	soundHandleHeavy_ = audio_->LoadWave("heavy.mp3");
	soundHandleStun_ = audio_->LoadWave("stun.mp3");
	///
	worldTransform_.scale_ = { 20.0f,2.5f,2.5f };
	worldTransform_.rotation_ = { 0.0f,0.0f, XMConvertToRadians(radius_.z) };
	worldTransform_.translation_ = { 25.0f,-5.0f,0.0f };

	worldTransform_.Initialize();

	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	///
	worldTransformFace_.scale_ = { 4.0f,4.0f,4.0f };
	worldTransformFace_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformFace_.translation_ = { 44.0f,27.0f,0.0f };

	worldTransformFace_.Initialize();

	worldTransformFace_.matWorld_ = Mat_Identity();
	worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);

	worldTransformFace_.TransferMatrix();

	worldTransformAttackrange_.scale_ = { 0.0f,0.0f,0.0f };
	worldTransformAttackrange_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformAttackrange_.translation_ = { 0.0f,0.0f,0.0f };

	worldTransformAttackrange_.Initialize();

	worldTransformAttackrange_.matWorld_ = Mat_Identity();
	worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);

	worldTransformAttackrange_.TransferMatrix();

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

			worldTransformAttackrange_.scale_ = { 2.0f,10.0f,1.0f };

			weakAttack_ = true;
			movement_ = false;
			audio_->PlayWave(soundHandleWeak_, false, 3);
		}
	}

	if (input_->TriggerKey(DIK_L)) {
		if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
			motionspeedX = 0.5f;
			motionspeedY = 1.0f;

			attackbufferX_ = worldTransform_.translation_.x;
			attackbufferY_ = worldTransform_.translation_.y;

			worldTransformAttackrange_.scale_ = { 3.0f,10.0f,1.0f };

			heavyAttack_ = true;
			movement_ = false;
			audio_->PlayWave(soundHandleHeavy_, false, 3);
		}
	}

	if (input_->TriggerKey(DIK_N)) {
		if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
			attackbufferX_ = 43.0f;
			attackbufferY_ = -5.0f;

			worldTransformAttackrange_.translation_.x = attackbufferX_;
			worldTransformAttackrange_.translation_.y = attackbufferY_;

			worldTransformAttackrange_.scale_ = { 2.0f,20.0f,1.0f };

			stunAttack_ = true;
			movement_ = false;
			audio_->PlayWave(soundHandleStun_, false, 8);
		}
	}


	/*debugText_->SetPos(0, 0);
	debugText_->Printf("rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(0, 60);
	debugText_->Printf("translation:(%f,%f,%f)", worldTransformFace_.translation_.x, worldTransformFace_.translation_.y, worldTransformFace_.translation_.z);

	debugText_->SetPos(0, 80);
	debugText_->Printf("block:%d,weak:%d,heavy:%d,stun:%d", block_, weakAttack_, heavyAttack_, stunAttack_);*/


	///回す方向忘れないように
	if (input_->PushKey(DIK_LEFTARROW)) {
		worldTransform_.rotation_.z += 0.1f;
	}

	if (input_->PushKey(DIK_RIGHTARROW)) {
		worldTransform_.rotation_.z -= 0.1f;
	}

	///モーションまとめ
	Motion();

	///更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	worldTransformFace_.matWorld_ = Mat_Identity();
	worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);
	worldTransformFace_.TransferMatrix();

	worldTransformAttackrange_.matWorld_ = Mat_Identity();
	worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);
	worldTransformAttackrange_.TransferMatrix();

}

void PlayerArm::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	modelFace_->Draw(worldTransformFace_, viewProjection, textureHandle_);

	if (attackrange_ == true) {
		modelAttackRange_->Draw(worldTransformAttackrange_, viewProjection, textureHandle_);
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
		}


		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ == 0) {
			bufferpointX = worldTransform_.translation_.x;
			bufferpointY = worldTransform_.translation_.y;
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
		}

		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ == 0) {
			bufferpointX = worldTransform_.translation_.x;
			bufferpointY = worldTransform_.translation_.y;
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
		if (stunStartmotionFrame_ > 0) {

		}

		if (stunStartmotionFrame_ < 0) {
			stunAttackingFrame_ -= 1;
		}



		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ > 0) {

			//37.0f, 20.0f, 0.0f x:7 y:7
			if (stunAttackingFrame_ > 55) {
				worldTransformFace_.translation_.x -= 1.4f;
				worldTransformFace_.translation_.y -= 1.4f;
			}
			attackrange_ = true;
			worldTransformAttackrange_.translation_.x -= 1.3f;
		}

		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ < 0) {
			attackrange_ = false;
			stunEndmotionFrame_ -= 1;
		}



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