#pragma once
#include "../includes.h"

class Renderer {
public:
  static Renderer& GetInstance() {
    static Renderer instance;
    return instance;
  }

  bool Initialize(LPDIRECT3DDEVICE9 pDevice);
  void Cleanup();
  void OnLostDevice();
  void OnReset();

  LPDIRECT3DDEVICE9 GetDevice() const { return m_pDevice; }
  HWND GetWindow() const { return m_window; }
  bool IsInitialized() const { return m_initialized; }

  void BeginFrame();
  void EndFrame();

private:
  Renderer() = default;
  ~Renderer() { Cleanup(); }

  LPDIRECT3DDEVICE9 m_pDevice = nullptr;
  HWND m_window = nullptr;
  bool m_initialized = false;
};