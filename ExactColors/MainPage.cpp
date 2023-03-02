#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::Graphics::Canvas;
using namespace winrt::Microsoft::Graphics::Canvas::Brushes;
using namespace winrt::Microsoft::Graphics::Canvas::UI::Xaml;
using namespace winrt::Windows::Graphics::DirectX;

namespace winrt::ExactColors::implementation
{
    static void ComputeRGBFromXYZ(double XYZ[3], double RGB[3])
    {
        constexpr double scRGB_Mat[3][3] =
            { { 3.2404542, -1.5371385, -0.4985314},
              {-0.9692660,  1.8760108,  0.0415560},
              { 0.0556434, -0.2040259,  1.0572252} };

        // Loop through each row of the matrix
        for (int i = 0; i < 3; i++) {
            // Initialize the result element to zero
            RGB[i] = 0;
            // Loop through each column of the matrix
            for (int j = 0; j < 3; j++) {
                // Add the product of the matrix element and the vector element to the result element
                RGB[i] += scRGB_Mat[i][j] * XYZ[j];
            }
        }
    }

    MainPage::MainPage() : 
        m_device(nullptr), m_swapchain(nullptr), m_brush(nullptr), m_panel(nullptr)
    {
        InitializeComponent();
    }

    void MainPage::CanvasControl_CreateResources(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args)
    {
        m_panel = UI::Xaml::CanvasSwapChainPanel();
        m_panel.Width(500);
        m_panel.Height(500);
        MainPanel().Children().Append(m_panel);

        m_device = CanvasDevice();
        m_swapchain = CanvasSwapChain(
            m_device.GetSharedDevice(),
            500, 
            500,
            CanvasControl().Dpi(),
            DirectXPixelFormat::R16G16B16A16Float,
            2,
            CanvasAlphaMode::Ignore
        );

        RecreateBrush();
    }

    void MainPage::RecreateBrush()
    {
        double x = _wtof(TextBox_x().Text().c_str());
        double y = _wtof(TextBox_y().Text().c_str());
        double Y = _wtof(TextBox_Y().Text().c_str());

        double scRGB[3], XYZ[3];
        XYZ[0] = x * Y / y;             // X
        XYZ[1] = Y;                     // Y
        XYZ[2] = (1.0 - x - y) * Y / y; // Z

        ComputeRGBFromXYZ(XYZ, scRGB);
        m_brush = CanvasSolidColorBrush::CreateHdr(m_device.GetSharedDevice(), {static_cast<float>(scRGB[0]), static_cast<float>(scRGB[1]), static_cast<float>(scRGB[2]), 1.0f});
        m_brush.ColorHdr({ static_cast<float>(scRGB[0]), static_cast<float>(scRGB[1]), static_cast<float>(scRGB[2]), 1.0f });
    }

    void MainPage::CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
    {
        m_panel.SwapChain(m_swapchain);

        {
            auto ds = m_swapchain.CreateDrawingSession(winrt::Windows::UI::Colors::AliceBlue());
            ds.FillRectangle(0, 0, 500, 500, m_brush);
        }
        m_swapchain.Present();
    }

    void MainPage::Button_Apply_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        RecreateBrush();

        CanvasControl().Invalidate();
    }
}