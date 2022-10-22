#include "Servers/GuiServer/GuiServer.hpp"

#include "imgui-docking/backends/imgui_impl_glfw.h"
#include "imgui-docking/backends/imgui_impl_opengl3.h"
#include "imgui-docking/imgui.h"

#include <GLFW/glfw3.h>

#include "res/Roboto-Medium.ttf.res.hpp"

#include "Core/Application.hpp"
#include "Core/ProjectSettings.hpp"
#include "ECS/Components/Components.hpp"
#include "Resource/EditorTheme/EditorTheme.hpp"
#include "Resource/ResourceLoader.hpp"

namespace Ease {
GuiServer::GuiServer(Application *app, EngineContext &ctx) : _Application(app), _Context(ctx) {
}
GuiServer::~GuiServer() {
}

void GuiServer::InitGui(GLFWwindow *window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Res::include_res_Roboto_Medium_ttf_data.data(), Res::include_res_Roboto_Medium_ttf_data.size(), 16.f);
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	Reference<Ease::EditorTheme> theme = Ease::ResourceLoader::get_singleton().LoadResource<Ease::EditorTheme>("abs://res/theme.escfg");
	ImGui::GetStyle() = theme->GetStyle();
}

void GuiServer::BeginGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GuiServer::EndGui() {
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GuiServer::BeginWindow(const std::string &title, uint32_t flags /*= 0*/) {
	int guiflags = 0;
	if (flags & WindowFlags_NoResize)
		guiflags |= ImGuiWindowFlags_NoResize;
	if (flags & WindowFlags_NoMove)
		guiflags |= ImGuiWindowFlags_NoMove;
	if (flags & WindowFlags_NoBringToFrontOnFocus)
		guiflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (flags & WindowFlags_NoNavFocus)
		guiflags |= ImGuiWindowFlags_NoNavFocus;
	if (flags & WindowFlags_NoDocking)
		guiflags |= ImGuiWindowFlags_NoDocking;
	if (flags & WindowFlags_NoTitleBar)
		guiflags |= ImGuiWindowFlags_NoTitleBar;
	if (flags & WindowFlags_NoCollapse)
		guiflags |= ImGuiWindowFlags_NoCollapse;
	if (flags & WindowFlags_MenuBar)
		guiflags |= ImGuiWindowFlags_MenuBar;
	if (flags & WindowFlags_NoBackground)
		guiflags |= ImGuiWindowFlags_NoBackground;

	for (auto &[panelTitle, panel] : _panelViews) {
		if (panelTitle == title) {
			if (!panel.visible) {
				return false;
			}
		}
	}
	_panelStack += 1;
	return ImGui::Begin(title.c_str(), nullptr, guiflags);
}
void GuiServer::EndWindow() {
	if (_panelStack >= 1) {
		_panelStack--;
		ImGui::End();
	}
}

void GuiServer::Text(const std::string &text) {
	ImGui::Text("%s", text.c_str());
}

bool GuiServer::Button(const std::string &label, int width /*= 0*/, int height /*= 0*/) {
	return ImGui::Button(label.c_str(), ImVec2(width, height));
}

bool GuiServer::DragFloat(const std::string &label, float &f) {
	return ImGui::DragFloat(label.c_str(), &f);
}

bool GuiServer::Checkbox(const std::string &label, bool &value) {
	return ImGui::Checkbox(label.c_str(), &value);
}

void GuiServer::Separator() {
	ImGui::Separator();
}

void GuiServer::SetNextWindowPos(int x, int y) {
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
}

void GuiServer::SetNextWindowSize(int width, int height) {
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
}

void GuiServer::PushStyleVar(StyleVar var, float value) {
	ImGuiStyleVar styleVar = 0;
	if (var == StyleVar::WindowRounding) {
		styleVar = ImGuiStyleVar_WindowRounding;
	} else {
		Debug::Error("GuiServer: Invalid style var '{}' with type 'float'", (int)var);
	}

	ImGui::PushStyleVar(styleVar, value);
}
void GuiServer::PushStyleVar(StyleVar var, Vec2 value) {
	ImGuiStyleVar styleVar = 0;
	if (var == StyleVar::WindowPadding) {
		styleVar = ImGuiStyleVar_WindowPadding;
	} else {
		Debug::Error("GuiServer: Invalid style var '{}' with type 'Vec2'", (int)var);
	}

	ImGui::PushStyleVar(styleVar, ImVec2(value.x, value.y));
}
void GuiServer::PopStyleVar(int count) {
	ImGui::PopStyleVar(count);
}

void GuiServer::SetupDockspace() {
	ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.f, 0.f), ImGuiDockNodeFlags_PassthruCentralNode);
}

