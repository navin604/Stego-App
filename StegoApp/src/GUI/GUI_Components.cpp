#include "GUI_Components.h"
#include "../Backend/Validation.h"
#include "../Backend/Utils.h"


namespace App
{
	void RenderUI(PWSTR& BaseFilePath, PWSTR& SecretFilePath)
	{
		ImGui::Begin("Image Handler");
		if (ImGui::CollapsingHeader("Encode Image"))
		{
			ImGui::SeparatorText("Files");
			if (ImGui::Button("Select Cover Image")) {
				if (BaseFilePath != nullptr)
				{
					CoTaskMemFree(BaseFilePath);
				}
				BaseFilePath = Validation::SelectFile();
			}

			if (ImGui::Button("Select Secret Image")) {
				if (SecretFilePath != nullptr)
				{
					CoTaskMemFree(SecretFilePath);
				}
				SecretFilePath = Validation::SelectFile();
				
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

			ImGui::SeparatorText("Password");
			static char pass[256] = "";
			ImGui::Text("Enter Password");
			ImGui::SameLine();
			ImGui::InputText("  ", pass, IM_ARRAYSIZE(pass));


			if (ImGui::Button("Encode"))
			{
				if (path[0] == '\0' or BaseFilePath == nullptr or SecretFilePath == nullptr or pass[0] == '\0')
				{
					ImGui::OpenPopup("Input Error");

				}
				else if (Validation::check_filesize(BaseFilePath, SecretFilePath, path, pass))
				{
					std::vector<char> bits_to_embed;
					bits_to_embed = Validation::prep_bits(BaseFilePath, SecretFilePath, path, pass);
					Stego::embed(bits_to_embed, BaseFilePath, path);
				}
				else
				{
					ImGui::OpenPopup("Size Error");
				}


			}
			


			if (ImGui::BeginPopupModal("Input Error")) {
				ImGui::Text("An error has occurred.");
				ImGui::Text("Ensure files and a password have been entered!");
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
			}
		}
		
		


		if (ImGui::CollapsingHeader("Decode Image"))
		{
			ImGui::SeparatorText("Details");
			static char output_path[256] = "";
			ImGui::Text("Image to Decode:");
			ImGui::SameLine();
			ImGui::InputText("   ", output_path, IM_ARRAYSIZE(output_path));


			ImGui::SeparatorText("Password");
			static char password[256] = "";
			ImGui::Text("Enter Password");
			ImGui::SameLine();
			ImGui::InputText("    ", password, IM_ARRAYSIZE(password));

			if (ImGui::Button("Decode"))
			{
				if (output_path[0] == '\0' or password[0] == '\0')
				{
					ImGui::OpenPopup("Error");

				}
				else
				{
					if (!Stego::decode(output_path, password))
					{
						ImGui::OpenPopup("Password");
					}

				}
			}

			if (ImGui::BeginPopupModal("Error")) {
				ImGui::Text("An error has occurred.");
				ImGui::Text("Please enter a file path and/or password.");
				if (ImGui::Button("OK")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopupModal("Password")) {
				ImGui::Text("An error has occurred.");
				ImGui::Text("Incorrect Password");
				if (ImGui::Button("OK")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}


		}	
		ImGui::End();
	}


   


}