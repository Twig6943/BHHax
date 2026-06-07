#pragma once
#include <Windows.h>

#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "kiero.h"

#include "Renderer/renderer.h"
#include "UI/menu.h"

#include "Proxy/proxy.h"
#include "Hooks/hook_manager.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

typedef HRESULT(__stdcall* EndSceneFunc)(LPDIRECT3DDEVICE9 pDevice);
typedef uintptr_t PTR;