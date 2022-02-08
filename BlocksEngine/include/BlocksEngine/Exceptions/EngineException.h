// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: EngineException.h

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
#pragma once
#include "BlocksEngine/Exceptions/Exception.h"

#define ENGINE_EXCEPTION(msg) EngineException(__LINE__, __FILE__, msg)

namespace BlocksEngine
{
    class EngineException;
}

class BlocksEngine::EngineException : public Exception
{
public:
    EngineException(int line, std::string file, std::string msg) noexcept;
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] const char* GetType() const noexcept override;

private:
    std::string msg_;
};
