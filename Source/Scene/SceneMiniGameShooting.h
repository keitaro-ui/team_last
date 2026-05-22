//#pragma once

#include "../Source/Game/Stage.h"
#include "../Source/Game/PlayerShooting.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "Scene.h"
#include "System/Sprite.h"
#include <PhysicsSystem2d.h>
#include "imgui.h"
#include "../Source/Game/MiniGame.h"
#include "../Game/Balloon.h"

// ゲームシーン
class SceneGameShooting : public Scene
{
public:
	SceneGameShooting() {};
	~SceneGameShooting() override {};

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	// GUI描画
	void DrawGUI() override;

	void RenderUI(RenderContext rc);

	int GetCount() const
	{
		return count;
	}

private:
	// unique_ptr
	std::unique_ptr<Stage> stage = nullptr;
	std::unique_ptr<PlayerShooting> player = nullptr;
	std::unique_ptr<CameraController> cameraController = nullptr;
	//std::vector<std::unique_ptr<Balloon>>balloon;
	Balloon* balloon = nullptr;

	// sprite定義
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite_number = nullptr;
	std::unique_ptr<Sprite> sprite_text = nullptr;
	std::unique_ptr<Sprite>spriteUI = nullptr;

	float timer = 10.0f;
	float game_timer;

	//当たり判定
	PhysicsSystem2d physics;
	float xDis = 0.0f;
	float zDis = 0.0f;
	DirectX::XMFLOAT3 obbPos = { 0,0,0 };
	DirectX::XMFLOAT3 blockSize = { 0.0f, 0.0f, 0.0f };

	// ゴール
	DirectX::XMFLOAT3 goalPos = { 0.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT3 uiE = { -7.746f,4.0f,-40.663f };
	//ゲージ描画
	const float gaugeWidth = 120.0f;
	const float gaugeHeight = 20.0f;

	EnemyManager& enemyManager = EnemyManager::Instance();

	int count;
	int score;

public:

};
