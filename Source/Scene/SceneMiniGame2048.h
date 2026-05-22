#pragma once

#include "../Game/Stage.h"
#include "../Game/Player2048.h"
#include "../Game/CameraController2048.h"
#include "DirectXMath.h"
#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Box.h"
#include "../Game/Grid.h"
#include "SoundManager.h"
#include "SceneResult.h"

// ゲームシーン
class SceneMiniGame2048 : public Scene
{
public:
	SceneMiniGame2048() {};
	~SceneMiniGame2048() override {};

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

	// scoreのゲッター
	int GetScore() const
	{
		return score;
	}
private:

	float game_timer;
	float limit_timer;
	const float coolTime = 1.0f;

	std::unique_ptr<Stage> stage = nullptr;

	std::unique_ptr<Player2048> player2048 = nullptr;

	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite_number = nullptr;
	std::unique_ptr<Sprite> sprite_text = nullptr;
	std::unique_ptr<Sprite> sprite_black = nullptr;

	CameraController2048* cameraController = nullptr;

	// スクリーンサイズ
	float screenWidth = 0;
	float screenHeight = 0;

	//使う箱の種類
	std::unique_ptr<Box> boxes[11];

	//重なったらtrue
	bool overlap = false;
	bool up = false;

	//mapの1マスの間隔
	float tileSize;
	//map[0][0]の位置
	DirectX::XMFLOAT3 startPos;

	int count;

	int score;

	void UpdateCursorToggle();
};

