#pragma once

#include "System/Model.h"
#include "Enemy.h"

//スライム
class EnemySlime : public Enemy
{
public:
	EnemySlime();
	~EnemySlime() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

	void CreateModel();

protected:
	//死亡したときに呼ばれる
	void OnDead() override;

	void DrawDebugGUI();

private:

	Model* model[2];

	enum class BossAnimation
	{
		punch = 0,
		kick,
		down,
		damage,
		title,
		dance1,
		dance2,
		Idle,
	};
};
