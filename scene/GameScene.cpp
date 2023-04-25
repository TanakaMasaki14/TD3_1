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
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	playerArm_ = new PlayerArm();
	enemy_ = new Enemy();
	modelPlayerArm_ = Model::CreateFromOBJ("cathand");
	textureHandlePlayerArm_ = TextureManager::Load("Nort8.png");
	modelPlayerFace_ = Model::CreateFromOBJ("catface");
	modelEnemy_ = Model::CreateFromOBJ("Ecathand");
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
	//textureHandleNya_ = TextureManager::Load("nya.png");
	//nyaSprite_ = Sprite::Create(textureHandleNya_, { 1100,100 });
	//textureHandleBatin_ = TextureManager::Load("batin.png");
	//batinSprite_ = Sprite::Create(textureHandleBatin_, { 400,400 });
	//textureHandleDon_ = TextureManager::Load("don.png");
	//donSprite_ = Sprite::Create(textureHandleDon_, { 800,400 });
	textureHandleKo_ = TextureManager::Load("ko.png");
	koSprite_ = Sprite::Create(textureHandleKo_, { 0, 0 });
	textureHandleClear_ = TextureManager::Load("clear.png");
	clearSprite_ = Sprite::Create(textureHandleClear_, { 0, 0 });

	soundHandleTitle_ = audio_->LoadWave("neownch.mp3");
	soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true, 1);
	soundHandleNext_ = audio_->LoadWave("next.mp3");
	soundHandleKo_ = audio_->LoadWave("ko.mp3");
	//soundHandleClear_ = audio_->LoadWave("clear.mp3");
}

void GameScene::Finalize() {
	delete titleSprite_;
	delete stage1Sprite_;
	delete stage2Sprite_;
	delete stage3Sprite_;
	delete koSprite_;
	delete clearSprite_;
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
		CheckCollision();
		viewProjection_.UpdateMatrix();
		if (input_->TriggerKey(DIK_K) || input_->TriggerKey(DIK_L)) {
			fontFlag_ = 1;
		}
		if (fontFlag_ == 1) {
			fontTimer_++;
		}

		if (input_->PushKey(DIK_K) || input_->TriggerKey(DIK_L)) {
			if (fontTimer_ <= 13) {
				fontTimer_++;
			}
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
		if (koFlag_ == 1) {
			if (input_->TriggerKey(DIK_SPACE)) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				koFlag_ = 0;
				scene = 2;
			}
		}
		break;
	case 2://ステージ２
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
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			scene = 3;
		}
		//k.o
		if (input_->TriggerKey(DIK_G)) {
			koFlag_ = 1;
			if (koFlag_ == 1) {
				audio_->PlayWave(soundHandleKo_, false, 3);
			}
		}
		if (input_->TriggerKey(DIK_G)) {
			koFlag_ = 1;
			audio_->PlayWave(soundHandleKo_, false, 3);
		}
		if (koFlag_ == 1) {
			if (input_->TriggerKey(DIK_SPACE)) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				koFlag_ = 0;
				scene = 3;
			}
		}
		break;
	case 3://ステージ３
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
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			scene = 4;
		}
		//k.o
		if (input_->TriggerKey(DIK_G)) {
			koFlag_ = 1;
			if (koFlag_ == 1) {
				audio_->PlayWave(soundHandleKo_, false, 3);
			}
		}
		if (input_->TriggerKey(DIK_G)) {
			koFlag_ = 1;
			audio_->PlayWave(soundHandleKo_, false, 3);
		}
		if (koFlag_ == 1) {
			if (input_->TriggerKey(DIK_SPACE)) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				koFlag_ = 0;
				scene = 4;
			}
		}
		break;
	case 4://クリア画面
		//audio_->PlayWave(soundHandleClear_, false, 1);
		if (input_->TriggerKey(DIK_SPACE)) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true, 1);
			scene = 0;
		}
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
		//if (fontTimer_ > 0) {
		//	nyaSprite_->Draw();
		//}
		////k.o
		//if (fontTimer_ >= 1) {
		//	nyaSprite_->Draw();
		//}
		//batinSprite_->Draw();
		//donSprite_->Draw();
		if (koFlag_ == 1) {
			koSprite_->Draw();
		}
		break;
	case 2:// ステージ2
		stage2Sprite_->Draw();
		if (koFlag_ == 1) {
			koSprite_->Draw();
		}
		break;
	case 3:// ステージ3
		stage3Sprite_->Draw();
		if (koFlag_ == 1) {
			koSprite_->Draw();
		}
		break;
	case 4:// クリア画面
		clearSprite_->Draw();
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

void GameScene::CheckCollision()
{

	CollisionPlayerAttackToEnemy();
	CollisionEnemyAttackToPlayer();
	///プレイヤーから敵への攻撃
	{

		//ヒット時
		if (PlayerAttackToEnemy == 1) {

			//
			//ブロックされた
			if (playerArm_->GetIsBlock() == true) {
				enemy_->GetBlock();
			}

			//弱攻撃を当てた
			if (enemy_->GetIsBlock() == false && enemy_->GetIsWeak() == true && enemy_->GetIsHeavy() == false && enemy_->GetIsStun() == false) {
				playerArm_->GetWeak();
			}

			//
			//強攻撃を当てた
			if (enemy_->GetIsBlock() == false && enemy_->GetIsWeak() == false && enemy_->GetIsHeavy() == true && enemy_->GetIsStun() == false) {
				playerArm_->GetHeavy();
			}

			//スタン攻撃を当てた
			if (enemy_->GetIsBlock() == false && enemy_->GetIsWeak() == false && enemy_->GetIsHeavy() == false && enemy_->GetIsStun() == true) {
				playerArm_->GetStun();
			}
			EnemyAttackToPlayer = 2;
		}

		//ヒット待機移行待ち
		if (EnemyAttackToPlayer == 2) {
			if (enemy_->GetFase() == 3) {
				EnemyAttackToPlayer = 0;
			}

		}
	}
}

