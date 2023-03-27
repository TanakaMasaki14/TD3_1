#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "SafeDelete.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	SafeDelete(enemy_);
	SafeDelete(playerArm_);

	SafeDelete(modelPlayerFace_);
	SafeDelete(modelPlayerArm_);
	SafeDelete(modelEnemy_);
	SafeDelete(modelEnemyFace_);
}

void GameScene::Initialize() {
	playerArm_ = new PlayerArm();
	enemy_ = new Enemy();


	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	modelPlayerArm_ = Model::Create();
	textureHandlePlayerArm_ = TextureManager::Load("blocktest.png");

	modelPlayerFace_ = Model::Create();

	modelEnemy_ = Model::Create();

	modelEnemyFace_ = Model::Create();


	playerArm_->Initialize(modelPlayerArm_, modelPlayerFace_, textureHandlePlayerArm_);

	enemy_->Initialize(modelEnemy_, modelEnemyFace_, textureHandlePlayerArm_);

	viewProjection_.Initialize();

	textureHandleTitle_ = TextureManager::Load("neownch.png");
	titleSprite_ = Sprite::Create(textureHandleTitle_, { 0, 0 });
	textureHandleStage1_ = TextureManager::Load("stage1.png");
	stage1Sprite_ = Sprite::Create(textureHandleStage1_, { 0, 0 });
	textureHandleStage2_ = TextureManager::Load("stage2.png");
	stage2Sprite_ = Sprite::Create(textureHandleStage2_, { 0, 0 });
	textureHandleStage3_ = TextureManager::Load("stage3.png");
	stage3Sprite_ = Sprite::Create(textureHandleStage3_, { 0, 0 });
	textureHandlePlayerArm_ = TextureManager::Load("blocktest.png");
	textureHandleNya_ = TextureManager::Load("nya.png");
	nyaSprite_ = Sprite::Create(textureHandleNya_, { 1100,100 });
	soundHandleTitle_ = audio_->LoadWave("neownch.mp3");
	soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true, 1);
	soundHandleNext_ = audio_->LoadWave("next.mp3");
	//k.o
	textureHandleKo_ = TextureManager::Load("ko.png");
	koSprite_ = Sprite::Create(textureHandleKo_, { 0, 0 });
	soundHandleKo_ = audio_->LoadWave("ko.mp3");
}

void GameScene::Finalize() {
	delete titleSprite_;
	delete stage1Sprite_;
	delete stage2Sprite_;
	delete stage3Sprite_;
	delete nyaSprite_;
	delete koSprite_;
}

void GameScene::Update() {
	
	switch (scene)
	{
	case 0://タイトル
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			audio_->StopWave(soundHandleLoop_);
			scene = 1;
		}
		break;

	case 1://ステージ１
		if (input_->TriggerKey(DIK_T)) {
			SpeedBuffer = playerArm_->GetSpeed();
			stop = true;
		}
		Stop();
		playerArm_->Update();
		enemy_->Update();
		viewProjection_.UpdateMatrix();

		if (input_->TriggerKey(DIK_K) || input_->TriggerKey(DIK_L)) {
			fontFlag_ = 1;
		}
		if (fontFlag_ == 1) {
			fontTimer_++;
		}
		if (fontTimer_ == 13) {
			fontFlag_ = 0;
			fontTimer_ = 0;
		}
		//k.o
		if (input_->TriggerKey(DIK_G)) {
			koFlag_ = 1;
			if (koFlag_ == 1) {
				audio_->PlayWave(soundHandleKo_, false, 3);
			}
		}

		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			koFlag_ = 0;
			scene = 2;
		}
		break;
	case 2://ステージ２
		if (input_->TriggerKey(DIK_T)) {
			SpeedBuffer = playerArm_->GetSpeed();
			stop = true;
		}
		Stop();
		playerArm_->Update();
		viewProjection_.UpdateMatrix();
		if (input_->TriggerKey(DIK_K) || input_->TriggerKey(DIK_L)) {
			fontFlag_ = 1;
		}
		if (fontFlag_ == 1) {
			fontTimer_++;
		}
		if (fontTimer_ == 13) {
			fontFlag_ = 0;
			fontTimer_ = 0;
		}
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			scene = 3;
		}
		break;
	case 3://ステージ３
		if (input_->TriggerKey(DIK_T)) {
			SpeedBuffer = playerArm_->GetSpeed();
			stop = true;
		}
		Stop();
		playerArm_->Update();
		viewProjection_.UpdateMatrix();
		if (input_->TriggerKey(DIK_K) || input_->TriggerKey(DIK_L)) {
			fontFlag_ = 1;
		}
		if (fontFlag_ == 1) {
			fontTimer_++;
		}
		if (fontTimer_ == 13) {
			fontFlag_ = 0;
			fontTimer_ = 0;
		}
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true, 1);
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
		titleSprite_->Draw();
		break;
	case 1:// ステージ1
		stage1Sprite_->Draw();
		if (fontTimer_ > 0) {
			nyaSprite_->Draw();
		}
		//k.o
		if (koFlag_ == 1) {
			koSprite_->Draw();
		}
		break;
	case 2:// ステージ2
		stage2Sprite_->Draw();
		if (fontTimer_ > 0) {
			nyaSprite_->Draw();
		}
		break;
	case 3:// ステージ3
		stage3Sprite_->Draw();
		if (fontTimer_ > 0) {
			nyaSprite_->Draw();
		}
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
		enemy_->Draw(viewProjection_);
		break;
	case 2:// ステージ2
		playerArm_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		break;
	case 3:// ステージ3
		playerArm_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
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