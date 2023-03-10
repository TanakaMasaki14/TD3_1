#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("sora.png");
	stage1Sprite_ = Sprite::Create(textureHandle_, { 0, 0 });
}

void GameScene::Finalize() {
	delete stage1Sprite_;
	//delete input_;
}

void GameScene::Update() {

	////入力の更新
	//input_->Update();
	
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
		//次のシーンへ
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 0;
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
		stage1Sprite_->Draw();
		break;

	case 1:// ステージ1
		stage1Sprite_->Draw();
		break;
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();
	// 3Dオブジェクト描画後処理
	//model_->PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
}
