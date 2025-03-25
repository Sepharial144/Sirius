#pragma once

// Dear ImGui: standalone example application for DirectX 12

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "implot.h"
#include "implot_internal.h"

#include "D3D12Context.hpp"

#include "Windows.h"
#include <memory>


// Encapsulates examples for customizing ImPlot.
namespace MyImPlot {

    // Example for Custom Data and Getters section.
    struct Vector2f {
        Vector2f(float _x, float _y) { x = _x; y = _y; }
        float x, y;
    };

    // Example for Custom Data and Getters section.
    struct WaveData {
        double X, Amp, Freq, Offset;
        WaveData(double x, double amp, double freq, double offset) { X = x; Amp = amp; Freq = freq; Offset = offset; }
    };
    ImPlotPoint SineWave(int idx, void* wave_data);
    ImPlotPoint SawWave(int idx, void* wave_data);
    ImPlotPoint Spiral(int idx, void* wave_data);

    // Example for Tables section.
    void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size);

    // Example for Custom Plotters and Tooltips section.
    void PlotCandlestick(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip = true, float width_percent = 0.25f, ImVec4 bullCol = ImVec4(0, 1, 0, 1), ImVec4 bearCol = ImVec4(1, 0, 0, 1));

    // Example for Custom Styles section.
    void StyleSeaborn();

} // namespace MyImPlot

class Application
{
public:
	explicit Application();
	virtual ~Application();

	HWND createWindow();
	void Run();

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	WNDCLASSEXW m_wc;
	HWND m_handleWindow;
};