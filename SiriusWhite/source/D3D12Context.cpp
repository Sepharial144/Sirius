#include "D3D12Context.hpp"
#include "imgui_impl_dx12.h"

D3D12Context::D3D12Context()
{
}

D3D12Context::~D3D12Context()
{
    CleanupRenderTarget();
    m_pSwapChain->SetFullscreenState(false, nullptr);
    if (m_hSwapChainWaitableObject != nullptr)
        ::CloseHandle(m_hSwapChainWaitableObject);

    for (UINT i = 0; i < APP_NUM_FRAMES_IN_FLIGHT; i++)
        if (g_frameContext[i].CommandAllocator) { g_frameContext[i].CommandAllocator->Release(); g_frameContext[i].CommandAllocator = nullptr; }

    if (m_fenceEvent)
        ::CloseHandle(m_fenceEvent);

#ifdef DX12_ENABLE_DEBUG_LAYER
    IDXGIDebug1* pDebug = nullptr;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
    {
        pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
        pDebug->Release();
    }
#endif
}

unsigned long D3D12Context::Release() { return 0ul; }

void D3D12Context::CleanupRenderTarget()
{
    WaitForLastSubmittedFrame();

    for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
        if (m_mainRenderTargetResource[i]) { m_mainRenderTargetResource[i]->Release(); m_mainRenderTargetResource[i] = nullptr; }
}

_Templ_spec_ D3D12Context::Get<ID3D12Device>()       -> ID3D12Device* { return m_pd3dDevice.Get(); }
_Templ_spec_ D3D12Context::Get<ID3D12CommandQueue>() -> ID3D12CommandQueue* { return m_pd3dCommandQueue.Get(); }
_Templ_spec_ D3D12Context::Get<ID3D12GraphicsCommandList>() -> ID3D12GraphicsCommandList* { return m_pd3dCommandList.Get(); }
_Templ_spec_ D3D12Context::Get<IDXGISwapChain3>() -> IDXGISwapChain3* { return m_pSwapChain.Get(); }
_Templ_spec_ D3D12Context::Get<ID3D12Fence>() -> ID3D12Fence* { return m_fence.Get(); }

auto D3D12Context::GetSrvHeap() -> ID3D12DescriptorHeap* { return m_pd3dSrvDescHeap.Get(); }
auto D3D12Context::GetSrvHeapAlloc() -> ExampleDescriptorHeapAllocator* { return m_pd3dSrvDescHeapAlloc.get(); }
bool D3D12Context::isSwapChainOccluded() const { return m_SwapChainOccluded; }
void D3D12Context::setSwapChainOccluded(bool value) { m_SwapChainOccluded = value; } //TODO: need to delete this function because it dont use anymore


// TODO: Maybe threre is no reason to use raw pointers here, 
// instead use Microsoft::WRL::ComPtr for better compatibility
void D3D12Context::initialize(HWND window_handle)
{

    HRESULT ret;
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC1 sd;
    {
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = APP_NUM_BACK_BUFFERS;
        sd.Width = 0;
        sd.Height = 0;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.Stereo = FALSE;
    }

    // [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
    ComPtr<ID3D12Debug> ptrdx12Debug = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(ptrdx12Debug.GetAddressOf()))))
        ptrdx12Debug->EnableDebugLayer();
#endif
    /*
    // Create factory
    if (ret = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_factory)) != S_OK)
        Win32::throw_exception("CreateDXGIFactory2: error", ret);
    */

    // Create device
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    // !!!! WARNING !!!! Maybe it should a problem to get void** device
    if (ret = D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(m_pd3dDevice.GetAddressOf())) != S_OK) // nullptr - use default adapter first in list
        Win32::throw_exception("D3D12CreateDevice: error", ret);

    // [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
    if (ptrdx12Debug.Get() != nullptr)
    {
        ComPtr<ID3D12InfoQueue> ptrInfoQueue = nullptr;
        m_pd3dDevice->QueryInterface(IID_PPV_ARGS(ptrInfoQueue.GetAddressOf()));
        ptrInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        ptrInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        ptrInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
    }
