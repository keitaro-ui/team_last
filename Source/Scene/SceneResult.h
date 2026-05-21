#pragma once

#include "System/Sprite.h"
#include "Scene.h"

//タイトルシーン
class SceneResult :public Scene
{
public:
	SceneResult() {}
	~SceneResult() override {}

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

	std::unique_ptr<Sprite> win = nullptr;
	std::unique_ptr<Sprite> lose = nullptr;

	// スクリーンサイズ
	float screenWidth = 0.0f;;
	float screenHeight = 0.0f;

	// クリア状態
	bool gameClear = false;


};
