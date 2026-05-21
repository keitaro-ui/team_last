#include "System/Graphics.h"
#include "SceneResult.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneBoss.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include <imgui.h>
#include "SceneTitle.h"

//初期化
void SceneResult::Initialize()
{
	//スプライト
	sprite = std::make_unique<Sprite>("Data/Sprite/result.png");
	win = std::make_unique<Sprite>("Data/Sprite/newwin.png");
	lose = std::make_unique<Sprite>("Data/Sprite/newlose.png");

	// クリア状態をセット
	SceneBoss sceneBoss;
	gameClear = sceneBoss.GetGameClear();

	//gameClear = true;
}

//終了化
void SceneResult::Finalize()
{
	ShowCursor(true);
	gameClear = false;
}

//更新処理
void SceneResult::Update(float elapsedTime)
{


	// タイトルへ戻る
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_START)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		//SceneManager::Instance().ChangeScene((new SceneGameproject));

		if (gameClear = false)
		{

		}
		else
		{

		}
	}
	
}

//描画処理
void SceneResult::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	// スクリーンサイズ
	screenWidth = static_cast<float>(graphics.GetScreenWidth());
	screenHeight = static_cast<float>(graphics.GetScreenHeight());

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	/*{

		sprite->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

	}*/
	if (gameClear == false)
	{
		lose->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

	}
	if (gameClear == true)
	{
		win->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

	}
}

//GUI描画
void SceneResult::DrawGUI()
{
	/*ImGui::Text("result = %d", result+1);
	ImGui::Text("point = %d", point);
	ImGui::Text("boad_answer = &d", board_answer);*/
}
