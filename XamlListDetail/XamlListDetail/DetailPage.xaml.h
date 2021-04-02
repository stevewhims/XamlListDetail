//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once
#include "DetailPage.g.h"

namespace winrt::XamlListDetail::implementation
{
    struct DetailPage : DetailPageT<DetailPage>
    {
        DetailPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        static void RegisterDependencyProperties();

        ViewModels::ItemViewModel Item() { return GetValue(s_itemProperty).as<ViewModels::ItemViewModel>(); }
        void Item(ViewModels::ItemViewModel value) { SetValue(s_itemProperty, value); }

        static Microsoft::UI::Xaml::DependencyProperty ItemProperty() { return s_itemProperty; }

        void PageRoot_Loaded(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void Window_SizeChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::WindowSizeChangedEventArgs const& e);

    private:
        winrt::Microsoft::UI::Xaml::Window::SizeChanged_revoker m_event_revoker;

        static Microsoft::UI::Xaml::DependencyProperty s_itemProperty;

        void ReplaceLastBackStackEntryParameter(Windows::Foundation::IInspectable const& parameter);
        void NavigateBackForWideState(bool useTransition);
        bool ShouldGoToWideState();
    };
}

namespace winrt::XamlListDetail::factory_implementation
{
    struct DetailPage : DetailPageT<DetailPage, implementation::DetailPage>
    {
    };
}
