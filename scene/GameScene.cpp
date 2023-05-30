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
	playerArm_->SetEnemy(enemy_);
	enemy_->SetPlayerArm(playerArm_);
	modelPlayerArm_ = Model::CreateFromOBJ("cathand");
	textureHandlePlayerArm_ = TextureManager::Load("Nort8.png");
	modelPlayerFace_ = Model::CreateFromOBJ("catface");
	modelEnemy_ = Model::CreateFromOBJ("Ecathand");
	modelEnemyFace_ = Model::CreateFromOBJ("Ecatface");
	playerArm_->Initialize(modelPlayerArm_, modelPlayerFace_, textureHandlePlayerArm_);
	enemy_->Initialize(modelEnemy_, modelEnemyFace_, textureHandlePlayerArm_);
	viewProjection_.Initialize();

	textureHandleTitle_ = TextureManager::Load("neownch.png");
	titleSprite_ = Sprite::Create(textureHandleTitle_, { 0, 0 });
	textureHandleSetumei_ = TextureManager::Load("setumei.png");
	setumeiSprite_ = Sprite::Create(textureHandleSetumei_, { 0, 0 });
	textureHandleStage1_ = TextureManager::Load("stage1.png");
	stage1Sprite_ = Sprite::Create(textureHandleStage1_, { 0, 0 });
	textureHandleStage2_ = TextureManager::Load("stage2.png");
	stage2Sprite_ = Sprite::Create(textureHandleStage2_, { 0, 0 });
	textureHandleStage3_ = TextureManager::Load("stage3.png");
	stage3Sprite_ = Sprite::Create(textureHandleStage3_, { 0, 0 });
	textureHandlePlayerArm_ = TextureManager::Load("blocktest.png");
	textureHandleKo_ = TextureManager::Load("ko.png");
	koSprite_ = Sprite::Create(textureHandleKo_, { 0, 0 });
	textureHandleClear_ = TextureManager::Load("clear.png");
	clearSprite_ = Sprite::Create(textureHandleClear_, { 0, 0 });

	soundHandleTitle_ = audio_->LoadWave("neownch.mp3");
	soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true, 1);
	soundHandleNext_ = audio_->LoadWave("next.mp3");
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

	PADUpdate();
	switch (scene)
	{
	case 0://タイトル
		if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
			audio_->PlayWave(soundHandleNext_, false, 3);
			audio_->StopWave(soundHandleLoop_);
			playerKoFlag_ = 0;
			enemyKoFlag_ = 0;
			playerArm_->ReInitialize();
			enemy_->ReInitialize();
			scene = 1;
		}
		break;
	case 1://操作説明
		if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false)
		{
			audio_->PlayWave(soundHandleNext_, false, 3);
			scene = 2;
		}
		break;
	case 2://ステージ１

		playerArm_->Update();
		enemy_->Update();
		CheckCollision();
		viewProjection_.UpdateMatrix();

		if (playerArm_->GetHp() <= 0) {
			playerKoFlag_ = 1;
		}
		if (enemy_->GetHp() <= 0) {
			enemyKoFlag_ = 1;
		}
		if (playerKoFlag_ == 1) {
			if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				playerKoFlag_ = 0;
				playerArm_->ReInitialize();
				enemy_->ReInitialize();
				scene = 0;
			}
		}
		if (enemyKoFlag_ == 1) {
			if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				enemyKoFlag_ = 0;
				playerArm_->ReInitialize();
				enemy_->ReInitialize();
				scene = 3;
			}
		}
		break;
	case 3://ステージ２
		playerArm_->Update();
		enemy_->Update();
		CheckCollision();
		viewProjection_.UpdateMatrix();

		if (playerArm_->GetHp() <= 0) {
			playerKoFlag_ = 1;
		}
		if (enemy_->GetHp() <= 0) {
			enemyKoFlag_ = 1;
		}
		if (playerKoFlag_ == 1) {
			if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				playerKoFlag_ = 0;
				playerArm_->ReInitialize();
				enemy_->ReInitialize();
				scene = 0;
			}
		}
		if (enemyKoFlag_ == 1) {
			if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				enemyKoFlag_ = 0;
				playerArm_->ReInitialize();
				enemy_->ReInitialize();
				scene = 4;
			}
		}
		break;
	case 4://ステージ３
		playerArm_->Update();
		enemy_->Update();
		CheckCollision();
		viewProjection_.UpdateMatrix();

		if (playerArm_->GetHp() <= 0) {
			playerKoFlag_ = 1;
		}
		if (enemy_->GetHp() <= 0) {
			enemyKoFlag_ = 1;
		}
		if (playerKoFlag_ == 1) {
			if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				playerKoFlag_ = 0;
				playerArm_->ReInitialize();
				enemy_->ReInitialize();
				scene = 0;
			}
		}
		if (enemyKoFlag_ == 1) {
			if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				enemyKoFlag_ = 0;
				playerArm_->ReInitialize();
				enemy_->ReInitialize();
				scene = 5;
			}
		}
		break;
	case 5://クリア画面
		if (GamePAD_RIGHT_SHOULDER == true && prevGamePAD_RIGHT_SHOULDER == false) {
				audio_->PlayWave(soundHandleNext_, false, 3);
				soundHandleLoop_ = audio_->PlayWave(soundHandleTitle_, true, 1);
				scene = 0;
		}
		break;
	}
	///1フレーム前のPADの状態を保存
	prevGamePAD_B = GamePAD_B;
	prevGamePAD_Y = GamePAD_Y;
	prevGamePAD_RIGHT_SHOULDER = GamePAD_RIGHT_SHOULDER;
}
void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//描画前処理
	Sprite::PreDraw(commandList);
	switch (scene)
	{
	PADUpdate();
	case 0://タイトル
		titleSprite_->Draw();
		break;
	case 1://操作説明
		setumeiSprite_->Draw();
		break;
	case 2://ステージ1
		stage1Sprite_->Draw();
		if (playerKoFlag_ == 1) {
			koSprite_->Draw();
		}
		if (enemyKoFlag_ == 1) {
			koSprite_->Draw();
		}
		
		break;
	case 3:// ステージ2
		stage2Sprite_->Draw();
		if (playerKoFlag_ == 1) {
			koSprite_->Draw();
		}
		if (enemyKoFlag_ == 1) {
			koSprite_->Draw();
		}

		break;
	case 4://ステージ3
		stage3Sprite_->Draw();
		if (playerKoFlag_ == 1) {
			koSprite_->Draw();
		}
		if (enemyKoFlag_ == 1) {
			koSprite_->Draw();
		}
		break;
	case 5://クリア画面
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
	case 0://タイトル
		break;
	case 1://操作説明
		break;
	case 2://ステージ1
		playerArm_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		break;
	case 3://ステージ2
		playerArm_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		break;
	case 4://ステージ3
		playerArm_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		break;
	case 5://クリア画面
		break;
	}
	Model::PostDraw();
}

