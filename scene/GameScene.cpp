#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "SafeDelete.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	SafeDelete(modelPlayerArm_);
	SafeDelete(playerArm_);
	SafeDelete(modelPlayerFace_);
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	soundHandleTitle_ = audio_->LoadWave("Neownch.mp3");
	audio_->PlayWave(soundHandleTitle_);
	textureHandleStage1_ = TextureManager::Load("stage1.png");
	stage1Sprite_ = Sprite::Create(textureHandleStage1_, { 0, 0 });
	textureHandleStage2_ = TextureManager::Load("stage2.png");
	stage2Sprite_ = Sprite::Create(textureHandleStage2_, { 0, 0 });
	textureHandleStage3_ = TextureManager::Load("stage3.png");
	stage3Sprite_ = Sprite::Create(textureHandleStage3_, { 0, 0 });
	textureHandlePlayerArm_ = TextureManager::Load("blocktest.png");
	modelPlayerArm_ = Model::Create();
	playerArm_ = new PlayerArm();
	modelPlayerFace_ = Model::Create();
	playerArm_->Initialize(modelPlayerArm_, modelPlayerFace_, textureHandlePlayerArm_);
	//worldTransform_.Initialize();
	viewProjection_.Initialize();
	//skyDome_ = Model::CreateFromOBJ("sora", true);
}

void GameScene::Finalize() {
	delete stage1Sprite_;
	delete stage2Sprite_;
	delete stage3Sprite_;
	//delete skyDome_;
}

void GameScene::Update() {
	
	switch (scene)
	{
	case 0://タイトル
		debugText_->Print("Title", 300, 300, 2.0f);
		soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true);
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->StopWave(soundHandleLoop_);
			scene = 1;
		}
		break;

	case 1://ステージ１
		debugText_->Print("Stage1", 300, 300, 2.0f);
		debugText_->Print("F = shake", 300, 0, 2.0f);
		if (input_->TriggerKey(DIK_T)) {
			SpeedBuffer = playerArm_->GetSpeed();
			stop = true;
		}
		Stop();
		playerArm_->Update();
		viewProjection_.UpdateMatrix();

		if (input_->TriggerKey(DIK_F)) {
			shakeFlag = 1;
		}
		if (shakeFlag == 1) {
			shakeTimer--;
			if (shakeTimer > 0) {
			}
			if (shakeTimer == 25) {
				stage1Sprite_ = Sprite::Create(textureHandleStage1_, { -25, -25 });
			}
			if (shakeTimer == 20) {
				stage1Sprite_ = Sprite::Create(textureHandleStage1_, { 25, 25 });
			}
			if (shakeTimer == 15) {
				stage1Sprite_ = Sprite::Create(textureHandleStage1_, { 25, -25 });
			}
			if (shakeTimer == 10) {
				stage1Sprite_ = Sprite::Create(textureHandleStage1_, { -25,  25 });
			}
			if (shakeTimer == 0) {
				stage1Sprite_ = Sprite::Create(textureHandleStage1_, { 0, 0 });
				shakeTimer = 30;
				shakeFlag = 0;
			}
		}
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 2;
		}
		break;
	case 2://ステージ２
		debugText_->Print("Stage2", 300, 300, 2.0f);
		if (input_->TriggerKey(DIK_T)) {
			SpeedBuffer = playerArm_->GetSpeed();
			stop = true;
		}
		Stop();
		playerArm_->Update();
		viewProjection_.UpdateMatrix();
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 3;
		}
		break;
	case 3://ステージ３
		debugText_->Print("Stage3", 300, 300, 2.0f);
		if (input_->TriggerKey(DIK_T)) {
			SpeedBuffer = playerArm_->GetSpeed();
			stop = true;
		}
		Stop();
		playerArm_->Update();
		viewProjection_.UpdateMatrix();
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = 0;
		}
		break;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//描画前処理
	Sprite::PreDraw(commandList);

	switch (scene)
	{

	case 0:// タイトル
		break;
	case 1:// ステージ1
		stage1Sprite_->Draw();
		break;
	case 2:// ステージ2
		stage2Sprite_->Draw();
		break;
	case 3:// ステージ3
		stage3Sprite_->Draw();
		break;
	}
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
	//描画後処理
	Sprite::PostDraw();

	Model::PreDraw(commandList);
	
	switch (scene)
	{

	case 0:// タイトル
		break;
	case 1:// ステージ1
		playerArm_->Draw(viewProjection_);
		break;
	case 2:// ステージ2
		playerArm_->Draw(viewProjection_);
		break;
	case 3:// ステージ3
		playerArm_->Draw(viewProjection_);
		break;
	}
	Model::PostDraw();
}

void GameScene::Stop() {

	if (stop == true) {
		stopTimer_ -= 1;
		if (stopTimer_ > 0) {
			playerArm_->SetSpeed({ 0.0f,0.0f,0.0f });
		}
		if (stopTimer_ < 0) {
			playerArm_->SetSpeed(SpeedBuffer);
			SpeedBuffer = { 0,0,0 };
			stop = false;
			stopTimer_ = stopSecond_ * 60;
		}
	}
}