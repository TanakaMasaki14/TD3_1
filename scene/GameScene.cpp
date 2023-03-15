#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <math.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("sora.png");
	stage1Sprite_ = Sprite::Create(textureHandle_, { 0, 0 });
	worldTransform_.Initialize();
	viewProjection_.eye = { 0,0,0 };
	viewProjection_.Initialize();
	
	//skyDome_ = Model::CreateFromOBJ("sora", true);
}

void GameScene::Finalize() {
	delete stage1Sprite_;
	//delete skyDome_;
}

void GameScene::Update() {
	
	switch (scene)
	{
	case 0:// タイトル
		debugText_->Print("Title", 300, 300, 5.0f);
		//次のシーンへ
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 1;
		}
		break;

	case 1:// ステージ1
		debugText_->Print("Stage1", 300, 300, 5.0f);
		debugText_->Print("F = shake", 300, 0, 3.0f);
		//次のシーンへ
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 0;
		}
		if (input_->TriggerKey(DIK_F)) {
			shakeFlag = 1;
		}
		if (shakeFlag == 1) {
			shakeTimer--;
			if (shakeTimer > 0) {
			}
			if (shakeTimer == 25) {
				stage1Sprite_ = Sprite::Create(textureHandle_, { -25, -25 });
			}
			if (shakeTimer == 20) {
				stage1Sprite_ = Sprite::Create(textureHandle_, { 25, 25 });
			}
			if (shakeTimer == 15) {
				stage1Sprite_ = Sprite::Create(textureHandle_, { 25, -25 });
			}
			if (shakeTimer == 10) {
				stage1Sprite_ = Sprite::Create(textureHandle_, { -25,  25});
			}
			if (shakeTimer == 0) {
				stage1Sprite_ = Sprite::Create(textureHandle_, { 0, 0 });
				shakeTimer = 30;
				shakeFlag = 0;
			}
		}
		break;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// スプライト描画前処理
	Sprite::PreDraw(commandList);           
	// 3Dオブジェクト描画前処理
	//Model::PreDraw(commandList);

	switch (scene)
	{
	case 0:// タイトル
		break;

	case 1:// ステージ1
		//skyDome_->Draw();
		stage1Sprite_->Draw();
		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();
	// 3Dオブジェクト描画後処理
	//Model::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
}