void GameScene::PADUpdate()
{
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			GamePAD_B = true;
		}
		else {
			GamePAD_B = false;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			GamePAD_Y = true;
		}
		else {
			GamePAD_Y = false;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			GamePAD_RIGHT_SHOULDER = true;
		}
		else {
			GamePAD_RIGHT_SHOULDER = false;
		}
	}
}

void GameScene::CheckCollision()
{

	CollisionPlayerAttackToEnemy();
	CollisionEnemyAttackToPlayer();
	///プレイヤーから敵への攻撃
	{

		//ヒット時
		if (PlayerAttackToEnemy == 1) {

			//ブロックされた
			if (enemy_->GetIsBlock() == true) {
				playerArm_->GetBlock();
			}

			//弱攻撃を敵にあてた
			if (playerArm_->GetIsBlock() == false && playerArm_->GetIsWeak() == true && playerArm_->GetIsHeavy() == false && playerArm_->GetIsStun() == false) {
				enemy_->GetWeak();
			}

			//強攻撃を当てた
			if (playerArm_->GetIsBlock() == false && playerArm_->GetIsWeak() == false && playerArm_->GetIsHeavy() == true && playerArm_->GetIsStun() == false) {
				enemy_->GetHeavy();
			}

			//スタン攻撃を当てた
			if (playerArm_->GetIsBlock() == false && playerArm_->GetIsWeak() == false && playerArm_->GetIsHeavy() == false && playerArm_->GetIsStun() == true) {
				enemy_->GetStun();
			}

			PlayerAttackToEnemy = 2;
		}

		//ヒット待機移行待ち
		if (PlayerAttackToEnemy == 2) {
			if (playerArm_->GetIsMovement() == true) {
				PlayerAttackToEnemy = 0;
			}
		}
	}

	///敵からプレイヤーへの攻撃
	{
		//ヒット時
		if (EnemyAttackToPlayer == 1) {

			//ブロックされた
			if (playerArm_->GetIsBlock() == true) {
				enemy_->GetBlock();
			}

			//弱攻撃を当てた
			if (enemy_->GetIsBlock() == false && enemy_->GetIsWeak() == true && enemy_->GetIsHeavy() == false && enemy_->GetIsStun() == false) {
				playerArm_->GetWeak();
			}

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
				if (enemy_->GetInvincibleTime_() > 0) {

				}
				else if (enemy_->GetInvincibleTime_() <= 0) {
					if (PlayerAttackToEnemy == 0) {
						enemy_->SetInvincibleTime_(invincibleSecond_ * 60);
						PlayerAttackToEnemy = 1;
					}
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
				if (playerArm_->GetInvincibleTime_() > 0) {

				}

				if (playerArm_->GetInvincibleTime_() == 0) {
					if (EnemyAttackToPlayer == 0) {
						playerArm_->SetInvincibleTime_(invincibleSecond_ * 60);
						EnemyAttackToPlayer = 1;
					}
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