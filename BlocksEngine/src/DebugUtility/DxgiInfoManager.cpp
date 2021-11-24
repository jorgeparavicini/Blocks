#include "BlocksEngine/pch.h"
#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"

#include <BlocksEngine/Exceptions/WindowException.h>

#include "BlocksEngine/Exceptions/GraphicsException.h"

#pragma comment(lib, "dxguid.lib")

BlocksEngine::DxgiInfoManager::DxgiInfoManager()
{
    using DxgiGetDebugInterface = HRESULT(WINAPI*)(REFIID, void**);
    const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hModDxgiDebug == nullptr)
    {
        throw WND_LAST_EXCEPT();
    }

    const auto dxgiGetDebugInterface = reinterpret_cast<DxgiGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(
        hModDxgiDebug, "DXGIGetDebugInterface")));

    if (dxgiGetDebugInterface == nullptr)
    {
        throw WND_LAST_EXCEPT();
    }

    HRESULT hr;
    GFX_THROW_NOINFO(dxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue_));
}

void BlocksEngine::DxgiInfoManager::Set() noexcept
{
    Instance().next_ = Instance().pDxgiInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> BlocksEngine::DxgiInfoManager::GetMessages()
{
    std::vector<std::string> messages;
    const auto end = Instance().pDxgiInfoQueue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (auto i = Instance().next_; i < end; i++)
    {
        HRESULT hr;
        SIZE_T messageLength{};
        // Get the size of the message i in bytes
        GFX_THROW_NOINFO(Instance().pDxgiInfoQueue_->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

        // Allocate memory for message
        auto bytes = std::make_unique<byte[]>(messageLength);
        const auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

        // Get the message and push its description into the result vector
        GFX_THROW_NOINFO(Instance().pDxgiInfoQueue_->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
        messages.emplace_back(static_cast<std::string>(pMessage->pDescription));
    }

    return messages;
}
