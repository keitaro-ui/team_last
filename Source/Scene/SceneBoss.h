#pragma once
#include "../Source/Game/Stage.h"
#include "../Source/Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "../Source/Scene/Scene.h"
#include "System/Sprite.h"
#include <PhysicsSystem2d.h>
#include "imgui.h"
#include <functional>
#include "../Game/Player.h"

class SceneBoss : public Scene
{
public:
	SceneBoss() {};
	~SceneBoss() override {};

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

	// クリア状態のゲッター
	bool GetGameClear() const { return gameClear; }

private:
	// クリア状態の管理
	bool gameClear = false;

	// バトル時に使う変数
	struct Employee
	{
		int hp;
		int punch;
		int kick;
		int special;
	};
	Employee emp;

	struct President
	{
		int hp;
		int punch;
		int kick;
	};
	President pre;

	// ルーレット
	int SceneBoss::BossRoulette(float elapsedTime, int maxCount);
	int rouletteIndex = 0;
	int resultIndex = -1;
	int resultHitCount = 0;
	int roulette = 0;
	float rouletteTimer = 0.0f;
	float rouletteInterval = 0.1f;
	float cooltime;
	bool attack = 0;
	bool isRoulette = false;
	bool isRouletteStop = false;

	// spacekey用変数
	enum state
	{
		START,
		STOP,
		NONE
	};
	int state = 0;

	// frameの数
	static constexpr int commands = 7;

	// レベルアップ用変数
	int nowLevel = 0;
	bool isLevelUp = false;
	int slide = 0;
	float slideSize = 0.0f;
	float upSlideSize = 0.0f;

	// unique_ptr
	std::unique_ptr<Stage> stage = nullptr;
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<CameraController> cameraController = nullptr;
	std::unique_ptr<Sprite> triangle_black = nullptr;
	std::unique_ptr<Sprite> frames[commands];

	// スクリーンサイズ
	int screenWidth = 0;
	int screenHeight = 0;

	// ルーレット結果をここに書き込む
	void rouletteResult(int result);

	// レベルアップ処理を書く
	void levelUp(float elapsedTime);

	// 技威力調整用関数
	void SetPlayerPunch();
	void SetPlayerKick();

};
