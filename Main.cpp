#include "BBE/BrotBoxEngine.h"
#include <iostream>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

#define EOL "\n"

class MyGame : public bbe::Game
{

	bool activateLogging = true;
	bool activateJsonLogging = true;
	bool activateUart = true;
	bool activateSeggerRtt = true;

	bool allowMalloc = false;
	bool failOnSizeTooBig = true;

	int selectedLicenseIndex = 0;
	bbe::String fullName = "";

	int selectedDeviceTypeIndex = 0;
	const char* deviceTypes[3] = { "STATIC", "SINK", "ASSET" };

	const char* chips[2] = { "_nrf52", "_nrf52840" };
	int selectedChipIndex = 0;

	bool isValidFruityMeshDir = false;
	const bbe::String openSourceLicense =
		"////////////////////////////////////////////////////////////////////////////////" EOL
		"// /****************************************************************************" EOL
		"// **" EOL
		"// ** Copyright (C) 2015-2021 M-Way Solutions GmbH" EOL
		"// ** Contact: https://www.blureange.io/licensing" EOL
		"// **" EOL
		"// ** This file is part of the Bluerange/FruityMesh implementation" EOL
		"// **" EOL
		"// ** $BR_BEGIN_LICENSE:GPL-EXCEPT$" EOL
		"// ** Commercial License Usage" EOL
		"// ** Licensees holding valid commercial Bluerange licenses may use this file in" EOL
		"// ** accordance with the commercial license agreement provided with the" EOL
		"// ** Software or, alternatively, in accordance with the terms contained in" EOL
		"// ** a written agreement between them and M-Way Solutions GmbH. " EOL
		"// ** For licensing terms and conditions see https://www.bluerange.io/terms-conditions. For further" EOL
		"// ** information use the contact form at https://www.bluerange.io/contact." EOL
		"// **" EOL
		"// ** GNU General Public License Usage" EOL
		"// ** Alternatively, this file may be used under the terms of the GNU" EOL
		"// ** General Public License version 3 as published by the Free Software" EOL
		"// ** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT" EOL
		"// ** included in the packaging of this file. Please review the following" EOL
		"// ** information to ensure the GNU General Public License requirements will" EOL
		"// ** be met: https://www.gnu.org/licenses/gpl-3.0.html." EOL
		"// **" EOL
		"// ** $BR_END_LICENSE$" EOL
		"// **" EOL
		"// ****************************************************************************/" EOL
		"////////////////////////////////////////////////////////////////////////////////" EOL;

	virtual void onStart() override
	{
	}
	virtual void update(float timeSinceLastFrame) override
	{
	}
	virtual void draw3D(bbe::PrimitiveBrush3D& brush) override
	{
	}

	bbe::String getCMakeString()
	{
		bbe::String cmake = "";

		cmake += bbe::String("set(PLATFORM ") + (selectedChipIndex == 0 ? "NRF52832" : "NRF52840") + ")" EOL;

		if (failOnSizeTooBig) cmake += "set(FAIL_ON_SIZE_TOO_BIG 0)" EOL;
		if (allowMalloc)      cmake += "set(ALLOW_MALLOC 1)" EOL;

		return cmake;
	}

	bbe::String getHeaderString()
	{
		bbe::String header = "";
		if (selectedLicenseIndex == 0)
		{
			header += openSourceLicense + EOL;
		}

		header += bbe::String("#define ACTIVATE_LOGGING ") + (activateLogging ? "1" : "0") + EOL;
		header += bbe::String("#define ACTIVATE_JSON_LOGGING ") + (activateJsonLogging ? "1" : "0") + EOL;
		header += bbe::String("#define ACTIVATE_UART ") + (activateUart ? "1" : "0") + EOL;
		header += bbe::String("#define ACTIVATE_SEGGER_RTT ") + (activateSeggerRtt ? "1" : "0") + EOL;

		return header;
	}

