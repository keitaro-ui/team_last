#pragma once
#include "../Source/Game/Stage.h"
#include "../Source/Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "../Source/Scene/Scene.h"
#include "System/Sprite.h"
#include <PhysicsSystem2d.h>
#include "imgui.h"

class SceneBoss : public Scene
{
public:
	SceneBoss() {};
	~SceneBoss() override {};

	// ‰Šú‰»
	void Initialize() override;

	// I—¹‰»
	void Finalize() override;

	// XVˆ—
	void Update(float elapsedTime) override;

	// •`‰æˆ—
	void Render() override;

	// GUI•`‰æ
	void DrawGUI() override;

	void RenderUI();

private:
	// unique_ptr
	std::unique_ptr<Stage> stage = nullptr;
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<CameraController> cameraController = nullptr;

};
