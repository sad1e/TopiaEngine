#include "WindowsEditor.h"
#include <Asserts.h>
#include <TopiaMath.h>

//#include <RHI.h>
//#include "SceneRenderer.h"

const wchar_t* TopiaEngineClassName = L"TopiaEngineWindowsClass";
const wchar_t* TopiaEngineTitleName = L"Topia Engine";

std::unique_ptr<WindowsEditor> g_WindowsEditor = nullptr;
//std::unique_ptr<topia::FSceneRenderer> g_SceneRenderer = nullptr;

HWND WindowsEditor::HWnd = nullptr;

LRESULT CALLBACK WindowsDesktopMsgProc(HWND InHWND, UINT InMessage, WPARAM InWParam, LPARAM InLParam)
{
	if (g_WindowsEditor != nullptr)
		return g_WindowsEditor->MsgProc(InHWND, InMessage, InWParam, InLParam);

	return NULL;
}

WindowsEditor::WindowsEditor() : Width(1280), Height(720), bActive(false) {}

void WindowsEditor::Init(HINSTANCE InHInstance, int InNCmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowsDesktopMsgProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = InHInstance;
	wcex.hIcon = LoadIcon(InHInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = TopiaEngineClassName;
	wcex.hIconSm = LoadIcon(InHInstance, IDI_APPLICATION);

	ASSERT(0 != ::RegisterClassEx(&wcex), "Unable to register a window");

	RECT rect = {0, 0, static_cast<LONG>(GetClientWidth()), static_cast<LONG>(GetClientHeight())};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWnd = CreateWindow(TopiaEngineClassName, TopiaEngineTitleName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, nullptr, nullptr, InHInstance, nullptr);
	ASSERT(HWnd != nullptr);

	ShowWindow(HWnd, InNCmdShow);
}

int WindowsEditor::Run()
{
	bool got;
	MSG msg = {};
	::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (bActive)
		{
			got = (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0);
		}
		else
		{
			got = (::GetMessage(&msg, nullptr, 0, 0) != 0);
		}

		if (got)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			// g_SceneRenderer->Render();
			// Engine Tick
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT WindowsEditor::MsgProc(HWND InHWND, UINT InMessage, WPARAM InWParam, LPARAM InLParam)
{
	switch (InMessage)
	{
		case WM_ACTIVATE:
		{
			bActive = (WA_INACTIVE != (LOWORD(InWParam)));
			break;
		}
		case WM_SIZE:
			// Display::Resize((UINT)(UINT64)lParam & 0xFFFF, (UINT)(UINT64)lParam >> 16);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(InHWND, InMessage, InWParam, InLParam);
	}

	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Allocate a console to view debug information.
	if (AllocConsole())
	{
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"TopiaDebug Console");

		HWND ConsoleHwnd = GetConsoleWindow();
		if (ConsoleHwnd)
		{
			if( HMENU hMenu = GetSystemMenu( ConsoleHwnd, FALSE ) )
			{
				EnableMenuItem( hMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
			}
		}
	}

	printf("%s\n", "Hello Topia!");

	g_WindowsEditor = std::make_unique<WindowsEditor>();
	g_WindowsEditor->Init(hInstance, nCmdShow);

	g_WindowsEditor->Run();

	FreeConsole();

	return 0;
}
