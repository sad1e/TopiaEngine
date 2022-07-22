#pragma once

#include "resource.h"

#include <Topia.h>

#include <cstdint>
#include <memory>

class WindowsEditor final
{
public:
    WindowsEditor();

    void Init(HINSTANCE InHInstance, int InNCmdShow);
    int Run();
    LRESULT CALLBACK MsgProc(HWND InHWND, UINT InMessage, WPARAM InWParam, LPARAM InLParam);

public:
    inline u32 GetClientWidth() const { return Width; }
    inline u32 GetClientHeight() const { return Height; }

    inline static HWND GetWindowHandler() { return HWnd; }

    inline bool GetActive() const { return bActive; }

protected:
    WindowsEditor(const WindowsEditor&) = delete;
    WindowsEditor(WindowsEditor&) = delete;
    WindowsEditor& operator=(const WindowsEditor&) = delete;
    WindowsEditor& operator=(WindowsEditor&&) = delete;

    u32 Width;
    u32 Height;

private:
    static HWND HWnd;

    u8 bActive : 1;
};