	bbe::String getCppString()
	{
		const bbe::String implTemplate =
			"#include \"DebugModule.h\"" EOL
			"#include \"Node.h\"" EOL
			"#include \"GlobalState.h\"" EOL
			"" EOL
			"void SetBoardConfiguration_%%%FULL_NAME%%%(BoardConfiguration* c)" EOL
			"{" EOL
			"    // TODO: Add supported board configurations here" EOL
			"}" EOL
			"" EOL
			"void SetFeaturesetConfiguration_%%%FULL_NAME%%%(ModuleConfiguration* config, void* module)" EOL
			"{" EOL
			"    if (config->moduleId == ModuleId::NODE)" EOL
			"    {" EOL
			"        NodeConfiguration* c = (NodeConfiguration*)config;" EOL
			"    }" EOL
			"    // TODO: Add configurations for base modules here" EOL
			"}" EOL
			"" EOL
			"void SetFeaturesetConfigurationVendor_%%%FULL_NAME%%%(VendorModuleConfiguration* config, void* module)" EOL
			"{" EOL
			"   // TODO: Add configuration for vendor modules here." EOL
			"}" EOL
			"" EOL
			"u32 InitializeModules_%%%FULL_NAME%%%(bool createModule)" EOL
			"{" EOL
			"    u32 size = 0;" EOL
			"    size += GS->InitializeModule<DebugModule>(createModule);" EOL
			"    // TODO " EOL
			"    return size;" EOL
			"}" EOL
			"" EOL
			"DeviceType GetDeviceType_%%%FULL_NAME%%%()" EOL
			"{" EOL
			"    return DeviceType::%%%DEVICETYPE%%%;" EOL
			"}" EOL
			"" EOL
			"Chipset GetChipset_%%%FULL_NAME%%%()" EOL
			"{" EOL
			"    return Chipset::%%%CHIPSET%%%;" EOL
			"}" EOL
			"" EOL
			"FeatureSetGroup GetFeatureSetGroup_%%%FULL_NAME%%%()" EOL
			"{" EOL
			"    return FeatureSetGroup::TODO; //Change this to a real, unique FeatureSetGroup" EOL
			"}" EOL
			"" EOL
			"u32 GetWatchdogTimeout_%%%FULL_NAME%%%()" EOL
			"{" EOL
			"    return 0; //Watchdog disabled by default, activate if desired" EOL
			"}" EOL
			"" EOL
			"u32 GetWatchdogTimeoutSafeBoot_%%%FULL_NAME%%%()" EOL
			"{" EOL
			"    return 0; //Safe Boot Mode disabled by default, activate if desired" EOL
			"}" EOL;

		bbe::String cppPreview = "";
		if (selectedLicenseIndex == 0)
		{
			cppPreview += openSourceLicense;
		}

		cppPreview += implTemplate
			.replace("%%%FULL_NAME%%%", fullName)
			.replace("%%%DEVICETYPE%%%", deviceTypes[selectedDeviceTypeIndex]);

		if (selectedChipIndex == 0)
		{
			cppPreview = cppPreview.replace("%%%CHIPSET%%%", "CHIP_NRF52");
		}
		else
		{
			cppPreview = cppPreview.replace("%%%CHIPSET%%%", "CHIP_NRF52840");
		}

		return cppPreview;
	}

