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
#include "../Scene/SceneBoss.h"

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	stage = std::make_unique<Stage>();
	stage->SetPosition(DirectX::XMFLOAT3(10, -5, 10));
	goalPos = { -22.0f, 1.0f, -70.5f };

	game_timer = 15;

	//プレイヤー初期化
	player = std::make_unique<Player>();
	player->SetPosition({ -22.8f, 1.0f, -26.0f });

	//Sprite読み込み
	sprite = std::make_unique<Sprite>("Data/Sprite/レティクル.png");
	sprite_number = std::make_unique<Sprite>("Data/Sprite/number.png");
	sprite_text = std::make_unique<Sprite>("Data/Sprite/残り時間.png");
	spriteUI = std::make_unique<Sprite>("Data/Sprite/ok.png");
	
	//3DModel読み込み


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
		//debug
		xDis = 50.0f;
		zDis = 50.0f;
		obbPos = { 0,0,0 };
		//physics.AddObb(obbPos, { xDis, 0, zDis }, 0);
		
		//外枠
		physics.AddObb({-8.9f, 0.0f, -34.5f}, {14.4f, 0.0f, 43.1f}, 0);
		physics.AddObb({-25.9f, 0.0f, 0.0f}, {6.2f, 0.0f, 19.5f}, 0);
		//手前、コピー機
		physics.AddObb({-6.0f, 0.0f, -3.5f}, {6.4f, 0.0f, 14.7f}, 0);
		physics.AddObb({-6.2f, 0.0f, -18.8f}, {3.1f, 0, 2.6f}, 0);
		//中
		physics.AddObb({-4.1f, 0.0f, -34.5f}, {11.6f, 0, 5.6f}, 0);
		//奥
		physics.AddObb({ -7.0f, 0.0f, -56.3f }, { 6.8f, 0.0f, 8.1f }, 0);
	}

	//マウス位置の取得とロック
	//Input::Instance().GetMouse().Lock();
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
	player->SetPosition(physics.CircleVsStage(player->GetPosition(), player->GethitRadius())); //当たり判定一旦消すならコメントして

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//シーン遷移
	GamePad& gamePad = Input::Instance().GetGamePad();

	const GamePadButton anyButton =
		GamePad::BTN_B;

	//game_timer++;
	//game_timer -= elapsedTime;

	if(playBoss())
	//if (game_timer < 0)
	//if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneBoss));
	}


	if (player.get()->GetPrev() == false)
	{
		//マウス位置の取得とロック
		Input::Instance().GetMouse().Lock();
		cameraController->MouseCamera(elapsedTime);
	}
	else
	{
		Input::Instance().GetMouse().Unlock();
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
		//RenderUI(rc);

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
	player->DrawDebugGUI();

	ImGui::Begin("Debug");

	// 変更検知フラグ
	bool changed = false;

	// DragFloatは「値が変わったらtrue返す」
	changed |= ImGui::DragFloat("xDis", &xDis, 0.1f);
	changed |= ImGui::DragFloat("zDis", &zDis, 0.1f);
	changed |= ImGui::DragFloat("obbPosX", &obbPos.x, 0.1f);
	changed |= ImGui::DragFloat("obbPosY", &obbPos.y, 0.1f);
	changed |= ImGui::DragFloat("obbPosZ", &obbPos.z, 0.1f);

	ImGui::End();

	// 値が変わったときだけ再生成
	if (changed)
	{
		physics.Clear();

		physics.AddObb(obbPos, { xDis, 0, zDis }, 0);
	}
}

void SceneGame::RenderUI(RenderContext rc)
{
	// スクリーンサイズ取得
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	Camera& camera = Camera::Instance();
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 plyPos = player.get()->GetPosition();
	plyPos.y += 2.0f;

	DirectX::XMVECTOR worldPlyPos = DirectX::XMLoadFloat3(&plyPos);

	DirectX::XMVECTOR screenPlyPos, screenPly2Pos;
	screenPlyPos = DirectX::XMVector3Project(
		worldPlyPos,
		0.0f, 0.0f, screenWidth, screenHeight,
		0.0f, 1.0f, projection, view, world
	);

	DirectX::XMFLOAT2 screenPlyPosition;
	DirectX::XMStoreFloat2(&screenPlyPosition, screenPlyPos);

	//ゲージ描画
	const float gaugeWidth = 120.0f;
	const float gaugeHeight = 20.0f;

	/*spriteUI->Render(rc,
		-4.946f, 2.0f, -14.102f,
		10.0f,10.
		0,
		0, 1, 1, 1
	);*/
}

// playerの当たり判定がgoalPosに当たったらtrue返すよ
bool SceneGame::playBoss()
{
	DirectX::XMFLOAT3 playerPos = player->GetPosition();
	float radius = player->GethitRadius();

	// XZ平面で距離チェック（2D扱い）
	float dx = playerPos.x - goalPos.x;
	float dz = playerPos.z - goalPos.z;

	// 距離²
	float distSq = dx * dx + dz * dz;

	// 半径内に入ったら
	if (distSq <= radius * radius)
		return true;
	else
		return false;
}
