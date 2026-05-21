#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include <thread>

//ローディングシーン
class SceneMiniGameSelect : public Scene
{
public:
	SceneMiniGameSelect(Scene* nextScene) : nextScene(nextScene) {}
	SceneMiniGameSelect() {};
	~SceneMiniGameSelect() override {}

	//初期化
	void Initialize() override;

	//終了化
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render() override;

	//GUI描画
	void DrawGUI() override;

private:
	float cooltime = 0.0f;
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
	POINT cursorPos;

	bool select = false;
	bool modeFlow = false;
	bool modeShot1 = false;
	bool modeShot2 = false;

	float screenWidth;
	float screenHeight;
	DirectX::XMFLOAT2 size;

	std::unique_ptr<Sprite> flow = nullptr;
	std::unique_ptr<Sprite> shot = nullptr;
	std::unique_ptr<Sprite> flowTutorial = nullptr;
	std::unique_ptr<Sprite> shotTutorial1 = nullptr;
	std::unique_ptr<Sprite> shotTutorial2 = nullptr;
};