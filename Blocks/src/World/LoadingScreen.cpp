#include "Blocks/pch.h"
#include "Blocks/LoadingScreen.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/Game.h"
#include "BlocksEngine/GraphicsException.h"

using namespace BlocksEngine;
using namespace Blocks;

LoadingScreen::LoadingScreen(std::weak_ptr<Actor> actor)
    : Component{std::move(actor)}
{
    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();
    HRESULT hr;
    GFX_THROW_INFO(renderTarget.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue, 1.0f), &pBrush_));
    GFX_THROW_INFO(renderTarget.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &pTextBrush_));

    IDWriteFactory& writeFactory = GetGame()->Graphics().GetDWriteFactory();
    GFX_THROW_INFO(
        writeFactory.CreateTextFormat(
            L"Verdana",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            50,
            L"",
            &pTextFormat_));
    pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void LoadingScreen::LevelLoaded()
{
    isLoading_ = false;
}

void LoadingScreen::Draw2D()
{
    if (!isLoading_) return;

    const std::wstring message{L"World is loading"};

    const auto size = GetGame()->Graphics().Size();
    const D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));

    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();
    renderTarget.FillRectangle(&rect, pBrush_.Get());

    renderTarget.DrawTextW(message.c_str(), static_cast<UINT32>(message.size()), pTextFormat_.Get(), &rect,
                           pTextBrush_.Get());
}
