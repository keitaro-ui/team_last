#pragma once
#include "../Source/Game/Stage.h"
#include "../Source/Game/Player.h"
#include "Game/CameraController2048.h"
#include "DirectXMath.h"
#include "../Source/Scene/Scene.h"
#include "System/Sprite.h"
#include <PhysicsSystem2d.h>
#include "imgui.h"
#include <functional>
#include "../Game/Player.h"
#include "Game/EnemySlime.h"

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
	bool isRoulette = false;
	bool isRouletteStop = false;
	float stopTimer = 0.0f;
	bool isStopping = false;

	// その他
	bool attack = 0;
	int punch = 0;
	int kick = 0;
	bool punchPlayer = false;
	bool kickPlayer = false;
	bool specialPlayer = false;
	bool punchEnemy = false;
	bool kickEnemy = false;
	bool dancePlayer = false;
	bool rightDown = false;

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
	int slide = 0;
	int nowLevel = 0;
	float slideSize = 0.0f;
	float upSlideSize = 0.0f;
	bool isLevelUp = false;

	// unique_ptr
	std::unique_ptr<Stage> stage = nullptr;
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<CameraController2048> cameraController = nullptr;
	std::unique_ptr<Sprite> triangle_black = nullptr;
	std::unique_ptr<Sprite> frames[commands];
	std::unique_ptr<Sprite> playerPunch;
	std::unique_ptr<Sprite> playerKick;
	std::unique_ptr<Sprite> playerSpecial;
	std::unique_ptr<Sprite> playerDance;
	std::unique_ptr<Sprite> enemyPunch;
	std::unique_ptr<Sprite> enemyKick;
	std::unique_ptr<Sprite> underHp;
	std::unique_ptr<Sprite> topHp;

	DirectX::XMFLOAT3 postionpl = {-40.925f, 1.0f,41.388f};
	DirectX::XMFLOAT3 postionboss = {-41.925f, 1.0f,79.016f};
	DirectX::XMFLOAT3 scalepl{ 0.1f,0.1f,0.1f };

	DirectX::XMFLOAT3 EYE = { -18,22, 36 };
	DirectX::XMFLOAT3 TARGET = { -284,-91,243 };
	
	DirectX::XMFLOAT3 angleboss = { 0,66.0f,0 };

	EnemySlime* boss = nullptr;

	// スクリーンサイズ
	int screenWidth = 0;
	int screenHeight = 0;

	// ルーレット結果をここに書き込む
	void rouletteResult(int result);

	// レベルアップ処理を書く
	void levelUp(float elapsedTime);

	// 技威力調整用関数
	int SetPlayerPunch();
	int SetPlayerKick();

	void Motion();
	float php = 0;
	float ehp = 0;
};
