#include "includes.h"

DWORD WINAPI MainThread(LPVOID lpReserved) {
  bool init_hook = false;

  do {
    auto status = kiero::init(kiero::RenderType::D3D9);
    if (status == kiero::Status::Success) {
      HookManager::GetInstance().Install();
      init_hook = true;
    } else {
      Sleep(100);
    }
  } while (!init_hook);

  return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
  switch (dwReason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hMod);
      if (!Proxy_Init(hMod))
        return FALSE;
      CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
      break;

    case DLL_PROCESS_DETACH:
      kiero::shutdown();
      Proxy_Shutdown();
      break;
  }
  return TRUE;
}
