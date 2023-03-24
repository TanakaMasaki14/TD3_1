#include "Enemy.h"

#include <cassert>

#include "Procession.h"
using namespace MathUtility;
using namespace DirectX;


Enemy::~Enemy()
{
	SafeDelete(modelAttackRange_);
	
}

void Enemy::Initialize(Model* model, Model* modelFace, uint32_t textureHandle)
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

	///
	worldTransform_.scale_ = { 20.0f,2.5f,2.5f };
	worldTransform_.rotation_ = { 0.0f,0.0f, XMConvertToRadians(30.0f) };
	worldTransform_.translation_ = { -25.0f,-5.0f,0.0f };

	worldTransform_.Initialize();

	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	///
	worldTransformFace_.scale_ = { 4.0f,4.0f,4.0f };
	worldTransformFace_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformFace_.translation_ = { -44.0f,27.0f,0.0f };

	worldTransformFace_.Initialize();

	worldTransformFace_.matWorld_ = Mat_Identity();
	worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);

	worldTransformFace_.TransferMatrix();

	///
	worldTransformAttackrange_.scale_ = { 0.0f,0.0f,0.0f };
	worldTransformAttackrange_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformAttackrange_.translation_ = { 0.0f,0.0f,0.0f };

	worldTransformAttackrange_.Initialize();

	worldTransformAttackrange_.matWorld_ = Mat_Identity();
	worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);

	worldTransformAttackrange_.TransferMatrix();
}

void Enemy::Update()
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

			worldTransformAttackrange_.scale_ = { 2.0f,10.0f,1.0f };

			weakAttack_ = true;
		}
	}

	if (input_->TriggerKey(DIK_O)) {
		if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
			motionspeedX = 0.5f;
			motionspeedY = 1.0f;

			attackbufferX_ = worldTransform_.translation_.x;
			attackbufferY_ = worldTransform_.translation_.y;

			worldTransformAttackrange_.scale_ = { 3.0f,10.0f,1.0f };

			heavyAttack_ = true;
			
		}
	}

	if (input_->TriggerKey(DIK_P)) {
		if (block_ == false && weakAttack_ == false && heavyAttack_ == false && stunAttack_ == false) {
			attackbufferX_ = -43.0f;
			attackbufferY_ = -5.0f;

			worldTransformAttackrange_.translation_.x = attackbufferX_;
			worldTransformAttackrange_.translation_.y = attackbufferY_;

			worldTransformAttackrange_.scale_ = { 2.0f,20.0f,1.0f };

			stunAttack_ = true;
		}
	}

	Motion();

	///çXêV
	//
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	//
	worldTransformFace_.matWorld_ = Mat_Identity();
	worldTransformFace_.matWorld_ = MatWorld(worldTransformFace_.scale_, worldTransformFace_.rotation_, worldTransformFace_.translation_);
	worldTransformFace_.TransferMatrix();

	//
	worldTransformAttackrange_.matWorld_ = Mat_Identity();
	worldTransformAttackrange_.matWorld_ = MatWorld(worldTransformAttackrange_.scale_, worldTransformAttackrange_.rotation_, worldTransformAttackrange_.translation_);
	worldTransformAttackrange_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	modelFace_->Draw(worldTransformFace_, viewProjection, textureHandle_);

	if (attackrange_ == true) {
		modelAttackRange_->Draw(worldTransformAttackrange_, viewProjection, textureHandle_);
	}
}

void Enemy::Motion()
{
	Block();
	WeakAttack();
	HeavyAttack();
	StunAttack();
}

void Enemy::Block()
{
	if (block_) {
		blockStartmotionFrame_ -= 1;

		///ÉuÉçÉbÉNëOÉÇÅ[ÉVÉáÉì
		if (blockStartmotionFrame_ > 0) {
			//à⁄ìÆ
			worldTransform_.translation_.x += 3.0f;
			worldTransform_.translation_.y += 1.0f;
			//âÒì]
			worldTransform_.rotation_.z += 0.3f;
			if (worldTransform_.rotation_.z > 1.55f) {
				worldTransform_.rotation_.z = 1.55f;
			}
		}

		if (blockStartmotionFrame_ < 0) {
			blockChanceFrame_ -= 1;
		}


		//ÉuÉçÉbÉNíÜÉÇÅ[ÉVÉáÉì
		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ > 0) {

		}

		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ < 0) {
			blockEndmotionFlame_ -= 1;
		}


		///ÉuÉçÉbÉNå„ÉÇÅ[ÉVÉáÉì
		if (blockStartmotionFrame_ < 0 && blockChanceFrame_ < 0 && blockEndmotionFlame_ > 0) {
			//à⁄ìÆ
			worldTransform_.translation_.x -= 3.0f;
			worldTransform_.translation_.y -= 1.0f;
			//âÒì]
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
		}
	}
}

