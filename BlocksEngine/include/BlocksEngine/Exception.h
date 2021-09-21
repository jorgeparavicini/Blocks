// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Exception.h

#pragma once
#include <exception>
#include <string>

namespace BlocksEngine
{
    class Exception;
}

class BlocksEngine::Exception : public std::exception
{
public:
    Exception(int line, std::string file) noexcept;
    [[nodiscard]] const char* what() const noexcept override;
    [[nodiscard]] virtual const char* GetType() const noexcept;
    [[nodiscard]] int GetLine() const noexcept;
    [[nodiscard]] const std::string& GetFile() const noexcept;
    [[nodiscard]] std::string GetOriginString() const noexcept;

protected:
    mutable std::string whatBuffer_;

    // Make virtual if a subclass needs custom translation techniques.
    static std::string TranslateErrorCode(HRESULT hr) noexcept;

private:
    int line_;
    std::string file_;
};
