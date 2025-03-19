#pragma once

#include "Win32Window.hpp"
#include "WinProcEvent.hpp"

#include "exception/exception.hpp"

#include <cstdint>

namespace Win32 {

	class WindowTool: Win32::Win32Window
	{
	public:
		explicit WindowTool() = delete;
		explicit WindowTool(const wchar_t* label, const wchar_t* window_name, int32_t st_x, int32_t st_y, int32_t width, int32_t height);
		virtual ~WindowTool();

		virtual HWND key();

		virtual HWND getWindowHandle() override;
		virtual void show() override;
		virtual void onResize() override;
		virtual void onDestroy() override;
		virtual void onSysCommand() override;

	private:
		const wchar_t* m_label = nullptr;
		const wchar_t* m_windowName = nullptr;
		HWND m_handleWindow = nullptr;
		WNDCLASSEXW m_wndCl;
	};
} // namespace Win32