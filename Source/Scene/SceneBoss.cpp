#include "SceneBoss.h"
#include <System/Graphics.h>
#include <CameraController.h>
#include <Camera.h>
#include <System/Input.h>
#include <ctime>
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneMiniGameShooting.h"
#include "SceneMiniGame2048.h"

void SceneBoss::Initialize()
{
	//random準備
	srand((unsigned int)time(nullptr));

	//ステージ初期設定
	stage = std::make_unique<Stage>(1);
	stage->SetPosition(DirectX::XMFLOAT3(10, -5, 10));

	//プレイヤー初期設定
	player = std::make_unique<Player>(1);
	player->SetPosition({ -11.025f, 1.0f, -21.912f });
	player->SetScale({ 0.009f,0.009f,0.009f });
	emp.hp = 500;
	emp.special = 150;
	punch = SetPlayerPunch();
	kick = SetPlayerKick();

	//エネミー初期設定
	pre.hp = 500;
	pre.punch = 50;
	pre.kick = 100;
	boss = new EnemySlime();
	boss->SetPosition( {-41.302f,1.0f,79.016f});
	boss->SetScale( {0.12f, 0.12f,0.12f});

	////カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 7, 30),//視点
		DirectX::XMFLOAT3(0, 0, 0),//注視点
		DirectX::XMFLOAT3(0, 1, 0)//上方向
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//画面アスペクト比
		0.1f,//クリップ距離（近）
		1000.0f//クリップ距離（遠）
	);
	//カメラコントローラー初期化
	cameraController = std::make_unique<CameraController2048>();
	player->cameraController2048 = cameraController.get();

	//カメラコントローラー初期化
	//cameraController = std::make_unique<CameraController2048>();
	//Camera& camera = Camera::Instance();
	////cameraController->angle.y = DirectX::XMConvertToRadians(45.0f);
	//cameraController->angle.x = DirectX::XMConvertToRadians(78.0f);
	//cameraController->distance = 29.8f;
	////player->cameraController = cameraController;
	//DirectX::XMFLOAT3 target =stage->GetPosition();
	//cameraController->SetTarget(target);

	////カメラ初期設定
	//Graphics& graphics = Graphics::Instance();
	////Camera& camera = Camera::Instance();
	//camera.SetLookAt(
	//	DirectX::XMFLOAT3(0, 7, 30),//視点
	//	DirectX::XMFLOAT3(0, 0, 0),//注視点
	//	DirectX::XMFLOAT3(0, 1, 0)//上方向
	//);
	//camera.SetPerspectiveFov(
	//	DirectX::XMConvertToRadians(45),//視野角
	//	graphics.GetScreenWidth() / graphics.GetScreenHeight(),//画面アスペクト比
	//	0.1f,//クリップ距離（近）
	//	1000.0f//クリップ距離（遠）
	//);

	// スクリーンサイズ取得
	screenWidth = Graphics::Instance().GetScreenWidth();
	screenHeight = Graphics::Instance().GetScreenHeight();

	//sprite初期設定
	frames[0] = std::make_unique<Sprite>("Data/Sprite/page1.png");
	frames[1] = std::make_unique<Sprite>("Data/Sprite/page2.png");
	frames[2] = std::make_unique<Sprite>("Data/Sprite/page3.png");
	frames[3] = std::make_unique<Sprite>("Data/Sprite/page4.png");
	triangle_black = std::make_unique<Sprite>("Data/Sprite/BLACK_TRIANGLE.png");

	underHp = std::make_unique<Sprite>("Data/Sprite/HP下地.png");
	topHp = std::make_unique<Sprite>("Data/Sprite/HPバー.png");

	enemyPunch = std::make_unique<Sprite>("Data/Sprite/敵パンチ.png");
	enemyKick = std::make_unique<Sprite>("Data/Sprite/敵キック.png");
	playerSpecial = std::make_unique<Sprite>("Data/Sprite/ラリアット.png");
	playerDance = std::make_unique<Sprite>("Data/Sprite/踊っている.png");
	if (punch == 1)
		playerPunch = std::make_unique<Sprite>("Data/Sprite/プレイヤーパンチI.png");
	else if(punch == 2)
		playerPunch = std::make_unique<Sprite>("Data/Sprite/プレイヤーパンチII.png");
	else if(punch == 3)
		playerPunch = std::make_unique<Sprite>("Data/Sprite/プレイヤーパンチIII.png");
	if (kick == 1)
		playerKick = std::make_unique<Sprite>("Data/Sprite/プレイヤーキックI.png");
	else if (kick == 2)
		playerKick = std::make_unique<Sprite>("Data/Sprite/プレイヤーキックII.png");
	else if (kick == 3)
		playerKick = std::make_unique<Sprite>("Data/Sprite/プレイヤーキックIII.png");

	//変数宣言
	state = NONE;
	slideSize = screenWidth / 3;
	upSlideSize = 0.0f;
	php = pre.hp;
	ehp = emp.hp;
}

