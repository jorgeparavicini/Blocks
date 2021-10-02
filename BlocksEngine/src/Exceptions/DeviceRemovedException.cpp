#include "BlocksEngine/pch.h"
#include "BlocksEngine/DeviceRemovedException.h"

BlocksEngine::DeviceRemovedException::DeviceRemovedException(const int line, const char* file, HRESULT hr,
                                                             const std::vector<std::string>& basicStrings):
    GraphicsException{line, file, hr, basicStrings}
{
}

const char* BlocksEngine::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
