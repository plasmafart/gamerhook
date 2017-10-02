/*
 * hooks.h
 *
 *  Created on: Jan 8, 2017
 *      Author: nullifiedcat
 */

#ifndef HOOKEDMETHODS_H_
#define HOOKEDMETHODS_H_

#include "../common.h"

typedef bool(*CreateMove_t)(void*, float, CUserCmd*);
typedef void(*PaintTraverse_t)(void*, unsigned int, bool, bool);
typedef bool(*CanPacket_t)(void*);
typedef int(*IN_KeyEvent_t)(void*, int, int, const char*);
typedef bool(*SendNetMsg_t)(void*, INetMessage&, bool, bool);
typedef void(*Shutdown_t)(void*, const char*);
typedef void(*OverrideView_t)(void*, CViewSetup*);
typedef bool(*DispatchUserMessage_t)(void*, int, bf_read&);
typedef void(*FrameStageNotify_t)(void*, int);
typedef void(*LevelInit_t)(void*, const char*);
typedef void(*LevelShutdown_t)(void*);
typedef void(*BeginFrame_t)(IStudioRender*);
typedef bool(*CanInspect_t)(IClientEntity*);
typedef void(*DrawModelExecute_t)(IVModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
typedef CUserCmd*(*GetUserCmd_t)(IInput*, int);
typedef const char*(*GetClientName_t)(CBaseClientState*);
typedef bool(*ProcessSetConVar_t)(CBaseClientState*, NET_SetConVar*);
typedef bool(*ProcessGetCvarValue_t)(CBaseClientState*, SVC_GetCvarValue*);
const char* GetClientName_hook(CBaseClientState* _this);
bool ProcessSetConVar_hook(CBaseClientState* _this, NET_SetConVar* msg);
bool ProcessGetCvarValue_hook(CBaseClientState* _this, SVC_GetCvarValue* msg);
//typedef void(*CInput__CreateMove_t)(void*, int, float, bool);
//void CInput__CreateMove_hook(void*, int sequence_number, float input_sample_frametime, bool active);
typedef const char*(*GetFriendPersonaName_t)(ISteamFriends*, CSteamID);
const char* GetFriendPersonaName_hook(ISteamFriends* _this, CSteamID steamID);

CUserCmd* GetUserCmd_hook(IInput*, int);
void DrawModelExecute_hook(IVModelRender* _this, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix);

#include "CreateMove.h"
#include "PaintTraverse.h"
#include "others.h"

#endif /* HOOKEDMETHODS_H_ */
