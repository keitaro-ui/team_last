#include "System/Graphics.h"
#include"System/Input.h"
#include "SceneTutorial.h"
#include "Camera.h"
#include "../Game/EnemyManager.h"
#include "time.h"
#include "random"
#include "algorithm"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"


// 初期化
void SceneTutorial::Initialize()
{
	spr1 = std::make_unique<Sprite>("Data/Sprite/tutorial_office1.png");
	spr2 = std::make_unique<Sprite>("Data/Sprite/newlose.png");

	//マウス位置の取得とロック
	Input::Instance().GetMouse().Lock();
	//Input::Instance().GetMouse();
}

// 終了化
void SceneTutorial::Finalize()
{
	Input::Instance().GetMouse().Unlock();
	state = 0;
	coolTime = 0.0f;
}

// 更新処理
void SceneTutorial::Update(float elapsedTime)
{
	coolTime += elapsedTime;

	GamePad& gamePad = Input::Instance().GetGamePad();
	const GamePadButton anyButton = GamePad::BTN_START;
	
	if (gamePad.GetButtonDown() & anyButton && state == 0)
	{
		state++;
		coolTime = 0.0f;
	}

	if (gamePad.GetButtonDown() & anyButton && state == 1 && coolTime > 0.5f)
	{
		state++;
		coolTime = 0.0f;
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}

	//シーン遷移
	if (gamePad.GetButtonDown() & anyButton && state == 2 && coolTime > 0.5f)
	{
	}
	
}

// 描画処理
void SceneTutorial::Render()
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

	
	// 2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		if (state == 0)
		{
			spr1->Render(rc,
				0, 0, 0,
				screenWidth, screenHeight, 0,
				1, 1, 1, 1);
		}
		else if (state == 1)
		{
			spr2->Render(rc,
				0, 0, 0,
				screenWidth, screenHeight, 0,
				1, 1, 1, 1);
		}
	}
}

// GUI描画
void SceneTutorial::DrawGUI()
{
	//プレーヤーデバッグ処理
	//player->DrawDebugGUI();
}
