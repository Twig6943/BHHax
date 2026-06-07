#pragma once
#include "../includes.h"

class HookManager {
public:
  static HookManager& GetInstance() {
    static HookManager instance;
    return instance;
  }

  void Install();
  void Uninstall();

  using EndSceneFunction = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);

  EndSceneFunction GetOriginalEndScene() const { return m_originalEndScene; }
  void SetOriginalEndScene(EndSceneFunction present) { m_originalEndScene = present; }

  WNDPROC GetOriginalWndProc() const { return m_originalWndProc; }
  void SetOriginalWndProc(WNDPROC wndproc) { m_originalWndProc = wndproc; }

private:
  HookManager() = default;
  ~HookManager() = default;
  HookManager(const HookManager&) = delete;
  HookManager& operator=(const HookManager&) = delete;

  static LRESULT CALLBACK WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  static HRESULT __stdcall EndSceneHook(LPDIRECT3DDEVICE9 pDevice);

  WNDPROC m_originalWndProc = nullptr;
  EndSceneFunction m_originalEndScene = nullptr;
};