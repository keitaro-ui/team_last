#include "System/Graphics.h"
#include "SceneTitle.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "System/Mouse.h"
#include "../Game/Player.h"
#include "../Scene/SceneTutorial.h"
#include "Game/PlayerManager.h"
#include"Camera.h"
#include"Game/EnemyManager.h"

//初期化
void SceneTitle::Initialize()
{
    //スプライト初期化
    sprite = std::make_unique<Sprite>("Data/Sprite/ok.png");
    sprite2 = std::make_unique<Sprite>("Data/Sprite/start.png");
    sprite3 = std::make_unique<Sprite>("Data/Sprite/tutorial.png");

    ShowCursor(true);

    titlestage = std::make_unique<Stage>(3);
    titlestage->SetPosition({ 0.0f, -3.0f, 3.8f });

    player = std::make_unique<Player>(1);

    PlayerManager::Instance().Register(player.get());
    //PlayerManager::Instance().GetPlayer()->SetProv(true);
    PlayerManager::Instance().GetPlayer()->SetPosition({ -10.0f, -3.0f, 6.8f });
    //pos = player->GetPosition();

    boss = std::make_unique<EnemySlime>();
    boss->SetScale({ 0.06f,0.06f,0.06f });
    boss->SetPosition({ 12.5f,-1.1f,3.1f });
    //EnemyManager::Instance().Register(boss.get());
    //EnemyManager::Instance().GetEnemy(0)->SetPosition({-10.0f, -3.0f, 6.8f});

    //カメラコントローラー初期化
    cameraController = new CameraController();
    //cameraController->angle.y = DirectX::XMConvertToRadians(45.0f);
    cameraController->angle.x = DirectX::XMConvertToRadians(78.0f);
    cameraController->distance = 29.8f;
    //player->cameraController = cameraController;
    DirectX::XMFLOAT3 target = titlestage->GetPosition();
    cameraController->SetTarget(target);

    //カメラ初期設定
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
    
    boss->SetTitleSwitch(true);
}

extern POINT cursorPos;

//終了化
void SceneTitle::Finalize()
{
    ShowCursor(false);	
    delete cameraController;
}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
    titlestage->Update(elapsedTime);
    player->Update(elapsedTime);
    boss->Update(elapsedTime);

    GetCursorPos(&cursorPos);
    HWND hwnd = GetForegroundWindow();
    ScreenToClient(hwnd, &cursorPos);

    Mouse& mouse = Input::Instance().GetMouse();

    

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

    //カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    {
        titlestage->UpdateTransform();
        titlestage->Render(rc, modelRenderer);
        player->Render(rc, modelRenderer);
        boss->UpdateTransform();
        boss->Render(rc, modelRenderer);
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


            //DrawGUI();

            player->DrawDebugGUI();
            boss->DrawDebugGUI();
	}
    
}

//GUI描画
void SceneTitle::DrawGUI()
{

    ImGui::Begin("Player Position");

    ImGui::DragFloat3("Position", &pos.x, 0.1f);

    ImGui::End();
}