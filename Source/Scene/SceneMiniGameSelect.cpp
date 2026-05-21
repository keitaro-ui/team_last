#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneManager.h"
#include "SceneMiniGameSelect.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneMiniGameTyping.h"
#include "Scene/SceneMiniGameShooting.h"
#include "Scene/SceneMiniGame2048.h"

void SceneMiniGameSelect::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	screenWidth = static_cast<float>(graphics.GetScreenWidth());
	screenHeight = static_cast<float>(graphics.GetScreenHeight());

	size = { screenWidth / 10 * 4, screenHeight / 4 * 3};

	select = true;
	ShowCursor(true);

	flow = std::make_unique<Sprite>("Data/Sprite/flow.png");
	shot = std::make_unique<Sprite>("Data/Sprite/trivia.png");
	flowTutorial = std::make_unique<Sprite>("Data/Sprite/tutoSprite.png");
	shotTutorial1 = std::make_unique<Sprite>("Data/Sprite/チュートリアル問題.png");
	shotTutorial2 = std::make_unique<Sprite>("Data/Sprite/チュートリアル解答.png");
}

void SceneMiniGameSelect::Finalize()
{
	select = false;
	modeFlow = false;
	modeShot1 = false;
	modeShot2 = false;
}

void SceneMiniGameSelect::Update(float elapsedTime)
{
	// マウス取得のための初期設定
	GetCursorPos(&cursorPos);
	HWND hwnd = GetForegroundWindow();
	ScreenToClient(hwnd, &cursorPos);
	Mouse& mouse = Input::Instance().GetMouse();

	constexpr float speed = 180;
	angle += speed * elapsedTime;
	cooltime += elapsedTime;

	//左クリックで画面遷移
   //スタート
	float xPos = 50.0f;
	float yPos = screenHeight / 3;

	if(modeFlow && mouse.GetButtonDown() & Mouse::BTN_LEFT && cooltime > 0.2f)
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneMiniGame2048));

	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		if (cursorPos.x >= xPos && cursorPos.x <= size.x)
		{
			if (cursorPos.y >= yPos - 150.0f && cursorPos.y <= yPos + size.y - 150.0f)
			{
				modeFlow = true;
			}
		}
	}

	if(modeShot2 && mouse.GetButtonDown() & Mouse::BTN_LEFT)
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameShooting));

	if (modeShot1 && mouse.GetButtonDown() & Mouse::BTN_LEFT && cooltime > 0.2f)
		modeShot2 = true;
		
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		if (cursorPos.x >= screenWidth / 2 + xPos && cursorPos.x <= screenWidth / 2 + size.x)
		{
			if (cursorPos.y >= yPos && cursorPos.y <= yPos + size.y)
			{
				modeShot1 = true;
			}
		}
	}
}

void SceneMiniGameSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	{
		
		if (select)
		{
			flow->Render(rc,
				/*screenWidth + */50.0f, screenHeight / 3 - 150.0f, 0,
				size.x, size.y, 0,
				1, 1, 1, 1);

			shot->Render(rc,
				screenWidth / 2 + 50.0f, screenHeight / 3, 0,
				size.x, size.y / 2, 0,
				1, 1, 1, 1);
		}

		if (modeFlow)
		{
			flowTutorial->Render(rc,
				0, 0, 0,
				screenWidth, screenHeight, 0,
				1, 1, 1, 1);
		}

		if (modeShot1)
		{
			shotTutorial1->Render(rc,
				0, 0, 0,
				screenWidth, screenHeight, 0,
				1, 1, 1, 1);
		}

		if (modeShot2)
		{
			shotTutorial2->Render(rc,
				0, 0, 0,
				screenWidth, screenHeight, 0,
				1, 1, 1, 1);
		}
	}
}

void SceneMiniGameSelect::DrawGUI()
{

}