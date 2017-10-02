/*
 * PaintTraverse.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: nullifiedcat
 */

#include "PaintTraverse.h"
#include "../common.h"
#include "../hack.h"
#include "hookedmethods.h"
#include "../gui/GUI.h"
#include "../segvcatch/segvcatch.h"
#include "../copypasted/CSignature.h"
#include "../profiler.h"
#include "../netmessage.h"

CatVar clean_screenshots(CV_SWITCH, "clean_screenshots", "1", "Clean screenshots", "Don't draw visuals while taking a screenshot");
CatVar disable_visuals(CV_SWITCH, "no_visuals", "0", "Disable ALL drawing", "Completely hides cathook");
CatVar no_zoom(CV_SWITCH, "no_zoom", "0", "Disable scope", "Disables black scope overlay");
CatVar info_text(CV_SWITCH, "info", "1", "Show info", "Show cathook version in top left corner");
CatVar pure_bypass(CV_SWITCH, "pure_bypass", "0", "Pure Bypass", "Bypass sv_pure");
void* pure_orig = nullptr;
void** pure_addr = nullptr;

CatEnum software_cursor_enum({"KEEP", "ALWAYS", "NEVER", "MENU ON", "MENU OFF"});
CatVar software_cursor_mode(software_cursor_enum, "software_cursor_mode", "0", "Software cursor", "Try to change this and see what works best for you");

void PaintTraverse_hook(void* _this, unsigned int vp, bool fr, bool ar) {
	static const PaintTraverse_t original = (PaintTraverse_t)hooks::panel.GetMethod(offsets::PaintTraverse());
#if DEBUG_SEGV == true
	if (!segvcatch::handler_fpe || !segvcatch::handler_segv) {
		if (!segvcatch::handler_fpe) segvcatch::init_segv();
		if (!segvcatch::handler_segv) segvcatch::init_fpe();
	}
#endif
	SEGV_BEGIN;
	static bool textures_loaded = false;
	if (!textures_loaded) {
		textures_loaded = true;
		hacks::tf::radar::Init();
	}
	if (pure_bypass) {
		if (!pure_addr) {
			pure_addr = *reinterpret_cast<void***>(gSignatures.GetEngineSignature("55 89 E5 83 EC 18 A1 ? ? ? ? 89 04 24 E8 0D FF FF FF A1 ? ? ? ? 85 C0 74 08 89 04 24 E8 ? ? ? ? C9 C3") + 7);
		}
		if (*pure_addr)
			pure_orig = *pure_addr;
		*pure_addr = (void*)0;
	} else if (pure_orig) {
		*pure_addr = pure_orig;
		pure_orig = (void*)0;
	}
	static unsigned long panel_focus = 0;
	static unsigned long panel_scope = 0;
	static unsigned long panel_top = 0;
	static bool draw_flag = false;
	bool call_default = true;
	if (cathook && panel_scope && no_zoom && vp == panel_scope) call_default = false;
	/*if (cathook) {
		bool vis = gui_visible;
		g_ISurface->SetCursorAlwaysVisible(vis);
	}*/

	if (software_cursor_mode) {
		static ConVar* software_cursor = g_ICvar->FindVar("cl_software_cursor");
		bool cur = software_cursor->GetBool();
		switch ((int)software_cursor_mode) {
		case 1:
			if (!software_cursor->GetBool()) software_cursor->SetValue(1);
			break;
		case 2:
			if (software_cursor->GetBool()) software_cursor->SetValue(0);
			break;
		case 3:
			if (cur != g_pGUI->Visible()) {
				software_cursor->SetValue(g_pGUI->Visible());
			}
			break;
		case 4:
			if (cur == g_pGUI->Visible()) {
				software_cursor->SetValue(!g_pGUI->Visible());
			}
		}
	}

	if (call_default) SAFE_CALL(original(_this, vp, fr, ar));
	// To avoid threading problems.

	PROF_SECTION(PaintTraverse);
	if (vp == panel_top) draw_flag = true;
	if (!cathook) return;

	if (!panel_top) {
		const char* name = g_IPanel->GetName(vp);
		if (strlen(name) > 4) {
			if (name[0] == 'M' && name[3] == 'S') {
				panel_top = vp;
			}
			if (name[0] == 'F' && name[5] == 'O') {
				panel_focus = vp;
			}
		}
	}
	if (!panel_scope) {
		if (!strcmp(g_IPanel->GetName(vp), "HudScope")) {
			panel_scope = vp;
		}
	}
	if (!g_IEngine->IsInGame()) {
		g_Settings.bInvalid = true;
	}

	ResetStrings();

	if (vp != panel_focus) return;
	g_IPanel->SetTopmostPopup(panel_focus, true);
	if (!draw_flag) return;
	draw_flag = false;

	{
		std::lock_guard<std::mutex> guard(hack::command_stack_mutex);
		while (!hack::command_stack().empty()) {
			logging::Info("executing %s", hack::command_stack().top().c_str());
			g_IEngine->ClientCmd_Unrestricted(hack::command_stack().top().c_str());
			hack::command_stack().pop();
		}
	}

	if (disable_visuals) return;

	if (clean_screenshots && g_IEngine->IsTakingScreenshot()) return;

	if (info_text) {
		AddSideString("cathook by nullifiedcat", colors::RainbowCurrent());
#if defined(GIT_COMMIT_HASH) && defined(GIT_COMMIT_DATE)
		AddSideString("Version: #" GIT_COMMIT_HASH " " GIT_COMMIT_DATE, GUIColor());
#endif
		AddSideString("Press 'INSERT' key to open/close cheat menu.", GUIColor());
		AddSideString("Use mouse to navigate in menu.", GUIColor());
		if (!g_IEngine->IsInGame() || g_pGUI->Visible()) {
			std::string name(force_name.GetString());
			if (name.length() < 3) name = "*Not Set*";
			AddSideString(""); // foolish
			std::string name_stripped(name); // RIP fps
			ReplaceString(name_stripped, "\n", "\\n");
			std::string reason_stripped(disconnect_reason.GetString());
			ReplaceString(reason_stripped, "\n", "\\n");
			AddSideString(format("Custom Name: ", name_stripped), GUIColor());
			AddSideString(format("Custom Disconnect Reason: ", (reason_stripped.length() > 3 ? reason_stripped : "*Not Set*")), GUIColor());
		}
	}

	if (CE_GOOD(g_pLocalPlayer->entity) && !g_Settings.bInvalid) {
		if (TF) SAFE_CALL(hacks::tf2::antidisguise::Draw());
		SAFE_CALL(hacks::shared::misc::Draw());
		SAFE_CALL(hacks::shared::esp::Draw());
		if (TF) SAFE_CALL(hacks::tf::spyalert::Draw());
		if (TF) SAFE_CALL(hacks::tf::radar::Draw());
		if (TF2) SAFE_CALL(hacks::tf2::skinchanger::PaintTraverse());
	}


#if GUI_ENABLED == true
		g_pGUI->Update();
#endif

	DrawStrings();
	SEGV_END;
}

