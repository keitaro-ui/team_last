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
	void DrawDebugGUI();

	// getter
	bool GetTitleSwitch() const { return titleSwitch; }

	// setter
	void SetTitleSwitch(bool flag) { titleSwitch = flag; }

protected:
	//死亡したときに呼ばれる
	void OnDead() override;


private:

	Model* model[2];

	bool titleSwitch = false;

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
