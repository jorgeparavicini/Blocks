// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: WindowException.h

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#pragma once
#include "Exception.h"

#define WND_EXCEPT(hr) WindowException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() WindowException(__LINE__, __FILE__, static_cast<HRESULT>(GetLastError()))

namespace BlocksEngine
{
    class WindowException;
}

class BlocksEngine::WindowException final : public Exception
{
public:
    WindowException(int line, const char* file, HRESULT hr) noexcept;
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] const char* GetType() const noexcept override;
    [[nodiscard]] HRESULT GetErrorCode() const noexcept;
    [[nodiscard]] std::string GetErrorDescription() const noexcept;

private:
    HRESULT hr_;
};
