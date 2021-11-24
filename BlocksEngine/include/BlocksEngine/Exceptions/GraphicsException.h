// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GraphicsException.h

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#pragma once
#include <vector>

#include "BlocksEngine/Exceptions/DxgiInfoException.h"
#include "BlocksEngine/Exceptions/Exception.h"

#define GFX_EXCEPT_NOINFO(hr) GraphicsException(__LINE__, __FILE__, hr)
#define GFX_THROW_NOINFO(hrCall) if (FAILED(hr=(hrCall))) throw GraphicsException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) GraphicsException(__LINE__, __FILE__, hr, DxgiInfoManager::GetMessages())
#define GFX_THROW_INFO(hrCall) DxgiInfoManager::Set(); if (FAILED(hr = (hrCall))) throw GFX_EXCEPT(hr)
#else
#define GFX_EXCEPT(hr) GraphicsException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO(hrCall) GFX_THROW_NOINFO(hrCall)
#endif

namespace BlocksEngine
{
    class GraphicsException;
}

class BlocksEngine::GraphicsException : public DxgiInfoException
{
public:
    GraphicsException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] const char* GetType() const noexcept override;
    [[nodiscard]] HRESULT GetErrorCode() const noexcept;
    [[nodiscard]] std::string GetErrorDescription() const noexcept;

private:
    HRESULT hr_;
};
