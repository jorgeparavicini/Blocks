//
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: pch.h
//

// ReSharper disable CppClangTidyClangDiagnosticReservedIdMacro
#pragma once

#include <winsdkver.h>
#ifndef _WIN32_WINNT

// ReSharper disable CppInconsistentNaming

// Target windows 10
#define _WIN32_WINNT 0x0A00 // NOLINT(cppcoreguidelines-macro-usage)

// ReSharper restore CppInconsistentNaming

#endif
#include <sdkddkver.h>


// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
//#define NOGDI
#define NOBITMAP

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <d3d11_1.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <dxgi1_2.h>
#include <exception>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <Windows.h>
#include <wrl/client.h>

#include "BlocksEngine/Matrix.h"
#include "BlocksEngine/Plane.h"
#include "BlocksEngine/Quaternion.h"
#include "BlocksEngine/Vector2.h"
#include "BlocksEngine/Vector3.h"
#include "BlocksEngine/Vector4.h"

namespace BlocksEngine::Com
{
    inline void ThrowIfFailed(const HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}