void SceneBoss::Finalize()
{
	Input::Instance().GetMouse().Unlock();
}

void SceneBoss::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	stage->Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	boss->Update(elapsedTime);
	Motion();

	Camera::Instance().SetLookAt(
		EYE,
		TARGET,
		DirectX::XMFLOAT3(0, 1, 0));

	player->SetPosition({ postionpl });
	player->SetScale({ scalepl });

	boss->SetAngle(angleboss);


	// ルーレット開始、resultに結果をintで返す
	if (isRoulette)
	{
		resultHitCount = BossRoulette(elapsedTime, commands);
	}


	// GameClearかどうか
	if (pre.hp <= 0)
	{
		winTimer -= elapsedTime;
		//player->SetBossDown(true);
		if(winTimer<=0.0f)
		gameClear = true;
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneBoss));
	}
	else if (emp.hp <= 0)
	{
		loseTimer -= elapsedTime;
		//player->SetPlayerDown(true);
		if (loseTimer <= 0.0f)
		{
			gameClear = false;
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneBoss));
		}
	}

	// レベルアップ
	levelUp(elapsedTime);

	// spacekey処理
	cooltime += elapsedTime;

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_START && cooltime > 1.0f)
	{
		if (state == START)
		{
			isRoulette = true;
			isLevelUp = false;
			state = STOP;
		}
		else if (state == STOP)
		{
			isRouletteStop = true;
		}
		else if (state == NONE)
		{
			attack = true;;
			state = START;
			rightDown = false;

			punchPlayer = false;
			kickPlayer = false;
			specialPlayer = false;
			dancePlayer = false;
			punchEnemy = false;
			kickEnemy = false;
		}
		cooltime = 0.0f;
	}

	// debug
	{
		// 操作
		/*if (gamePad.GetButtonDown() & GamePad::BTN_START)
		{
			isRouletteStop = true;
		}
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			isRoulette = true;
		}*/

		// マウスカーソル
		//if (!player->GetPrev())
		//{
		//	// マウス位置の取得とロック
		//	Input::Instance().GetMouse().Lock();
		//	cameraController->MouseCamera(elapsedTime);
		//}
		//else
		//{
		//	Input::Instance().GetMouse().Unlock();
		//}
	}

	/*Camera::Instance().SetLookAt(
		cameraController->eye,
		cameraController->target,*/
		/*DirectX::XMFLOAT3(0, 1, 0));*/

}