bool GuiServer::BeginMainMenuBar() {
	return ImGui::BeginMainMenuBar();
}

void GuiServer::EndMainMenuBar() {
	ImGui::EndMainMenuBar();
}

bool GuiServer::BeginMenu(const std::string &label) {
	return ImGui::BeginMenu(label.c_str());
}

void GuiServer::EndMenu() {
	ImGui::EndMenu();
}

bool GuiServer::MenuItem(const std::string &label, const std::string &shortcut /*= {""}*/) {
	return ImGui::MenuItem(label.c_str(), shortcut.c_str());
}

bool GuiServer::BeginFooter(const std::string &label) {
	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(
		ImVec2(viewport->Pos.x,
			   viewport->Pos.y + viewport->Size.y - ImGui::GetFrameHeight()));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, ImGui::GetFrameHeight()));
	PushStyleVar(Ease::StyleVar::WindowRounding, 0.f);
	// todo add Gui::PushStyleColor
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.17f, 0.50f, 0.70f, 1.f));	 // (45, 128, 178)
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.20f, 0.55f, 0.75f, 1.f)); // (51, 140, 191)

	bool opened = false;
	opened = ImGui::Begin(label.c_str(), nullptr,
						  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();
	if (!ImGui::BeginMenuBar()) {
		return false;
	}
	return opened;
}

void GuiServer::EndFooter() {
	ImGui::EndMenuBar();
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void GuiServer::RegisterPanel(const std::string &windowTitle, const std::string &header, bool visibleByDefault /*= true*/) {
	_panelViews[windowTitle] = PanelData();
	_panelViews[windowTitle].header = header;
	_panelViews[windowTitle].visible = visibleByDefault;
}

void GuiServer::DrawViewbar() {
	for (auto &[title, panel] : _panelViews) {
		ImGui::Checkbox(title.c_str(), &panel.visible);
	}
}

bool GuiServer::BeginCheckerList(const std::string &id) {
	return ImGui::BeginTable(id.c_str(), 1, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody);
}

void GuiServer::CheckerListNextItem() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
}

void GuiServer::EndCheckerList() {
	ImGui::EndTable();
}

void GuiServer::ShowDemoWindow() {
	ImGui::ShowDemoWindow();
}

bool GuiServer::BeginTree(const std::string &label, uint32_t flags /*= 0*/) {
	ImGuiTreeNodeFlags imflags = ImGuiTreeNodeFlags_None;
	imflags |= ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	if (flags & TreeFlags_Leaf)
		imflags |= ImGuiTreeNodeFlags_Leaf;
	if (!(flags & TreeFlags_SpanMinWidth))
		imflags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	return ImGui::TreeNodeEx(label.c_str(), imflags);
}
void GuiServer::EndTree() {
	ImGui::TreePop();
}

void GuiServer::PushID(const std::string &id) {
	ImGui::PushID(id.c_str());
}
void GuiServer::PopID() {
	ImGui::PopID();
}

void GuiServer::DrawFrame() {
	auto *app = _Context.GetSingleton<Application>(Ease::Server::APPLICATION);
	ImGui::Image((ImTextureID)app->Renderer_GetAlbedoFramebufferID(), ImGui::GetContentRegionAvail());
}

