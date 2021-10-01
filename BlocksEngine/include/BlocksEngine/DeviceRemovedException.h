// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DeviceRemovedException.h

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#pragma once
#include "BlocksEngine/GraphicsException.h"

#ifndef NDEBUG
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException(__LINE__, __FILE__, hr, DxgiInfoManager.GetMessages())
#else
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException(__LINE__, __FILE__, hr)
#endif

namespace BlocksEngine
{
    class DeviceRemovedException;
}

class BlocksEngine::DeviceRemovedException final : public GraphicsException
{
public:
    DeviceRemovedException(int line, const char* file, HRESULT hr, const std::vector<std::string>& basicStrings);

    [[nodiscard]] const char* GetType() const noexcept override;
};
