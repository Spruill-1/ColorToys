#pragma once

#include "MainPage.g.h"

using namespace winrt::Microsoft::Graphics::Canvas;

namespace winrt::ExactColors::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        void CanvasControl_CreateResources(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args);
        void CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args);
        void Button_Apply_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void RecreateBrush();

        CanvasDevice m_device;
        CanvasSwapChain m_swapchain;
        Brushes::CanvasSolidColorBrush m_brush;
        UI::Xaml::CanvasSwapChainPanel m_panel;

        float m_x = 0.312727f, m_y = 0.329023f;
    };
}

namespace winrt::ExactColors::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
