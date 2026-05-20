#pragma once

#include <set>
#include<vector>
#include"Player.h"
#include"Player2048.h"

//エネミーマネージャー
class PlayerManager
{
private:
	PlayerManager() {}
	~PlayerManager() {}

public:
	//唯一のインスタンス取得
	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	void Register(Player* p) { player = p; }
	void Register2048(Player2048* p) { player2048 = p; }

	Player* GetPlayer() { return player; }
	Player2048* GetPlayer2048() { return player2048; }

private:
	Player* player;
	Player2048* player2048;
};
