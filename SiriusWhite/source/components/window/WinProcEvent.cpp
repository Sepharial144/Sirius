#include "WinProcEvent.hpp"
#include "logger/logger.hpp"

LRESULT WINAPI SharedWinProcEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        logger::info("Resize window");
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        logger::info("Destroy window");
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}