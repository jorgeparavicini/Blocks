// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DxgiInfoManager.h

#pragma once

#include <dxgidebug.h>
#include <string>
#include <vector>
#include <wrl.h>


namespace BlocksEngine
{
    class DxgiInfoManager;
}

class BlocksEngine::DxgiInfoManager
{
    DxgiInfoManager();
public:
    ~DxgiInfoManager() = default;
    DxgiInfoManager(const DxgiInfoManager&) = delete;
    void operator=(const DxgiInfoManager&) = delete;
    DxgiInfoManager(DxgiInfoManager&&) = delete;
    DxgiInfoManager& operator=(DxgiInfoManager&&) = delete;

    static void Set() noexcept;
    [[nodiscard]] static std::vector<std::string> GetMessages();

private:
    static DxgiInfoManager& Instance()
    {
        static DxgiInfoManager instance;
        return instance;
    }

    unsigned long long next_ = 0u;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue_;
};
