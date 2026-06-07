#include "renderer.h"

bool Renderer::Initialize(LPDIRECT3DDEVICE9 pDevice) {
  if (m_initialized) return true;

  m_pDevice = pDevice;

  D3DDEVICE_CREATION_PARAMETERS params;
  if (SUCCEEDED(m_pDevice->GetCreationParameters(&params)))
    m_window = params.hFocusWindow;

  m_initialized = true;
  return true;
}

void Renderer::Cleanup() {
  if (m_pDevice) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    m_pDevice = nullptr;
  }
  m_initialized = false;
}

void Renderer::OnLostDevice() {
  ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Renderer::OnReset() {
  ImGui_ImplDX9_CreateDeviceObjects();
}

void Renderer::BeginFrame() {
  ImGui_ImplDX9_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void Renderer::EndFrame() {
  ImGui::Render();
  ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
