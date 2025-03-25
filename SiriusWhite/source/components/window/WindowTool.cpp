#include "WindowTool.hpp"
#include "logger/logger.hpp"

#include <string>

namespace Win32 {
	WindowTool::WindowTool(ComPtr<ID3D12Device> device, const wchar_t* label, const wchar_t* window_name, int32_t st_x, int32_t st_y, int32_t width, int32_t height)
		: D3D12FrameContext{ device }, m_label{ label }, m_windowName{ window_name }
	{
		ZeroMemory(&m_wndCl, sizeof(WNDCLASSEX));
		m_wndCl.cbSize = sizeof(WNDCLASSEX);
		m_wndCl.style = CS_HREDRAW | CS_VREDRAW;
		m_wndCl.lpfnWndProc = WindowProc;
		m_wndCl.cbClsExtra = 0L;
		m_wndCl.cbWndExtra = 0L;
		m_wndCl.hInstance = GetModuleHandle(nullptr);
		m_wndCl.hIcon = nullptr;
		m_wndCl.hCursor = nullptr; // LoadCursor(NULL, IDC_ARROW);
		m_wndCl.hbrBackground = nullptr; // (HBRUSH)COLOR_WINDOW;
		m_wndCl.lpszMenuName = nullptr;
		m_wndCl.lpszClassName = (LPCWSTR)m_label;
		m_wndCl.hIconSm = nullptr;

		if (!::RegisterClassExW(&m_wndCl))
		{
			std::string windowError{ "Main window class register failed" };
			//windowError.append(static_cast<const char*>(m_label));
			throw ui::exception(windowError.data(), ::GetLastError());
		}

		m_handleWindow = ::CreateWindowW(m_wndCl.lpszClassName, (LPCWSTR)m_windowName, WS_OVERLAPPEDWINDOW, st_x, st_y, width, height, nullptr, nullptr, m_wndCl.hInstance, nullptr);

		if (!::IsWindow(m_handleWindow))
		{
			std::string windowError{ "Ivalid handle window, window name:" };
			//windowError.append(static_cast<const char*>(m_windowName));
			throw ui::exception(windowError.data(), ::GetLastError());
		}
	}

	WindowTool::~WindowTool()
	{
		::DestroyWindow(m_handleWindow);
		::UnregisterClassW(m_wndCl.lpszClassName, m_wndCl.hInstance);
	}

	HWND WindowTool::key() { return m_handleWindow; }
	HWND WindowTool::getWindowHandle() { return m_handleWindow; }

	void WindowTool::show()
	{
		::ShowWindow(m_handleWindow, SW_SHOWDEFAULT);
		::UpdateWindow(m_handleWindow);
	}

	void WindowTool::onResize() {}
	void WindowTool::onDestroy() {}
	void WindowTool::onSysCommand() {}


	LRESULT CALLBACK WindowTool::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

} // namespace Win32