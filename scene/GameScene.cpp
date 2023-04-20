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

	playerArm_->SetEnemy(enemy_);
	enemy_->SetPlayerArm(playerArm_);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	modelPlayerArm_ = Model::Create();
	textureHandlePlayerArm_ = TextureManager::Load("Nort8.png");

	modelPlayerFace_ = Model::Create();

	modelEnemy_ = Model::Create();

	modelEnemyFace_ = Model::Create();


	playerArm_->Initialize(modelPlayerArm_, modelPlayerFace_, textureHandlePlayerArm_);

	enemy_->Initialize(modelEnemy_, modelEnemyFace_, textureHandlePlayerArm_);

	viewProjection_.Initialize();
}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_SPACE)) {
		SpeedBuffer = playerArm_->GetSpeed();
		stop = true;
	}
	Stop();

	debugText_->SetPos(0, 120);
	debugText_->Printf("%d", PlayerAttackToEnemy);



	playerArm_->Update();
	enemy_->Update();
	CheckCollision();
	viewProjection_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	playerArm_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
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