void GuiServer::DrawFilesystem() {
	static std::filesystem::path path = _Context.GetSingleton<Ease::ProjectSettings>(Ease::Server::PROJECTSETTINGS)->projectpath;
	static std::filesystem::path rel_path = "";

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.05f, 0.05f, 0.2f));

	float icon_width = 64;
	float avail_width = ImGui::GetContentRegionAvail().x;
	float padding_x = 8;

	int col_count = std::floor(avail_width / (icon_width + padding_x));
	col_count = std::max(1, col_count);

	ImGui::Text("res://%s", rel_path.string().c_str());

	ImGui::BeginTable("filesystem", col_count);
	if (rel_path != "") {
		ImGui::TableNextColumn();
		ImGui::ImageButton((ImTextureID)_DirectoryTexture->TextureID(), ImVec2(icon_width, icon_width));

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			rel_path = rel_path.parent_path();
		}
		ImGui::Text("../");
	}

	for (auto &dir_entry : std::filesystem::directory_iterator(path / rel_path)) {
		if (dir_entry.is_directory()) {
			ImGui::TableNextColumn();

			ImGui::ImageButton((ImTextureID)_DirectoryTexture->TextureID(), ImVec2(icon_width, icon_width));

			if (ImGui::IsItemHovered()) {
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					rel_path /= dir_entry.path().filename();
			}
			ImGui::Text("%s", dir_entry.path().filename().string().c_str());
		}
	}

	for (auto &dir_entry : std::filesystem::directory_iterator(path / rel_path)) {
		if (!dir_entry.is_directory()) {
			ImGui::TableNextColumn();

			std::string extension = dir_entry.path().extension().string();
			Reference<ImageTexture> fileTex = _FileTexture;
			for (auto it = _FileTextures.begin(); it != _FileTextures.end(); ++it) {
				if ((*it).first == extension) {
					fileTex = (*it).second;
					break;
				}
			}

			ImGui::ImageButton((ImTextureID)fileTex->TextureID(), ImVec2(icon_width, icon_width));
			ImGui::Text("%s", dir_entry.path().filename().string().c_str());
		}
	}
	ImGui::EndTable();

	ImGui::PopStyleColor(3);
}

void GuiServer::DrawPlayButton() {
	auto *app = _Context.GetSingleton<Ease::Application>(Ease::Server::APPLICATION);
	Reference<ImageTexture> tex = app->IsRunning() ? _StopTexture : _PlayTexture;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.2f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.05f, 0.05f, 0.2f));

	if (ImGui::ImageButton((ImTextureID)tex->TextureID(), ImVec2(32.f, 32.f))) {
		if (app->IsRunning())
			app->StopGame();
		else
			app->StartGame();
	}

	ImGui::PopStyleColor(3);
}

// todo: (scripting) add entity iterator
// entity_iterator child = Scene::GetRoot().GetChildren();
// while child != null
// Log child.get().name
// child.next()
void GuiServer::DrawScene() {
	BeginCheckerList("##sceneview");

	auto *app = _Context.GetSingleton<Ease::Application>(Ease::Server::APPLICATION);
	Reference<Ease::Scene> scene = app->GetCurrentScene();

	auto view = scene->m_Registry.view<Component::Common>();
	for (auto it = view.rbegin(); it != view.rend(); ++it) {
		Ease::Entity entity(*it, &scene->m_Registry);

		CheckerListNextItem();
		PushID(std::to_string(entity.ID()));
		if (BeginTree(entity.GetComponent<Component::Common>().Name(), TreeFlags_Leaf)) {
			EndTree();
		}
		PopID();
	}
	EndCheckerList();
}

bool GuiServer::IsWindowHovered() {
	return ImGui::IsWindowHovered() && !ImGui::IsItemHovered();
}

bool GuiServer::IsMouseClicked(GuiMouseButton button) {
	ImGuiMouseButton btn = ImGuiMouseButton_Left;
	if (button == GuiMouseButton::Left) {
		btn = ImGuiMouseButton_Left;
	} else if (button == GuiMouseButton::Right) {
		btn = ImGuiMouseButton_Right;
	} else if (button == GuiMouseButton::Middle) {
		btn = ImGuiMouseButton_Middle;
	}

	return ImGui::IsMouseClicked(btn);
}

bool GuiServer::IsMouseDoubleClicked(GuiMouseButton button) {
	ImGuiMouseButton btn = ImGuiMouseButton_Left;
	if (button == GuiMouseButton::Left) {
		btn = ImGuiMouseButton_Left;
	} else if (button == GuiMouseButton::Right) {
		btn = ImGuiMouseButton_Right;
	} else if (button == GuiMouseButton::Middle) {
		btn = ImGuiMouseButton_Middle;
	}

	return ImGui::IsMouseDoubleClicked(btn);
}

void GuiServer::OpenContextMenu(const std::string &id) {
	ImGui::OpenPopup(id.c_str());
}
bool GuiServer::BeginContextMenu(const std::string &id) {
	return ImGui::BeginPopup(id.c_str());
}
void GuiServer::EndContextMenu() {
	ImGui::EndPopup();
}

} // namespace Ease