void SceneBoss::Render()
{
	// 描画準備
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		//ステージ描画
		stage->Render(rc, modelRenderer);

		player->Render(rc, modelRenderer);

		player->RenderDebugPrimitive(rc, shapeRenderer);

		boss->Render(rc, modelRenderer);
		boss->UpdateTransform();
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		//player->RenderDebugPrimitive(rc, shapeRenderer);

		player->DrawDebugGUI();
		boss->DrawDebugGUI();
	}

	// 2Dスプライト描画
	{
		// サイズをまとめる
		DirectX::XMINT3 size = { screenWidth / 3 * 2, screenHeight / 3, 0 };

		//HP 
		// president
		underHp->Render(rc,
			screenWidth / 1.5f - 10.0f, 10.0f, 0.0f,
			screenWidth / 5 + 30.0f, screenHeight / 15, 0,
			1, 1, 1, 1);

		topHp->Render(rc,
			screenWidth / 1.5f, 16.0f, 0.0f,
			screenWidth / 5 * pre.hp / php, screenHeight / 20, 0,
			1, 1, 1, 1);

		// employee
		underHp->Render(rc,
			screenWidth / 6 - 10.0f, screenHeight / 10, 0,
			screenWidth / 5 + 30.0f, screenHeight / 15, 0,
			1, 1, 1, 1);

		topHp->Render(rc,
			screenWidth / 6, screenHeight / 10 + 7.5f, 0,
			screenWidth / 5 * emp.hp / ehp, screenHeight / 20, 0,
			1, 1, 1, 1);

		// ルーレット
		if (isRoulette)
		{
			triangle_black->Render(rc,
				size.x - screenWidth / 12, size.y + screenHeight / 11.8f * rouletteIndex, size.z,
				75, 70, 0,
				1, 1, 1, 1);

			frames[nowLevel]->Render(rc,
				size.x, size.y, size.z,
				screenWidth / 5, screenHeight / 5 * 3, 0,
				1, 1, 1, 1);
		}

		// レベルアップ
		if (isLevelUp)
		{
			int nextLevel = nowLevel + 1;

			frames[nowLevel]->Render(rc,
				size.x - upSlideSize, size.y, size.z,
				screenWidth / 5, screenHeight / 5 * 3, 0,
				1, 1, 1, 1);

			frames[nextLevel]->Render(rc,
				size.x - upSlideSize + slideSize, size.y, size.z,
				screenWidth / 5, screenHeight / 5 * 3, 0,
				1, 1, 1, 1);
		}

		// 技名表示
		if (rightDown)
		{
			if (punchPlayer)
			{
				playerPunch->Render(rc,
					screenWidth - screenWidth / 5, screenHeight - screenHeight / 12, size.z,
					screenWidth / 6, screenHeight / 15, 0,
					1, 1, 1, 1);
			}
			else if (kickPlayer)
			{
				playerKick->Render(rc,
					screenWidth - screenWidth / 5, screenHeight - screenHeight / 12, size.z,
					screenWidth / 6, screenHeight / 15, 0,
					1, 1, 1, 1);
			}
			else if (specialPlayer)
			{
				playerSpecial->Render(rc,
					screenWidth - screenWidth / 5, screenHeight - screenHeight / 12, size.z,
					screenWidth / 6, screenHeight / 15, 0,
					1, 1, 1, 1);
			}
			else if (dancePlayer)
			{
				playerDance->Render(rc,
					screenWidth - screenWidth / 5, screenHeight - screenHeight / 12, size.z,
					screenWidth / 6, screenHeight / 15, 0,
					1, 1, 1, 1);
			}
			else if (punchEnemy)
			{
				enemyPunch->Render(rc,
					screenWidth - screenWidth / 5, screenHeight - screenHeight / 12, size.z,
					screenWidth / 6, screenHeight / 15, 0,
					1, 1, 1, 1);
			}
			else if (kickEnemy)
			{
				enemyKick->Render(rc,
					screenWidth - screenWidth / 5, screenHeight - screenHeight / 12, size.z,
					screenWidth / 6, screenHeight / 15, 0,
					1, 1, 1, 1);
			}
		}
	}
}

void SceneBoss::DrawGUI()
{
	/*{
		ImGui::Begin("Roulette Debug");

		ImGui::Text("rouletteIndex : %d", rouletteIndex);

		ImGui::Text("roulette : %d", roulette);

		ImGui::Text("resultIndex : %d", resultIndex);

		ImGui::Text("rouletteTimer : %f", rouletteTimer);

		ImGui::Text("rouletteInterval : %f", rouletteInterval);

		ImGui::Text("isRouletteStop : %s",
			isRouletteStop ? "true" : "false");

		ImGui::Text("isRoulette : %s",
			isRoulette ? "true" : "false");

		ImGui::Text("isLevelUp : %s",
			isLevelUp ? "true" : "false");

		ImGui::DragInt("slide", &slide, 1);

		ImGui::Text("state : %d", state);

		ImGui::Text("cooltime : %d", cooltime);

		ImGui::End();
	}

	{
		ImGui::Begin("Character Status");

		ImGui::Text("Employee");
		ImGui::Separator();

		ImGui::DragInt("Emp HP", &emp.hp, 1);

		ImGui::DragInt("Emp Punch", &emp.punch, 1);

		ImGui::DragInt("Emp Kick", &emp.kick, 1);

		ImGui::DragInt("Emp Special", &emp.special, 1);

		ImGui::Spacing();

		ImGui::Text("President");
		ImGui::Separator();

		ImGui::DragInt("Pre HP", &pre.hp, 1);

		ImGui::DragInt("Pre Punch", &pre.punch, 1);

		ImGui::DragInt("Pre Kick", &pre.kick, 1);

		ImGui::End();
	}

	{
		ImGui::Begin("Debug Action");

		ImGui::Text("Player");
		ImGui::Checkbox("Punch##Player", &punchPlayer);
		ImGui::Checkbox("Kick##Player", &kickPlayer);
		ImGui::Checkbox("Special##Player", &specialPlayer);
		ImGui::Checkbox("Dance##Player", &dancePlayer);

		ImGui::Separator();

		ImGui::Text("Enemy");
		ImGui::Checkbox("Punch##Enemy", &punchEnemy);
		ImGui::Checkbox("Kick##Enemy", &kickEnemy);

		ImGui::Separator();

		ImGui::DragFloat3("Eye", &EYE.x);
		ImGui::DragFloat3("Target", &TARGET.x);

		ImGui::End();

		ImGui::Separator();


		ImGui::Begin("Player Transform");

		ImGui::DragFloat3("Position", &postionpl.x, 0.1f);
		ImGui::DragFloat3("Scale", &scalepl.x, 0.01f);

		ImGui::DragFloat3("Angle", &angleboss.x, 1.0f);

		DirectX::XMFLOAT3 angleRad =
		{
			DirectX::XMConvertToRadians(angleboss.x),
			DirectX::XMConvertToRadians(angleboss.y),
			DirectX::XMConvertToRadians(angleboss.z)
		};

		ImGui::End();
	}*/
}

