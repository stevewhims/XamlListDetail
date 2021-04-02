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
#include "ListDetailPage.g.h"

namespace winrt::XamlListDetail::implementation
{
    struct ListDetailPage : ListDetailPageT<ListDetailPage>
    {
        ListDetailPage();

        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

        // These methods are public so they can be called by binding.
        void LayoutRoot_Loaded(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void AdaptiveStates_CurrentStateChanged(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::VisualStateChangedEventArgs const& e);
        void ListView_ItemClick(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::Controls::ItemClickEventArgs const& e);

    private:
        Windows::Foundation::Collections::IVector<XamlListDetail::ViewModels::ItemViewModel> m_items;
        XamlListDetail::ViewModels::ItemViewModel m_lastSelectedItem{ nullptr };

        void UpdateForVisualState(Microsoft::UI::Xaml::VisualState const& newState, Microsoft::UI::Xaml::VisualState const& oldState = nullptr);
        void EnableContentTransitions();
        void DisableContentTransitions();
    };
}

namespace winrt::XamlListDetail::factory_implementation
{
    struct ListDetailPage : ListDetailPageT<ListDetailPage, implementation::ListDetailPage>
    {
    };
}
