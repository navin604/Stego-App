#include "Application.h"

namespace App
{
	void RenderUI()
	{
		ImGui::Begin("Settings");
		ImGui::Button("Hello");
		ImGui::End();
	}

}