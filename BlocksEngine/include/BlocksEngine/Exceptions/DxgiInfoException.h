// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DxgiInfoException.h

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#pragma once
#include <vector>

#include "BlocksEngine/Exceptions/Exception.h"

#ifndef NDEBUG
#define GFX_THROW_INFO_ONLY(call) DxgiInfoManager::Set(); (call); {auto v = DxgiInfoManager::GetMessages(); if (!v.empty()) {throw GraphicsException(__LINE__, __FILE__, v); }}
#else
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

namespace BlocksEngine
{
    class DxgiInfoException;
}

class BlocksEngine::DxgiInfoException : public Exception
{
public:
    DxgiInfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] const char* GetType() const noexcept override;
    [[nodiscard]] std::string GetErrorInfo() const noexcept;

private:
    std::string info_;
};