void Enemy::WeakAttack()
{
	if (weakAttack_) {
		weakStartmotionFrame_ -= 1;

		///é„çUåÇëOÉÇÅ[ÉVÉáÉì
		if (weakStartmotionFrame_ > 0) {
			//à⁄ìÆ
			worldTransform_.translation_.y += motionspeedY; //èâä˙íl4.0f
			worldTransform_.translation_.x += motionspeedX; //èâä˙íl2.0f
			motionspeedX += 1.0f;
			//âÒì]
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


		///é„çUåÇíÜÉÇÅ[ÉVÉáÉì
		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ > 0) {
			///à⁄ìÆ
			//èc
			worldTransform_.translation_.y += motionspeedY;
			//â°
			if (weakAttackingFrame_ >= 9) {
				worldTransform_.translation_.x += 9.8f;
			}
			if (weakAttackingFrame_ == 8) {

			}
			if (weakAttackingFrame_ <= 7 && weakAttackingFrame_ > 0) {
				worldTransform_.translation_.x -= 1.4f;
			}

			//à⁄ìÆêßå¿(-13)
			if ((worldTransform_.translation_.y - bufferpointY) < -13) {
				motionspeedY = 0;
				worldTransform_.translation_.y = bufferpointY - 13.0f;
			}

			//âÒì]
			worldTransform_.rotation_.z -= 0.4f;
			if (worldTransform_.rotation_.z < -0.6f) {
				worldTransform_.rotation_.z = -0.6f;
			}

			//çUåÇîªíËà íu
			attackrange_ = true;

			worldTransformAttackrange_.translation_.x = attackbufferX_ + 30.0f;
			worldTransformAttackrange_.translation_.y = attackbufferY_;
		}


		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ == 0) {
			bufferpointX = worldTransform_.translation_.x;
			bufferpointY = worldTransform_.translation_.y;

			//çUåÇîªíËè¡ñ≈
			attackrange_ = false;
		}


		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ < 0) {
			weakEndmotionFrame_ -= 1;
		}



		///é„çUåÇå„ÉÇÅ[ÉVÉáÉì
		if (weakStartmotionFrame_ < 0 && weakAttackingFrame_ < 0 && weakEndmotionFrame_ > 0) {
			//à⁄ìÆ
			worldTransform_.translation_.x -= 2.0f;
			worldTransform_.translation_.y += 2.0f;
			if ((worldTransform_.translation_.x - bufferpointX) < 5) {
				worldTransform_.translation_.x = bufferpointX - 5;
			}
			if ((worldTransform_.translation_.y - bufferpointY) > 5) {
				worldTransform_.translation_.y = bufferpointY + 5;
			}

			//âÒì]
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
		}
	}
}

void Enemy::HeavyAttack()
{
	if (heavyAttack_) {
		heavyStartmotionFrame_ -= 1;

		///ã≠çUåÇëOÉÇÅ[ÉVÉáÉì
		if (heavyStartmotionFrame_ > 0) {
			worldTransform_.translation_.y += motionspeedY;
			worldTransform_.translation_.x += motionspeedX;
			motionspeedX += 0.1f;

			//âÒì]
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

		//ã≠çUåÇíÜÉÇÅ[ÉVÉáÉì
		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ > 0) {
			///à⁄ìÆ
			//èc
			worldTransform_.translation_.y += motionspeedY;
			//â°
			if (heavyAttackingFrame_ >= 9) {
				worldTransform_.translation_.x += 9.8f;
			}
			if (heavyAttackingFrame_ == 8) {

			}
			if (heavyAttackingFrame_ <= 7 && heavyAttackingFrame_ > 0) {
				worldTransform_.translation_.x -= 1.4f;
			}

			//à⁄ìÆêßå¿(-13)
			if ((worldTransform_.translation_.y - bufferpointY) < -13) {
				motionspeedY = 0;
				worldTransform_.translation_.y = bufferpointY - 13.0f;
			}

			//âÒì]
			worldTransform_.rotation_.z -= 0.4f;
			if (worldTransform_.rotation_.z < -0.6f) {
				worldTransform_.rotation_.z = -0.6f;
			}

			//çUåÇîªíËà íu
			attackrange_ = true;

			worldTransformAttackrange_.translation_.x = attackbufferX_ + 35.0f;
			worldTransformAttackrange_.translation_.y = attackbufferY_;
		}

		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ == 0) {
			bufferpointX = worldTransform_.translation_.x;
			bufferpointY = worldTransform_.translation_.y;

			//çUåÇîªíËè¡ñ≈
			attackrange_ = false;
		}


		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ < 0) {
			heavyEndmotionFrame_ -= 1;
		}


		///ã≠çUåÇå„ÉÇÅ[ÉVÉáÉì
		if (heavyStartmotionFrame_ < 0 && heavyAttackingFrame_ < 0 && heavyEndmotionFrame_ > 0) {
			//à⁄ìÆ
			worldTransform_.translation_.x -= 2.5f;
			worldTransform_.translation_.y += 2.0f;
			if ((worldTransform_.translation_.x - bufferpointX) < 11) {
				worldTransform_.translation_.x = bufferpointX - 11;
			}
			if ((worldTransform_.translation_.y - bufferpointY) > 2) {
				worldTransform_.translation_.y = bufferpointY + 2;
			}

			//âÒì]
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
		}
		
	}
}

void Enemy::StunAttack()
{
	if (stunAttack_) {
		stunStartmotionFrame_ -= 1;
		//çUåÇëOÉÇÅ[ÉVÉáÉì
		if (stunStartmotionFrame_ > 0) {

		}

		if (stunStartmotionFrame_ < 0) {
			stunAttackingFrame_ -= 1;
		}


		//çUåÇíÜ
		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ > 0) {

			//37.0f, 20.0f, 0.0f x:7 y:7
			if (stunAttackingFrame_ > 55) {
				worldTransformFace_.translation_.x += 1.4f;
				worldTransformFace_.translation_.y -= 1.4f;
			}

			attackrange_ = true;
			worldTransformAttackrange_.translation_.x += 1.3f;
		}

		if (stunStartmotionFrame_ < 0 && stunAttackingFrame_ < 0) {
			attackrange_ = false;
			stunEndmotionFrame_ -= 1;

		}


		//çUåÇå„ÉÇÅ[ÉVÉáÉì
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
			
		}
	}
}


