﻿#include "stdafx.h"

#include <Windows.h>

#include "Application.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    return BlocksEngine::Application{}.MainLoop();
}
