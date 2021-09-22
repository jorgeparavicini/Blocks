#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

using Microsoft::WRL::ComPtr;

BlocksEngine::Graphics::Graphics(HWND hWnd)
    : featureLevel_{D3D_FEATURE_LEVEL_11_1}
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

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    GFX_THROW_INFO(D3D11CreateDevice(
        nullptr, // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        FeatureLevels,
        std::size(FeatureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),
        &featureLevel_,
        context.ReleaseAndGetAddressOf()
    ));
}

void BlocksEngine::Graphics::CreateResources()
{
    TODO_IMPLEMENT_ME();
}
