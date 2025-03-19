#pragma once

#include "Windows.h"

namespace Win32 {
	class Win32Window
	{
	public:
		explicit Win32Window();
		virtual ~Win32Window();

		virtual HWND getWindowHandle() = 0;
		virtual void show() = 0;
		virtual void onResize() = 0;
		virtual void onDestroy() = 0;
		virtual void onSysCommand() = 0;
	};
} // namespace Win32