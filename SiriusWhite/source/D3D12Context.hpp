#pragma once

#include "imgui.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#include <wrl.h>
#include "Windows.h"
#include "DescHeapAllocator.hpp"
#include "exception/exception.hpp"

#include <memory>


// Config for example app
static const int APP_NUM_FRAMES_IN_FLIGHT = 3;
static const int APP_NUM_BACK_BUFFERS = 3;
static const int APP_SRV_HEAP_SIZE = 64;

struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                      FenceValue;
};

// Data
static FrameContext                 g_frameContext[APP_NUM_FRAMES_IN_FLIGHT] = {};
static UINT                         g_frameIndex = 0;

/*
static ID3D12Device* g_pd3dDevice = nullptr;
static ID3D12DescriptorHeap* g_pd3dRtvDescHeap = nullptr;
static ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
static ExampleDescriptorHeapAllocator g_pd3dSrvDescHeapAlloc;
static ID3D12CommandQueue* g_pd3dCommandQueue = nullptr;
static ID3D12GraphicsCommandList* g_pd3dCommandList = nullptr;
static ID3D12Fence* g_fence = nullptr;
static HANDLE                       g_fenceEvent = nullptr;
static UINT64                       g_fenceLastSignaledValue = 0;
static IDXGISwapChain3* g_pSwapChain = nullptr;
static bool                         g_SwapChainOccluded = false;
static HANDLE                       g_hSwapChainWaitableObject = nullptr;
static ID3D12Resource* g_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[APP_NUM_BACK_BUFFERS] = {};
*/

using namespace Microsoft::WRL;

#define _Templ_spec_ template<> auto

class D3D12Context
{
public:
    explicit D3D12Context();
    virtual  ~D3D12Context();
    unsigned long Release();

    void initialize(HWND window_handle);
    void CreateRenderTarget();
    void CleanupRenderTarget();
    void WaitForLastSubmittedFrame();

    template<class D3D12ClassType>
    auto Get() -> D3D12ClassType*;

    auto GetSrvHeap() -> ID3D12DescriptorHeap*;
    auto GetSrvHeapAlloc() -> ExampleDescriptorHeapAllocator*;
    bool isSwapChainOccluded() const;

    void setSwapChainOccluded(bool value);

    auto WaitForNextFrameResources() -> FrameContext*;

    void Rendering(const ImVec4& clear_color);

private:
    ComPtr<IDXGIFactory4> m_factory; // cause error in create device
    ComPtr<ID3D12Device> m_pd3dDevice;
    ComPtr<ID3D12DescriptorHeap> m_pd3dRtvDescHeap;
    ComPtr<ID3D12DescriptorHeap> m_pd3dSrvDescHeap;
    std::unique_ptr<ExampleDescriptorHeapAllocator> m_pd3dSrvDescHeapAlloc = std::make_unique<ExampleDescriptorHeapAllocator>();
    ComPtr<ID3D12CommandQueue>  m_pd3dCommandQueue;
    ComPtr<ID3D12GraphicsCommandList> m_pd3dCommandList;
    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_fenceEvent = nullptr;
    UINT64 m_fenceLastSignaledValue = 0ul;
    ComPtr<IDXGISwapChain3> m_pSwapChain;
    bool m_SwapChainOccluded = false;
    HANDLE m_hSwapChainWaitableObject = nullptr;
    ID3D12Resource* m_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = { 0ul };
    D3D12_CPU_DESCRIPTOR_HANDLE  m_mainRenderTargetDescriptor[APP_NUM_BACK_BUFFERS] = { 0ul };
};
