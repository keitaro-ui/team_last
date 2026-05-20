#include "System/Graphics.h"
#include "SceneTitle.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "System/Mouse.h"
#include "../Game/Player.h"
#include "../Scene/SceneTutorial.h"

//初期化
void SceneTitle::Initialize()
{
	//スプライト初期化
	sprite = std::make_unique<Sprite>("Data/Sprite/title.png");
    sprite2 = std::make_unique<Sprite>("Data/Sprite/start.png");
    sprite3 = std::make_unique<Sprite>("Data/Sprite/tutorial.png");

    ShowCursor(true);

    titlestage = std::make_unique<Stage>(3);
    titlestage->SetPosition({ 0.0f, -3.0f, 3.8f });

    player = std::make_unique<Player>(1);
    //pos = player->GetPosition();
}

extern POINT cursorPos;

//終了化
void SceneTitle::Finalize()
{
    ShowCursor(false);	
}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
    GetCursorPos(&cursorPos);
    HWND hwnd = GetForegroundWindow();
    ScreenToClient(hwnd, &cursorPos);

    Mouse& mouse = Input::Instance().GetMouse();

    player->SetPosition(pos);
    player->Update(elapsedTime);

    //左クリックで画面遷移
    //スタート
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (cursorPos.x >= 505 && cursorPos.x <= 765)
        {
            if (cursorPos.y >= 520 && cursorPos.y <= 585)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
            }
        }
    }
    //チュートリアル
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (cursorPos.x >= 505 && cursorPos.x <= 765)
        {
            if (cursorPos.y >= 600 && cursorPos.y <= 670)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
            }
        }
    }
}

//描画処理
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();
    ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

    {
        player->Render(rc, modelRenderer);
    }

	//2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        sprite->Render(rc,
            0, 0, 0, screenWidth, screenHeight,
            0,
            1, 1, 1, 1);
            //スタートとチュートリアルの描画と拡大
            if (cursorPos.x >= 505 && cursorPos.x <= 765)
            {
                //スタート
                if (cursorPos.y >= 520 && cursorPos.y <= 585)
                {
                    //拡大
                    sprite2->Render(rc,
                        60, 25, 0, 1200, 700,
                        0,
                        1, 1, 1, 1);
                }
                else
                {
                    sprite2->Render(rc,
                        150, 100, 0, 1000, 600,
                        0,
                        1, 1, 1, 1);
                }

                //チュートリアル
                if (cursorPos.y >= 600 && cursorPos.y <= 670)
                {
                    //拡大
                    sprite3->Render(rc,
                        60, 20, 0, 1200, 700,
                        0,
                        1, 1, 1, 1);
                }
                else
                {
                    sprite3->Render(rc,
                        150, 100, 0, 1000, 600,
                        0,
                        1, 1, 1, 1);
                }
            }
            else
            {
                //通常時の描画
                sprite2->Render(rc,
                    150, 100, 0, 1000, 600,
                    0,
                    1, 1, 1, 1);

                sprite3->Render(rc,
                    150, 100, 0, 1000, 600,
                    0,
                    1, 1, 1, 1);
            }


            DrawGUI();
	}
}

//GUI描画
void SceneTitle::DrawGUI()
{

    ImGui::Begin("Player Position");

    ImGui::DragFloat3("Position", &pos.x, 0.1f);

    ImGui::End();
}