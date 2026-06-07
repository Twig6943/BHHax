#include "hook_manager.h"
#include "../Renderer/renderer.h"
#include "../UI/menu.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK HookManager::WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  auto& menu = Menu::GetInstance();

  if (uMsg == WM_KEYDOWN && wParam == VK_F1) {
    menu.Toggle();
    return 0;
  }

  if (menu.IsVisible() && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    return true;

  return CallWindowProc(GetInstance().GetOriginalWndProc(), hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall HookManager::EndSceneHook(LPDIRECT3DDEVICE9 pDevice) {
  auto& renderer = Renderer::GetInstance();
  auto& menu = Menu::GetInstance();

  if (!renderer.IsInitialized()) {
    if (renderer.Initialize(pDevice)) {
      GetInstance().SetOriginalWndProc((WNDPROC)SetWindowLongPtr(
        renderer.GetWindow(), GWLP_WNDPROC, (LONG_PTR)WndProcHook));

      menu.Initialize();
    } else {
      return GetInstance().GetOriginalEndScene()(pDevice);
    }
  }

  renderer.BeginFrame();
  menu.Render();
  renderer.EndFrame();

  return GetInstance().GetOriginalEndScene()(pDevice);
}

void HookManager::Install() {
  if (kiero::bind(42, (void**)&m_originalEndScene, EndSceneHook) != kiero::Status::Success) {
    MessageBoxA(NULL, "failed to hook EndScene", "Error", MB_OK);
  }
}

void HookManager::Uninstall() {
  if (m_originalWndProc) {
    SetWindowLongPtr(Renderer::GetInstance().GetWindow(), GWLP_WNDPROC, (LONG_PTR)m_originalWndProc);
  }

  kiero::unbind(42);
}
