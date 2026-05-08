#pragma once
#include <DirectXMath.h>
#include "System/RenderContext.h"
#include "System/ModelRenderer.h"
#include "Character.h"

class Typing:public Character
{
public:
	Typing();
	~Typing();

	//XVˆ—
	void Update(float elapsedTime);

	void DrawDebugGUI();

	//•`‰æˆ—
	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	Model* model = nullptr;
};