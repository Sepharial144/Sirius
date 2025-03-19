
#include "logger/logger.hpp"
//#include "D3DContext.hpp"

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

#include <cstdint>

#include "exception/exception.hpp"

using Microsoft::WRL::ComPtr;

int32_t main()
{
    HRESULT ret;
    // Setup swap chain

    UINT FrameCount = 2;
    UINT FrameIndex = 0;
    UINT rtvDescriptorSize = 0;
    int width = 900;
    int height = 900;
    ComPtr<ID3D12Resource> renderTargets[FrameCount];

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

    UINT dxgiFactoryFlags = 0;


    // [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
    ComPtr<ID3D12Debug> debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
    {
        debugController->EnableDebugLayer();
        // Enable additional debug layers.
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif
    // Create factory
    ComPtr<IDXGIFactory4> factory;
    if (ret = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(factory.GetAddressOf())) != S_OK)
        Win32::throw_exception("CreateDXGIFactory2: error", ret);

    // Create device
    // https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/Samples/UWP/D3D12HelloWorld/src/HelloTriangle/D3D12HelloTriangle.cpp
    // contain another implementation with enum devices
    ComPtr<ID3D12Device> device;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    if (ret = D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(device.GetAddressOf())) != S_OK) // nullptr - use default adapter first in list
        Win32::throw_exception("D3D12CreateDevice: error", ret);


    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 1;
    
    ComPtr<ID3D12CommandQueue> commandQueue;
    if (ret = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf())) != S_OK)
        Win32::throw_exception("CreateCommandQueue: error", ret);

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    // From this place it goes weird wrong
    ComPtr<IDXGISwapChain1> swapChain;
    // it could be a current window, so you can provide another impl
    if (ret = factory->CreateSwapChainForCoreWindow(
        commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        reinterpret_cast<IUnknown*>(Windows::UI::Core::CoreWindow::GetForCurrentThread()), &swapChainDesc, nullptr, &swapChain) != S_OK) // here is maybe a hwnd window as buffer of swap chain
        Win32::throw_exception("CreateSwapChainForCoreWindow: error", ret);

    /*
    ComPtr<IDXGISwapChain3> swapChain;
    HWND = window_handle = nullptr;
    if (ret = factory->CreateSwapChainForHwnd(commandQueue.Get(), window_handle, &sd, nullptr, nullptr, &swapChain) != S_OK)
        Win32::throw_exception("CreateSwapChainForHwnd: error", ret); 
    */

    FrameIndex = swapChain->GetCurrentBackBufferIndex();


    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (ret = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap.GetAddressOf())) != S_OK)
            Win32::throw_exception("CreateDescriptorHeap: error", ret);

        rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < FrameCount; n++)
        {
            if (ret = swapChain->GetBuffer(n, IID_PPV_ARGS(&renderTargets[n])) != S_OK)
                Win32::throw_exception("GetBuffer: error", ret); 
            device->CreateRenderTargetView(renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, rtvDescriptorSize);
        }
    }

    ComPtr<ID3D12CommandAllocator> commandAllocator;
    if (ret = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf())) != S_OK)
        Win32::throw_exception("CreateCommandAllocator: error", ret);

    return 0;
}