#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Player.h"
#include "../Game/Stage.h"
#include "CameraController.h"
#include "Game/EnemySlime.h"	
//タイトルシーン
class SceneTitle :public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

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
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Sprite> sprite3 = nullptr;
	
	std::unique_ptr<Player>player = nullptr;
	std::unique_ptr<EnemySlime>boss = nullptr;

	CameraController* cameraController = nullptr;

	std::unique_ptr<Stage> titlestage = nullptr;
	DirectX::XMFLOAT3 angleboss = { 0,0,0 };
	DirectX::XMFLOAT3 anglepl = { 0,0,0 };

	DirectX::XMFLOAT3 pos = { 0,0,0 };
};
