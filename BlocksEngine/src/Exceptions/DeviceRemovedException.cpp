#include "BlocksEngine/pch.h"
#include "BlocksEngine/DeviceRemovedException.h"

const char* BlocksEngine::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
