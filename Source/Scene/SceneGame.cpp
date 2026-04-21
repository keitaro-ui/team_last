#include "System/Graphics.h"
#include"System/Input.h"
#include "SceneGame.h"
#include "Camera.h"
#include "../Game/EnemyManager.h"
#include "../Game/EnemySlime.h"
#include "../Game/Board.h"
#include "time.h"
#include "random"
#include "algorithm"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
#include "SceneResult.h"

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	stage = std::make_unique<Stage>();
	stage->SetPosition(DirectX::XMFLOAT3(10, -5, 10));

	game_timer = 15;

	//プレイヤー初期化
	player = std::make_unique<Player>();

	//モデル読み込み
	sprite = std::make_unique<Sprite>("Data/Sprite/レティクル.png");
	sprite_number = std::make_unique<Sprite>("Data/Sprite/number.png");
	sprite_text = std::make_unique<Sprite>("Data/Sprite/残り時間.png");
	

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),//視点
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
	cameraController = std::make_unique<CameraController>();
	player->cameraController = cameraController.get();

	//エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
	//EnemySlime* target = new EnemySlime();
	//enemyManager.Register(target);

	//当たり判定
	{
		xDis = 900.0f;
		zDis = 600.0f;
		//debugDoorSize = 2.5f;
		blockSize = { 3.0f, 0.0f, 1.6f };

		//外枠
		physics.AddObb({ 0, 0, 0 }, { xDis, 0, zDis }, 0);

		//int map[5][6] =
		//{
		//	// 1列目
		//	{ 1, 1, 0, 1, 1, 1 },

		//	// 2列目
		//	{ 1, -1, 1, 0, 0, -1 },

		//	// 3列目
		//	{ 1, 1, 1, 0, 1, 1 },

		//	// 4列目
		//	{ -1, 0, -1, 1, 1, 0 },

		//	// 5列目
		//	{ 0, 1, 0, -1, 0, 1 }
		//};

		//for (int z = 0; z < 5; z++)
		//{
		//	for (int x = 0; x < 6; x++)
		//	{
		//		DirectX::XMFLOAT3 pos = { -22.5f + x * xDis, 0.0f, zDis * (2 - z) };

		//		int v = map[z][x];

		//		if (v == 0)
		//		{
		//			// 壁
		//			physics.AddObb(pos, blockSize, 0.0f);
		//		}
		//		else
		//		{
		//			// ドアの方向
		//			//DirectX::XMFLOAT3 dir = { 0, 0, (float)v };
		//			//physics.AddDoorObb(pos, blockSize, 0.0f, dir, debugDoorSize, 3.05f);
		//		}
		//	}
		//}

		//通路塞ぐ壁
		{
			/*physics.AddObb({ -22.5f + 3 * xDis, 0.0f, zDis * 1.5f }, blockSize, 0.0f);
			physics.AddObb({ -22.5f + 3 * xDis, 0.0f, zDis * 0.5f }, blockSize, 0.0f);

			physics.AddObb({ -22.5f + 2 * xDis, 0.0f, zDis * -0.5f }, blockSize, 0.0f);
			physics.AddObb({ 0.0f, 0.0f, 0.0f }, blockSize, 0.0f);*/
		}
	}

	//マウス位置の取得とロック
	Input::Instance().GetMouse().Lock();
}

// 終了化
void SceneGame::Finalize()
{
	Input::Instance().GetMouse().Unlock();

	//エネミー終了化
	EnemyManager::Instance().Clear();

	game_timer = 0;
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	player->Update(elapsedTime);

	//ステージ更新処理
	stage->Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);
	player->SetPosition(physics.CircleVsStage(player->GetPosition(), player->GethitRadius()));

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//シーン遷移
	GamePad& gamePad = Input::Instance().GetGamePad();

	const GamePadButton anyButton =
		GamePad::BTN_B;

	//game_timer++;


	//game_timer -= elapsedTime;
	if (game_timer < 0)
	//if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
	}

}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// 描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	//描画処理
	//RenderContext rc;
	rc.renderState = graphics.GetRenderState();
	rc.lightDirection = { 0.0f , -1.0f , 0.0f };	//ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		//ステージ描画
		stage->Render(rc, modelRenderer);

		player->Render(rc, modelRenderer);

		EnemyManager::Instance().Render(rc, modelRenderer);

		player->RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		//player->RenderDebugPrimitive(rc, shapeRenderer);

		//エネミーデバッグプリミティブ描画
		//EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);

		//当たり判定デバッグプリミティブ描画
		physics.RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
	{
		sprite->Render(rc,
			610, 335, 0, 64.0f, 64.0f,
			0,
			1, 1, 1, 1);

		sprite_text->Render(rc,
			950, 0, 0, 120, 80, 0, 1, 1, 1, 1);

		int n[2]{};
		//n[0] = static_cast<int>(game_timer) / 100 % 10;
		n[0] = static_cast<int>(game_timer) / 10 % 10;
		n[1] = static_cast<int>(game_timer) % 10;

		for (int i = 0; i < 2; i++)
		{
			sprite_number->Render(rc,
				32 * 2 * i + 1100, 00,
				0,
				32 * 2, 32 * 2,
				372.5 * n[i], 0,
				372.5, 514,
				0,
				1, 1, 1, 1);
		}
	}
}

// GUI描画
void SceneGame::DrawGUI()
{
	//プレーヤーデバッグ処理
	//player->DrawDebugGUI();

	ImGui::Begin("Debug");

	ImGui::DragFloat("xDis", &xDis, 0.1f);
	ImGui::DragFloat("zDis", &zDis, 0.1f);

	ImGui::End();
}