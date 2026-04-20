#pragma once

#include "Character.h"
#include "System/ModelRenderer.h";

//ステージ
class Stage:public Character
{
public:
	Stage();
	~Stage();

	//更新処理
	void Update(float elapsedTime);

	void DrawDebugGUI();

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	Model* model = nullptr;
};
