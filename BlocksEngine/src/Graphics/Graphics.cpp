#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

#pragma comment(lib, "d3d11.lib")

using Microsoft::WRL::ComPtr;


BlocksEngine::Graphics::Graphics(HWND hWnd, const int width, const int height)
    : window_{hWnd},
      width_{std::max(width, 1)},
      height_{std::max(height, 1)},
      featureLevel_{D3D_FEATURE_LEVEL_11_1}
{
    // Create independent resources
    CreateDevice();

    // Create dependent resources
    CreateResources();
}

void BlocksEngine::Graphics::CreateDevice()
{
    HRESULT hr;
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static constexpr D3D_FEATURE_LEVEL FeatureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    GFX_THROW_INFO(D3D11CreateDevice(
        nullptr, // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        FeatureLevels,
        static_cast<UINT>(std::size(FeatureLevels)),
        D3D11_SDK_VERSION,
        pDevice_.ReleaseAndGetAddressOf(),
        &featureLevel_,
        pContext_.ReleaseAndGetAddressOf()
    ));
}

void BlocksEngine::Graphics::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews[]{nullptr};
    pContext_->OMSetRenderTargets(static_cast<UINT>(std::size(nullViews)), nullViews, nullptr);
    pRenderTarget_.Reset();
    pDepthStencilView_.Reset();
    pContext_->Flush();

    const UINT backBufferWidth = static_cast<UINT>(width_);
    const UINT backBufferHeight = static_cast<UINT>(height_);
    constexpr DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    constexpr DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // Render Target View and Depth Stencil View
    constexpr UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one
    if (pSwapChain_)
    {
        DxgiInfoManager::Set();

        const HRESULT hr = pSwapChain_->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight,
                                                      backBufferFormat,
                                                      0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();
            return;
        }
        if (FAILED(hr))
        {
            throw GFX_EXCEPT(hr);
        }
    }
    HRESULT hr;
    // Get the DXGI Device
    ComPtr<IDXGIDevice1> dxgiDevice;
    GFX_THROW_INFO(pDevice_.As(&dxgiDevice));

    // Get the physical GPU
    ComPtr<IDXGIAdapter> dxgiAdapter;
    GFX_THROW_INFO(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

    // Get the factory object
    ComPtr<IDXGIFactory2> dxgiFactory;
    GFX_THROW_INFO(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

    // Swap Chain Descriptor
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = backBufferWidth;
    swapChainDesc.Height = backBufferHeight;
    swapChainDesc.Format = backBufferFormat;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = backBufferCount;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc{};
    fsSwapChainDesc.Windowed = TRUE;

    GFX_THROW_INFO(dxgiFactory->CreateSwapChainForHwnd(
        pDevice_.Get(),
        window_,
        &swapChainDesc,
        &fsSwapChainDesc,
        nullptr,
        pSwapChain_.ReleaseAndGetAddressOf()
    ));

    // This will disable exclusive fullscreen mode
    GFX_THROW_INFO(dxgiFactory->MakeWindowAssociation(window_, DXGI_MWA_NO_ALT_ENTER));


    // Get the back buffer which will be the final 3D render target
    ComPtr<ID3D11Texture2D> backBuffer;
    GFX_THROW_INFO(pSwapChain_->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the render target to use on bind
    GFX_THROW_INFO(
        pDevice_->CreateRenderTargetView(backBuffer.Get(), nullptr, pRenderTarget_.ReleaseAndGetAddressOf()));

    // Allocate a 2D depth/stencil buffer
    const CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1,
                                                 D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    GFX_THROW_INFO(pDevice_->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    const CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    GFX_THROW_INFO(
        pDevice_->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, pDepthStencilView_.
            ReleaseAndGetAddressOf()));
}

void BlocksEngine::Graphics::OnWindowSizeChanged(const int width, const int height)
{
    width_ = std::max(width, 1);
    height_ = std::max(height, 1);

    CreateResources();
}

ID3D11Device& BlocksEngine::Graphics::GetDevice() const noexcept
{
    return *pDevice_.Get();
}

ID3D11DeviceContext& BlocksEngine::Graphics::GetContext() const noexcept
{
    return *pContext_.Get();
}

void BlocksEngine::Graphics::Clear()
{
    // Clear the views
    pContext_->ClearRenderTargetView(pRenderTarget_.Get(), DirectX::Colors::CornflowerBlue);
    pContext_->ClearDepthStencilView(pDepthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    pContext_->OMSetRenderTargets(1, pRenderTarget_.GetAddressOf(), pDepthStencilView_.Get());

    // Set the viewport
    const CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(width_), static_cast<float>(height_));
    pContext_->RSSetViewports(1, &viewport);
}

// Present the back buffer contents to the screen.
void BlocksEngine::Graphics::Present()
{
    DxgiInfoManager::Set();
    const HRESULT hr = pSwapChain_->Present(1, 0);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }

    if (FAILED(hr))
    {
        throw GFX_EXCEPT(hr);
    }
}

void BlocksEngine::Graphics::OnDeviceLost()
{
    // Clean Up Direct3D resources
    pDepthStencilView_.Reset();
    pRenderTarget_.Reset();
    pSwapChain_.Reset();
    pContext_.Reset();
    pDevice_.Reset();

    CreateDevice();
    CreateResources();
}
