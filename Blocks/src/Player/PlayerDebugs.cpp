#include "Blocks/pch.h"
#include "Blocks/Player/PlayerDebugs.h"

#include <iomanip>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/GraphicsException.h"
#include "BlocksEngine/Main/Game.h"

using namespace Blocks;
using namespace BlocksEngine;


void PlayerDebugs::Start()
{
    SetEventTypes(EventType::Render2D);

    cameraTransform_ = GetTransform()->GetChild(0);

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

    pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void PlayerDebugs::Draw2D()
{
    std::wstringstream ss;

    ss << std::fixed << std::setprecision(2);
    ss << "FPS: " << GetGame()->Time().FramesPerSecond() << "\n\n";
    ss << "Player:\n";
    ss << "\tPosition: " << GetTransform()->GetPosition() << '\n';
    ss << "\tOrientation: " << GetTransform()->GetOrientation() << '\n';
    ss << "\tRotation: " << GetTransform()->GetOrientation().EulerAngles() << "\n\n";
    ss << "Camera:\n";
    ss << "\tPosition: " << cameraTransform_.lock()->GetPosition() << '\n';
    ss << "\tOrientation: " << cameraTransform_.lock()->GetOrientation() << '\n';
    ss << "\tRotation: " << cameraTransform_.lock()->GetOrientation().EulerAngles() << '\n';
    ss << "\tLocal Rotation: " << cameraTransform_.lock()->GetLocalOrientation().EulerAngles() << std::flush;


    const auto size = GetGame()->Graphics().Size();
    const float x = static_cast<float>(size.x);
    const D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, 600.0f, 200.0f);

    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();

    renderTarget.DrawTextW(ss.str().c_str(), static_cast<UINT32>(ss.str().size()), pTextFormat_.Get(), &rect,
                           pTextBrush_.Get());
}
