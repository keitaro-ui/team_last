//#pragma once

#include "../Source/Game/Stage.h"
#include "../Source/Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "Scene.h"
#include "System/Sprite.h"
#include <PhysicsSystem2d.h>
#include "imgui.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {};
	~SceneGame() override {};

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

private:
	float game_timer;

	//Stage* stage = nullptr;
	std::unique_ptr<Stage> stage = nullptr;

	//Player* player = nullptr;
	std::unique_ptr<Player> player = nullptr;

	//レティクル関数
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite_number = nullptr;
	std::unique_ptr<Sprite> sprite_text = nullptr;

	std::unique_ptr<CameraController> cameraController = nullptr;

	//当たり判定
	PhysicsSystem2d physics;
	float xDis = 0.0f;
	float zDis = 0.0f;
	DirectX::XMFLOAT3 blockSize = {0.0f, 0.0f, 0.0f};
public:

};