	virtual void draw2D(bbe::PrimitiveBrush2D& brush) override
	{
		ImGui::SetNextWindowSize({ (float)getScaledWindowWidth() / 3.f, (float)getScaledWindowHeight() });
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::Begin("Settings", nullptr);
		ImGui::Text("FruityMesh Path:");
		ImGui::SameLine();
		static char fruityMeshPathBuffer[1024] = {};
		if (!isValidFruityMeshDir) ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.3, 0, 0, 1 });
		ImGui::InputText("##FruityMeshPath", fruityMeshPathBuffer, sizeof(fruityMeshPathBuffer));
		if (!isValidFruityMeshDir) ImGui::PopStyleColor();
		isValidFruityMeshDir = bbe::simpleFile::doesFileExist(bbe::String(fruityMeshPathBuffer) + "/cherrysim/CherrySim.cpp");
		ImGui::Text("Name:");
		ImGui::SameLine();
		static int selectedProdDevGithubComboIndex = 0;
		const char* prodDevGithub[] = { "prod_", "dev_", "github_" };
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		if (ImGui::BeginCombo("##proddevgithubcombo", prodDevGithub[selectedProdDevGithubComboIndex], ImGuiComboFlags_PopupAlignLeft))
		{
			for (size_t i = 0; i < sizeof(prodDevGithub) / sizeof(*prodDevGithub); i++)
			{
				if (ImGui::Selectable(prodDevGithub[i], selectedProdDevGithubComboIndex == i, 0, { 100 , 0 })) selectedProdDevGithubComboIndex = i;
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		static char nameBuffer[1024] = {};
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
		ImGui::InputText("##fsname", nameBuffer, sizeof(nameBuffer));
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::BeginCombo("##nrf52nrf52840", chips[selectedChipIndex], 0))
		{
			for (size_t i = 0; i < sizeof(chips) / sizeof(*chips); i++)
			{
				if (ImGui::Selectable(chips[i], selectedChipIndex == i, 0, { 150 , 0 })) selectedChipIndex = i;
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		ImGui::Text("License:");
		ImGui::SameLine();
		const char* licenses[] = { "M-Way Open Source", "None" };
		if (ImGui::BeginCombo("##licenses", licenses[selectedLicenseIndex], 0))
		{
			for (size_t i = 0; i < sizeof(licenses) / sizeof(*licenses); i++)
			{
				if (ImGui::Selectable(licenses[i], selectedLicenseIndex == i, 0, { 0 , 0 })) selectedLicenseIndex = i;
			}
			ImGui::EndCombo();
		}

		ImGui::Text("DeviceType:");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##deviceType", deviceTypes[selectedDeviceTypeIndex], 0))
		{
			for (size_t i = 0; i < sizeof(deviceTypes) / sizeof(*deviceTypes); i++)
			{
				if (ImGui::Selectable(deviceTypes[i], selectedDeviceTypeIndex == i, 0, { 0 , 0 })) selectedDeviceTypeIndex = i;
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();
		ImGui::Checkbox("ACTIVATE_LOGGING", &activateLogging);
		ImGui::Checkbox("ACTIVATE_JSON_LOGGING", &activateJsonLogging);
		ImGui::Checkbox("ACTIVATE_UART", &activateUart);
		ImGui::Checkbox("ACTIVATE_SEGGER_RTT", &activateSeggerRtt);

		ImGui::Separator();
		ImGui::Checkbox("Fail on size too big", &failOnSizeTooBig);
		ImGui::Checkbox("Allow link with malloc", &allowMalloc);

		fullName = bbe::String(prodDevGithub[selectedProdDevGithubComboIndex]) + bbe::String(nameBuffer) + bbe::String(chips[selectedChipIndex]);

		ImGui::Separator();
		if (isValidFruityMeshDir && ImGui::Button("Save (will overwrite without question!)"))
		{
			bbe::simpleFile::writeStringToFile(bbe::String(fruityMeshPathBuffer) + "/config/featuresets/" + fullName + ".cpp", getCppString());
			bbe::simpleFile::writeStringToFile(bbe::String(fruityMeshPathBuffer) + "/config/featuresets/" + fullName + ".h", getHeaderString());
			bbe::simpleFile::writeStringToFile(bbe::String(fruityMeshPathBuffer) + "/config/featuresets/" + fullName + ".cmake", getCMakeString());
		}

		ImGui::End();



		ImGui::SetNextWindowSize({ (float)getScaledWindowWidth() / 2.f * 3.f, (float)getScaledWindowHeight() });
		ImGui::SetNextWindowPos({ (float)getScaledWindowWidth() / 3.f, 0 });
		ImGui::Begin("preview", nullptr);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("cpp"))
			{
				ImGui::Text(getCppString().getRaw());
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("header"))
			{
				ImGui::Text(getHeaderString().getRaw());
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("cmake"))
			{
				ImGui::Text(getCMakeString().getRaw());
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	virtual void onEnd() override
	{
	}
};

int main()
{
	MyGame* mg = new MyGame();
	mg->start(WINDOW_WIDTH, WINDOW_HEIGHT, "Featureset Generator");
	delete mg;
}
