#include "GUI_Components.h"
#include "../Backend/Validation.h"


namespace App
{
	void RenderUI(PWSTR& BaseFilePath, PWSTR& SecretFilePath)
	{
		bool show_demo_window = true;
		bool error = false;
		bool button_clicked = false;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Begin("Image Encoder");
		if (ImGui::Button("Select Cover Image")) {
			if (BaseFilePath != nullptr)
			{
				CoTaskMemFree(BaseFilePath);
			}
			std::cout << "Opening file explorer" << std::endl;
			BaseFilePath = Validation::SelectFile();
		}
		
		if (ImGui::Button("Select Secret Image")) {
			if (SecretFilePath != nullptr)
			{
				CoTaskMemFree(SecretFilePath);
			}
			std::cout << "Opening file explorer" << std::endl;
			SecretFilePath = Validation::SelectFile();
			//std::wcout << SecretFilePath << std::endl;
		}
		ImGui::SeparatorText("Details");

		if (BaseFilePath != nullptr)
		{

			std::string cover_str = Validation::filename_to_string(BaseFilePath);
			ImGui::Text("Cover File: %s", cover_str.c_str());
			
		}
		else {
			ImGui::Text("Cover File: None");
		}
			
		if (SecretFilePath != nullptr) 
		{ 
			std::string secret_str = Validation::filename_to_string(SecretFilePath);
			ImGui::Text("Secret File: %s", secret_str.c_str());
			
		}
		else {
			ImGui::Text("Secret File: None");
		}

		static char path[256] = "";
		ImGui::Text("Output File Path:");
		ImGui::SameLine();
		ImGui::InputText(" ", path, IM_ARRAYSIZE(path));



		if (ImGui::Button("Encode")) 
		{
			if (path[0] == '\0' or BaseFilePath == nullptr or SecretFilePath == nullptr)
			{
				ImGui::OpenPopup("File Error");
				
			} 
			else if (Validation::check_filesize(BaseFilePath, SecretFilePath, path))
			{
				Validation::prep_bits(BaseFilePath, SecretFilePath, path);
			}
			else
			{
				ImGui::OpenPopup("Size Error");
			}
			

		}

		
		if (ImGui::BeginPopupModal("File Error")) {
			ImGui::Text("An error has occurred.");
			ImGui::Text("Please choose a valid file.");
			if (ImGui::Button("OK")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Size Error")) {
			ImGui::Text("An error has occurred.");
			ImGui::Text("Cover image is not large enough");
			if (ImGui::Button("OK")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}ImGui::End();
	}


   


}