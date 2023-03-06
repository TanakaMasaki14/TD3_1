#include "PlayerArm.h"
#include <cassert>

#include "Procession.h"
using namespace MathUtility;
using namespace DirectX;

void PlayerArm::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.scale_ = { 20.0f,2.5f,2.5f };
	worldTransform_.rotation_ = { 0.0f,0.0f, XMConvertToRadians(radius_.z) };
	worldTransform_.translation_ = { 25.0f,-5.0f,0.0f };

	worldTransform_.Initialize();

	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void PlayerArm::Update()
{
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


	if (input_->TriggerKey(DIK_B)) {
		block_ = true;
	}






	///回す方向忘れないように
	if (input_->PushKey(DIK_LEFTARROW)) {
		worldTransform_.rotation_.z = XMConvertToRadians(radius_.z += 0.5f);
	}

	if (input_->PushKey(DIK_RIGHTARROW)) {
		worldTransform_.rotation_.z = XMConvertToRadians(radius_.z -= 0.5f);
	}

	///モーションまとめ
	Motion();

	///更新
	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PlayerArm::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

/// <summary>
/// モーションぶち込み場
/// </summary>
void PlayerArm::Motion()
{
	Block();
}

/// <summary>
/// ブロックモーション
/// </summary>
void PlayerArm::Block()
{
	if (block_) {
		blockstartmotionframe_ -= 1;
		if (blockstartmotionframe_ > 0) {
			worldTransform_.rotation_.z = XMConvertToRadians(radius_.z -= 12.0f);
			worldTransform_.translation_.x -= 3.0f;
			worldTransform_.translation_.y += 1.0f;
		}

		if (blockstartmotionframe_ < 0) {
			blockchanceframe_ -= 1;
		}

		if (blockstartmotionframe_ < 0 && blockchanceframe_ > 0) {

		}

		if (blockstartmotionframe_ < 0 && blockchanceframe_ < 0) {
			blockendmotionflame_ -= 1;
		}

		if (blockstartmotionframe_ < 0 && blockchanceframe_ < 0 && blockendmotionflame_>0) {
			worldTransform_.rotation_.z = XMConvertToRadians(radius_.z += 12.0f);
			worldTransform_.translation_.x += 3.0f;
			worldTransform_.translation_.y -= 1.0f;
		}

		if (blockstartmotionframe_ < 0 && blockchanceframe_ < 0 && blockendmotionflame_ < 0) {
			blockstartmotionframe_ = 6;
			blockchanceframe_ = 60;
			blockendmotionflame_ = 6;
			block_ = false;
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




