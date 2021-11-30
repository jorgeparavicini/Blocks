#include "Blocks/pch.h"
#include "Blocks/Player/PlayerDebugs.h"

#include <iomanip>

#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/GraphicsException.h"
#include "BlocksEngine/Main/Game.h"

using namespace Blocks;
using namespace BlocksEngine;

PlayerDebugs::PlayerDebugs(std::weak_ptr<Actor> actor)
    : Component{std::move(actor)}
{
    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();

    HRESULT hr;
    GFX_THROW_INFO(renderTarget.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &pTextBrush_));

    IDWriteFactory& writeFactory = GetGame()->Graphics().GetDWriteFactory();
    GFX_THROW_INFO(
        writeFactory.CreateTextFormat(
            L"Verdana",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"",
            &pTextFormat_));

    pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void PlayerDebugs::Draw2D()
{
    std::wstringstream ss;

    ss << std::fixed << std::setprecision(2);
    ss << "FPS: " << GetGame()->Time().FramesPerSecond() << '\n';
    ss << "Position: " << GetTransform()->GetPosition().x << ", " << GetTransform()->GetPosition().y << ", " <<
        GetTransform()->GetPosition().z << '\n';

    const auto size = GetGame()->Graphics().Size();
    const float x = static_cast<float>(size.x);
    const float y = static_cast<float>(size.y);
    const D2D1_RECT_F rect = D2D1::RectF((x - 500.0f), 0.0f, x, 200.0f);

    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();

    renderTarget.DrawTextW(ss.str().c_str(), static_cast<UINT32>(ss.str().size()), pTextFormat_.Get(), &rect,
                           pTextBrush_.Get());
}