void GameScene::CollisionPlayerAttackToEnemy()
{
	Vector3 PlayerAttackPos[10];
	Vector3 EnemyCollisionPos[7];

	{
		PlayerAttackPos[0] = playerArm_->GetWorldTransformPlayerAttackCollision0();
		PlayerAttackPos[1] = playerArm_->GetWorldTransformPlayerAttackCollision1();
		PlayerAttackPos[2] = playerArm_->GetWorldTransformPlayerAttackCollision2();
		PlayerAttackPos[3] = playerArm_->GetWorldTransformPlayerAttackCollision3();
		PlayerAttackPos[4] = playerArm_->GetWorldTransformPlayerAttackCollision4();
		PlayerAttackPos[5] = playerArm_->GetWorldTransformPlayerAttackCollision5();
		PlayerAttackPos[6] = playerArm_->GetWorldTransformPlayerAttackCollision6();
		PlayerAttackPos[7] = playerArm_->GetWorldTransformPlayerAttackCollision7();
		PlayerAttackPos[8] = playerArm_->GetWorldTransformPlayerAttackCollision8();
		PlayerAttackPos[9] = playerArm_->GetWorldTransformPlayerAttackCollision9();
	}

	{
		EnemyCollisionPos[0] = enemy_->GetWorldTransformEnemyCollision0();
		EnemyCollisionPos[1] = enemy_->GetWorldTransformEnemyCollision1();
		EnemyCollisionPos[2] = enemy_->GetWorldTransformEnemyCollision2();
		EnemyCollisionPos[3] = enemy_->GetWorldTransformEnemyCollision3();
		EnemyCollisionPos[4] = enemy_->GetWorldTransformEnemyCollision4();
		EnemyCollisionPos[5] = enemy_->GetWorldTransformEnemyCollision5();
		EnemyCollisionPos[6] = enemy_->GetWorldTransformEnemyCollision6();
	}

	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 7; k++) {
			if (
				(PlayerAttackPos[i].x - EnemyCollisionPos[k].x) * (PlayerAttackPos[i].x - EnemyCollisionPos[k].x) +
				(PlayerAttackPos[i].y - EnemyCollisionPos[k].y) * (PlayerAttackPos[i].y - EnemyCollisionPos[k].y) +
				(PlayerAttackPos[i].z - EnemyCollisionPos[k].z) * (PlayerAttackPos[i].z - EnemyCollisionPos[k].z)
				<= (enemy_->GetScaleEnemyCollision().x + playerArm_->GetScalePlayerAttackCollision().x) * (enemy_->GetScaleEnemyCollision().x + playerArm_->GetScalePlayerAttackCollision().x)
				) {
				if (PlayerAttackToEnemy == 0) {
					PlayerAttackToEnemy = 1;
				}

			}
		}
	}

}

void GameScene::CollisionEnemyAttackToPlayer()
{
	Vector3 EnemyAttackPos[10];
	Vector3 PlayerCollisionPos[7];

	{
		EnemyAttackPos[0] = enemy_->GetWorldTransformEnemyAttackCollision0();
		EnemyAttackPos[1] = enemy_->GetWorldTransformEnemyAttackCollision1();
		EnemyAttackPos[2] = enemy_->GetWorldTransformEnemyAttackCollision2();
		EnemyAttackPos[3] = enemy_->GetWorldTransformEnemyAttackCollision3();
		EnemyAttackPos[4] = enemy_->GetWorldTransformEnemyAttackCollision4();
		EnemyAttackPos[5] = enemy_->GetWorldTransformEnemyAttackCollision5();
		EnemyAttackPos[6] = enemy_->GetWorldTransformEnemyAttackCollision6();
		EnemyAttackPos[7] = enemy_->GetWorldTransformEnemyAttackCollision7();
		EnemyAttackPos[8] = enemy_->GetWorldTransformEnemyAttackCollision8();
		EnemyAttackPos[9] = enemy_->GetWorldTransformEnemyAttackCollision9();
	}

	{
		PlayerCollisionPos[0] = playerArm_->GetWorldTransformPlayerCollision0();
		PlayerCollisionPos[1] = playerArm_->GetWorldTransformPlayerCollision1();
		PlayerCollisionPos[2] = playerArm_->GetWorldTransformPlayerCollision2();
		PlayerCollisionPos[3] = playerArm_->GetWorldTransformPlayerCollision3();
		PlayerCollisionPos[4] = playerArm_->GetWorldTransformPlayerCollision4();
		PlayerCollisionPos[5] = playerArm_->GetWorldTransformPlayerCollision5();
		PlayerCollisionPos[6] = playerArm_->GetWorldTransformPlayerCollision6();
	}

	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 7; k++) {
			if (
				(EnemyAttackPos[i].x - PlayerCollisionPos[k].x) * (EnemyAttackPos[i].x - PlayerCollisionPos[k].x) +
				(EnemyAttackPos[i].y - PlayerCollisionPos[k].y) * (EnemyAttackPos[i].y - PlayerCollisionPos[k].y) +
				(EnemyAttackPos[i].z - PlayerCollisionPos[k].z) * (EnemyAttackPos[i].z - PlayerCollisionPos[k].z)
				<= (playerArm_->GetScalePlayerCollision().x + enemy_->GetScaleEnemyAttackCollision().x) * (playerArm_->GetScalePlayerCollision().x + enemy_->GetScaleEnemyAttackCollision().x)
				) {
				if (EnemyAttackToPlayer == 0) {
					EnemyAttackToPlayer = 1;
				}
			}
		}
	}

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