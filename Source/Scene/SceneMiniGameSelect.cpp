#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneManager.h"
#include "SceneMiniGameSelect.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneMiniGameTyping.h"
#include "Scene/SceneMiniGame2048.h"

void SceneMiniGameSelect::Initialize()
{
}

void SceneMiniGameSelect::Finalize()
{
	
}

void SceneMiniGameSelect::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	//次のシーンが完了したらシーンを切り替える
	if (GetAsyncKeyState('F' & 0x8000))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneMiniGame2048));
	}
}

void SceneMiniGameSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	{
		//画面右下にローディングアイコン描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float spriteWidth = 256;
		float spriteHeight = 256;
		float positionX = screenWidth - spriteWidth;
		float positionY = screenHeight - spriteHeight;

		sprite->Render(rc,
			positionX, positionY, 0, spriteWidth, spriteHeight,
			angle,
			1, 1, 1, 1);
	}
}

void SceneMiniGameSelect::DrawGUI()
{

}