#include "MiniGame.h"
#include "Imgui.h"

Typing::Typing()
{
	model= new Model("Data/Model/Boad/paper.mdl");
}

Typing::~Typing()
{
	delete model;
}

void Typing::Update(float elapsedTime)
{
	model->UpdateTransform();
}

void Typing::DrawDebugGUI()
{
	ImGui::Begin("Typing");

	ImGui::DragFloat3("Scale", &scale.x, 1.0f);
	ImGui::Text("Scale: %.2f %.2f %.2f", scale.x, scale.y, scale.z);
	ImGui::DragFloat3("Position", &position.x, 0.1f);

	ImGui::End();
}

void Typing::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
