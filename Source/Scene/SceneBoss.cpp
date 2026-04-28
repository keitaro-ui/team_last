#include "SceneBoss.h"
#include <System/Graphics.h>
#include <CameraController.h>
#include <Camera.h>
#include <System/Input.h>

void SceneBoss::Initialize()
{
	//ステージ初期化
	stage = std::make_unique<Stage>();
	stage->SetPosition(DirectX::XMFLOAT3(10, -5, 10));
	stage->SetStageNum(1);

	//プレイヤー初期化
	player = std::make_unique<Player>();
	player->SetPosition({ -22.8f, 1.0f, -26.0f });

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

	//debugのマウスカーソル
	if (!player->GetPrev())
	{
		// マウス位置の取得とロック
		Input::Instance().GetMouse().Lock();
		cameraController->MouseCamera(elapsedTime);
	}
	else
	{
		Input::Instance().GetMouse().Unlock();
	}
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
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		//player->RenderDebugPrimitive(rc, shapeRenderer);

	}

	// 2Dスプライト描画
	{

	}
}

void SceneBoss::DrawGUI()
{
}

void SceneBoss::RenderUI()
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