int SceneBoss::BossRoulette(float elapsedTime, int maxCount)
{
	//rouletteTimer += elapsedTime;

	if (!isStopping)
	{
		rouletteTimer += elapsedTime;

		if (rouletteTimer >= rouletteInterval)
		{
			rouletteTimer = 0.0f;

			rouletteIndex++;

			if (rouletteIndex >= maxCount)
			{
				rouletteIndex = 0;
			}
		}
	}

	// 停止開始
	if (isRouletteStop)
	{
		// 徐々に減速
		rouletteInterval += elapsedTime * 0.25f;

		// 最初の1回だけ結果決定
		if (resultIndex == -1)
		{
			//resultIndex = rand() % maxCount;
			resultIndex = 4;
			roulette = resultIndex;
		}

		// 結果位置に来た瞬間
		if (!isStopping &&
			rouletteIndex == resultIndex &&
			rouletteInterval >= 0.35f)
		{
			isStopping = true;
			stopTimer = 0.0f;
		}

		// 停止待機
		if (isStopping)
		{
			stopTimer += elapsedTime;

			if (stopTimer >= 0.5f)
			{
				rouletteIndex = resultIndex;
				roulette = resultIndex;

				rouletteResult(resultIndex);

				rightDown = true;

				isRoulette = false;
				isRouletteStop = false;
				isStopping = false;

				resultIndex = -1;
				rouletteInterval = 0.1f;
				stopTimer = 0.0f;
			}
		}
	}

	return rouletteIndex;
}

