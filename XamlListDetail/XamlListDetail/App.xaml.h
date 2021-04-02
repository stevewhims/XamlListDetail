#pragma once

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "App.xaml.g.h"

#pragma pop_macro("GetCurrentTime")

namespace winrt::XamlListDetail::implementation
{
struct App : AppT<App>
{
    App();
    Microsoft::UI::Xaml::Controls::Frame CreateRootFrame();

    void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
    void OnNavigationFailed(IInspectable const&, Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

    static winrt::Microsoft::UI::Xaml::Window Window() { return window; }

private:
    static winrt::Microsoft::UI::Xaml::Window window;
};
}
