#include "../includes.h"
#include "proxy.h"

static HMODULE g_realDinput8 = nullptr;

bool Proxy_Init(HMODULE hModule) {
  char path[MAX_PATH];
  GetSystemDirectoryA(path, MAX_PATH);
  strcat_s(path, "\\dinput8.dll");
  g_realDinput8 = LoadLibraryA(path);
  return g_realDinput8 != nullptr;
}

void Proxy_Shutdown() {
  if (g_realDinput8) {
    FreeLibrary(g_realDinput8);
    g_realDinput8 = nullptr;
  }
}

extern "C" HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
  if (!g_realDinput8) return E_FAIL;
  FARPROC proc = GetProcAddress(g_realDinput8, "DirectInput8Create");
  if (!proc) return E_FAIL;
  return ((HRESULT(WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN))proc)(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

extern "C" HRESULT WINAPI DllCanUnloadNow() {
  if (!g_realDinput8) return S_FALSE;
  FARPROC proc = GetProcAddress(g_realDinput8, "DllCanUnloadNow");
  return proc ? ((HRESULT(WINAPI*)())proc)() : S_FALSE;
}

extern "C" HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
  if (!g_realDinput8) return CLASS_E_CLASSNOTAVAILABLE;
  FARPROC proc = GetProcAddress(g_realDinput8, "DllGetClassObject");
  return proc ? ((HRESULT(WINAPI*)(REFCLSID, REFIID, LPVOID*))proc)(rclsid, riid, ppv) : CLASS_E_CLASSNOTAVAILABLE;
}

extern "C" HRESULT WINAPI DllRegisterServer() {
  if (!g_realDinput8) return E_FAIL;
  FARPROC proc = GetProcAddress(g_realDinput8, "DllRegisterServer");
  return proc ? ((HRESULT(WINAPI*)())proc)() : E_FAIL;
}

extern "C" HRESULT WINAPI DllUnregisterServer() {
  if (!g_realDinput8) return E_FAIL;
  FARPROC proc = GetProcAddress(g_realDinput8, "DllUnregisterServer");
  return proc ? ((HRESULT(WINAPI*)())proc)() : E_FAIL;
}
