#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include <thread>

const int EDGE_DISTANCE = 60;
bool cursorLimitEnabled = false;
HWND robloxWindow = NULL;

void ToggleCursorLimit()
{
    cursorLimitEnabled = !cursorLimitEnabled;
    if (cursorLimitEnabled)
    {
        std::wcout << L"Cursor limit enabled" << std::endl;
    }
    else
    {
        std::wcout << L"Cursor limit disabled" << std::endl;
    }
}

void MoveThread()
{
    while (true)
    {
        if (cursorLimitEnabled && robloxWindow)
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            RECT screenRect;
            GetClientRect(robloxWindow, &screenRect);
            ClientToScreen(robloxWindow, reinterpret_cast<POINT *>(&screenRect.left));
            ClientToScreen(robloxWindow, reinterpret_cast<POINT *>(&screenRect.right));

            int newX = std::min(std::max(cursorPos.x, screenRect.left + EDGE_DISTANCE), screenRect.right - EDGE_DISTANCE);
            int newY = std::min(std::max(cursorPos.y, screenRect.top + EDGE_DISTANCE), screenRect.bottom - EDGE_DISTANCE);

            if (newX != cursorPos.x || newY != cursorPos.y)
            {
                SetCursorPos(newX, newY);
            }
        }
    }
}

int main()
{
    std::wcout << L"Press INSERT to toggle" << std::endl;
    std::thread moveThread(MoveThread);

    while (true)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            ToggleCursorLimit();
        }

        HWND activeWindow = GetForegroundWindow();
        wchar_t windowTitle[256];
        GetWindowTextW(activeWindow, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));
        robloxWindow = wcsstr(windowTitle, L"Roblox") ? activeWindow : NULL;

        Sleep(500);
    }

    moveThread.join();

    return 0;
}
