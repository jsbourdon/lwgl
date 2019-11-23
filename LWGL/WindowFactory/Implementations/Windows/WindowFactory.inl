#pragma once

#include <pch.h>
#include <stdio.h>
#include "LWGL/WindowFactory/WindowFactory.h"

using namespace lwgl;

struct WindowData
{
    wchar_t m_windowClass[32] { };
};

static WindowData g_WindowData;

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY)
    {
        PostQuitMessage(0);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void RegisterNewClass(HINSTANCE owner)
{
    swprintf_s(g_WindowData.m_windowClass, L"%s", L"lwgl_class");

    WNDCLASSEXW wcex {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = owner;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = g_WindowData.m_windowClass;

    RegisterClassExW(&wcex);
}

WindowHandle lwgl::factory::CreateNewWindow(AppHandle owner, uint32_t width, uint32_t height)
{
    HINSTANCE ownerHdl = reinterpret_cast<HINSTANCE>(owner);

    if (!g_WindowData.m_windowClass[0])
    {
        RegisterNewClass(ownerHdl);
    }

    HWND hWnd = CreateWindowW(g_WindowData.m_windowClass, L"LWGL", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, ownerHdl, nullptr);

    return reinterpret_cast<WindowHandle>(hWnd);
}

void lwgl::factory::DisplayWindow(const WindowHandle &hwnd)
{
    ShowWindow(reinterpret_cast<HWND>(hwnd), SW_SHOWDEFAULT);
}

void lwgl::factory::HideWindow(const WindowHandle &hwnd)
{
    ShowWindow(reinterpret_cast<HWND>(hwnd), SW_HIDE);
}

