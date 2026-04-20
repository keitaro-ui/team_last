#include "Stage.h"
#include <imgui.h>

//コンストラクタ
Stage::Stage()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/Stage/Noroof_offeceroom_scale(1).mdl");
	scale.x = scale.y = scale.z = 0.1f;
}

Stage::~Stage()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
	//行列更新
	UpdateTransform();
	model->UpdateTransform();

}

//描画処理
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	/*DirectX::XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());*/

	//レンダラモデルに描画してもらう
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

void Stage::DrawDebugGUI()
{
	static float uniformScale = 1.2f;

	if (ImGui::DragFloat("Scale", &uniformScale, 0.01f, 0.0f, 10.0f))
	{
		scale.x = scale.y = scale.z = uniformScale;
	}
}