#endif

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = APP_NUM_BACK_BUFFERS;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 1;
        if (ret = m_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_pd3dRtvDescHeap.GetAddressOf())) != S_OK)
            Win32::throw_exception("CreateDescriptorHeap: error", ret);

        // Create render target view
        SIZE_T rtvDescriptorSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
        {
            m_mainRenderTargetDescriptor[i] = rtvHandle;
            rtvHandle.ptr += rtvDescriptorSize;
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = APP_SRV_HEAP_SIZE;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (ret = m_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_pd3dSrvDescHeap.GetAddressOf())) != S_OK)
            Win32::throw_exception("CreateDescriptorHeap2: error", ret);

        m_pd3dSrvDescHeapAlloc->Create(m_pd3dDevice.Get(), m_pd3dSrvDescHeap.Get());
    }

    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 1;
        if (ret = m_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pd3dCommandQueue.GetAddressOf())) != S_OK)
            Win32::throw_exception("CreateCommandQueue: error", ret);
    }

    for (UINT i = 0; i < APP_NUM_FRAMES_IN_FLIGHT; i++)
        if (ret = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
            Win32::throw_exception("CreateCommandAllocator: error", ret);

    if (ret = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&m_pd3dCommandList)) != S_OK ||
        m_pd3dCommandList->Close() != S_OK)
        Win32::throw_exception("CreateCommandList: error", ret); // TODO: need to delete ret because it is useles here or add impl for second condition expression

    if (ret = m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)) != S_OK)
        Win32::throw_exception("CreateFence: error", ret);

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
        Win32::throw_exception("CreateEvent: error", ret); // TODO: need to delete ret because it is useless here

    {
        // TODO: maube better to use ComPtr instead call ptr->Release()
        IDXGIFactory4* dxgiFactory = nullptr;
        IDXGISwapChain1* swapChain1 = nullptr;
        if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
            Win32::throw_exception("CreateFence: error", ret);

        if (ret = dxgiFactory->CreateSwapChainForHwnd(m_pd3dCommandQueue.Get(), window_handle, &sd, nullptr, nullptr, &swapChain1) != S_OK)
            Win32::throw_exception("CreateFence: error", ret);

        if (ret = swapChain1->QueryInterface(IID_PPV_ARGS(m_pSwapChain.GetAddressOf())) != S_OK)
            Win32::throw_exception("QueryInterface: error", ret);
        swapChain1->Release();
        dxgiFactory->Release();
        m_pSwapChain->SetMaximumFrameLatency(APP_NUM_BACK_BUFFERS);
        m_hSwapChainWaitableObject = m_pSwapChain->GetFrameLatencyWaitableObject();
    }

    CreateRenderTarget();
}

void D3D12Context::CreateRenderTarget()
{
    for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
    {
        ID3D12Resource* pBackBuffer = nullptr;
        m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, m_mainRenderTargetDescriptor[i]);
        m_mainRenderTargetResource[i] = pBackBuffer;
    }
}

void D3D12Context::WaitForLastSubmittedFrame()
{
    FrameContext* frameCtx = &g_frameContext[g_frameIndex % APP_NUM_FRAMES_IN_FLIGHT];

    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue == 0)
        return; // No fence was signaled

    frameCtx->FenceValue = 0;
    if (m_fence->GetCompletedValue() >= fenceValue)
        return;

    m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
    ::WaitForSingleObject(m_fenceEvent, INFINITE);
}

auto D3D12Context::WaitForNextFrameResources() -> FrameContext*
{
    UINT nextFrameIndex = g_frameIndex + 1;
    g_frameIndex = nextFrameIndex;

    HANDLE waitableObjects[] = { m_hSwapChainWaitableObject, nullptr };
    DWORD numWaitableObjects = 1;

    FrameContext* frameCtx = &g_frameContext[nextFrameIndex % APP_NUM_FRAMES_IN_FLIGHT];
    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue != 0) // means no fence was signaled
    {
        frameCtx->FenceValue = 0;
        m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
        waitableObjects[1] = m_fenceEvent;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return frameCtx;
}

void D3D12Context::Rendering(const ImVec4& clear_color)
{
    FrameContext* frameCtx = WaitForNextFrameResources();
    UINT backBufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();
    frameCtx->CommandAllocator->Reset();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_mainRenderTargetResource[backBufferIdx];
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    m_pd3dCommandList->Reset(frameCtx->CommandAllocator, nullptr);
    m_pd3dCommandList->ResourceBarrier(1, &barrier);

    // Render Dear ImGui graphics
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    m_pd3dCommandList->ClearRenderTargetView(m_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, nullptr);
    m_pd3dCommandList->OMSetRenderTargets(1, &m_mainRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
    m_pd3dCommandList->SetDescriptorHeaps(1, m_pd3dSrvDescHeap.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_pd3dCommandList.Get());
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_pd3dCommandList->ResourceBarrier(1, &barrier);
    m_pd3dCommandList->Close();

    m_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)m_pd3dCommandList.GetAddressOf());

    // Present
    HRESULT hr = m_pSwapChain->Present(1, 0);   // Present with vsync
    //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
    m_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

    UINT64 fenceValue = m_fenceLastSignaledValue + 1ul;
    m_pd3dCommandQueue->Signal(m_fence.Get(), fenceValue);
    m_fenceLastSignaledValue = fenceValue;
    frameCtx->FenceValue = fenceValue;
}