void SceneBoss::rouletteResult(int result)
{
	if (nowLevel == 0)
	{
		switch (result)
		{
		case 0:
			// 敵パンチ
			if (attack)
			{
				emp.hp -= pre.punch;
				attack = false;;
			}
			state = NONE;
			punchEnemy = true;
			break;

		case 1:
			// 次へ
			slide = nowLevel + 1;
			isLevelUp = true;
			isRoulette = false;
			break;

		case 2:
			// 踊り
			state = NONE;
			dancePlayer = true;
			break;

		case 3:
			// 次へ
			slide = nowLevel + 1;
			isLevelUp = true;
			isRoulette = false;
			break;

		case 4:
			// 敵パンチ
			if (attack)
			{
				emp.hp -= pre.punch;
				attack = false;
			}
			state = NONE;
			punchEnemy = true;
			break;

		case 5:
			// プレイヤーパンチ
			if (attack)
			{
				pre.hp -= emp.punch;
				attack = false;
			}
			state = NONE;
			punchPlayer = true;
			break;

		case 6:
			// 踊り
			state = NONE;
			dancePlayer = true;
			break;
		}
	}
	else if (nowLevel == 1)
	{
		switch (result)
		{
		case 0:
			// 次へ
			slide = nowLevel + 1;
			isLevelUp = true;
			isRoulette = false;
			break;

		case 1:
			// 踊り
			state = NONE;
			dancePlayer = true;
			break;

		case 2:
			// プレイヤ―キック
			if (attack)
			{
				pre.hp -= emp.kick;
				attack = false;
			}
			state = NONE;
			kickPlayer = true;
			break;

		case 3:
			// プレイヤ―キック
			if (attack)
			{
				pre.hp -= emp.kick;
				attack = false;
			}
			state = NONE;
			kickPlayer = true;
			break;

		case 4:
			// 敵キック
			if (attack)
			{
				emp.hp -= pre.kick;
				attack = false;
			}
			state = NONE;
			kickEnemy = true;
			break;

		case 5:
			// 次へ
			slide = nowLevel + 1;
			isLevelUp = true;
			isRoulette = false;
			break;

		case 6:
			// 敵キック
			if (attack)
			{
				emp.hp -= pre.kick;
				attack = false;
			}
			state = NONE;
			kickEnemy = true;
			break;
		}
	}
	else if (nowLevel == 2)
	{
		switch (result)
		{
		case 0:
			// プレイヤ―キック
			if (attack)
			{
				pre.hp -= emp.kick;
				attack = false;
			}
			state = NONE;
			kickPlayer = true;
			break;

		case 1:
			// 敵パンチ
			if (attack)
			{
				emp.hp -= pre.punch;
				attack = false;
			}
			state = NONE;
			punchEnemy = true;
			break;

		case 2:
			// プレイヤーパンチ
			if (attack)
			{
				pre.hp -= emp.punch;
				attack = false;
			}
			state = NONE;
			punchPlayer = true;
			break;

		case 3:
			// 次へ
			slide = nowLevel + 1;
			isLevelUp = true;
			isRoulette = false;
			break;

		case 4:
			// 敵キック
			if (attack)
			{
				emp.hp -= pre.kick;
				attack = false;
			}
			state = NONE;
			kickEnemy = true;
			break;

		case 5:
			// プレイヤ―キック
			if (attack)
			{
				pre.hp -= emp.kick;
				attack = false;
			}
			state = NONE;
			kickPlayer = true;
			break;

		case 6:
			// 敵パンチ
			if (attack)
			{
				emp.hp -= pre.punch;
				attack = false;
			}
			state = NONE;
			punchEnemy = true;
			break;
		}
	}
	else if (nowLevel == 3)
	{
		switch (result)
		{
		case 0:
			// 必殺
			if (attack)
			{
				pre.hp -= emp.special;
				attack = false;
			}
			state = NONE;
			specialPlayer = true;
			break;

		case 1:
			// 踊り
			state = NONE;
			dancePlayer = true;
			break;

		case 2:
			// 必殺
			if (attack)
			{
				pre.hp -= emp.special;
				attack = false;
			}
			state = NONE;
			specialPlayer = true;
			break;

		case 3:
			// 次へ
			slide = nowLevel + 1;
			isLevelUp = true;
			isRoulette = false;
			break;

		case 4:
			// 踊り
			state = NONE;
			dancePlayer = true;
			break;

		case 5:
			// 必殺
			if (attack)
			{
				pre.hp -= emp.special;
				attack = false;
			}
			state = NONE;
			specialPlayer = true;
			break;

		case 6:
			// 踊り
			state = NONE;
			dancePlayer = true;
			break;
		}
	}
	//state = NONE;
}

void SceneBoss::levelUp(float elapsedTime)
{
	// オーバーロードを防ぎ、最初にもどす
	if (nowLevel == commands - 1)
	{
		nowLevel = 0;
	}

	// レベルアップ終了処理
	if (slideSize < upSlideSize)
	{
		isRoulette = true;
		isLevelUp = false;
		upSlideSize = 0.0f;
		nowLevel += 1;
		state = START;
	}
	// レベルアップ開始、slideSizeまで増やしていく
	else if (isLevelUp && slideSize > upSlideSize)
	{
		upSlideSize += elapsedTime * 500;
	}

}

int SceneBoss::SetPlayerPunch()
{
	SceneGameShooting scene;
	int level = scene.GetCount();
	if (level == 1)
	{
		emp.punch = 15;
	}
	else if (level == 2)
	{
		emp.punch == 30;
	}
	else if (level == 3)
	{
		emp.punch == 45;
	}
	return level;
}

int SceneBoss::SetPlayerKick()
{
	SceneMiniGame2048 scene;
	int level = scene.GetScore();
	if (level == 1)
	{
		emp.kick = 30;
	}
	else if (level == 2)
	{
		emp.kick = 50;
	}
	else if (level == 3)
	{
		emp.kick = 70;
	}
	return level;
}

void SceneBoss::Motion()
{
	//パンチ　player
	if (punchPlayer == true)
	{
		player->SetPunch(true);
	}
	else
		player->SetPunch(false);

	if (kickPlayer == true)
	{
		player->SetKick(true);
	}
	else
		player->SetKick(false);

	if (specialPlayer == true)
	{
		player->SetLariat(true);
	}
	else
		player->SetLariat(false);

	if (dancePlayer == true)
	{
		player->SetPlayerDance(true);
	}
	else
		player->SetPlayerDance(false);
}