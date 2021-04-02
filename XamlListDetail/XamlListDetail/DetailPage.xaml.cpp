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

#include "pch.h"
#include "DetailPage.xaml.h"
#include "DetailPage.g.cpp"
#include <App.xaml.h>

using namespace winrt;
using namespace XamlListDetail::Data;
using namespace XamlListDetail::ViewModels;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media::Animation;
using namespace Microsoft::UI::Xaml::Navigation;

namespace winrt::XamlListDetail::implementation
{
    Microsoft::UI::Xaml::DependencyProperty DetailPage::s_itemProperty{ nullptr };

    void DetailPage::RegisterDependencyProperties()
    {
        s_itemProperty = DependencyProperty::Register(L"Item", xaml_typename<ItemViewModel>(),
            xaml_typename<XamlListDetail::DetailPage>(), nullptr);
    }

    DetailPage::DetailPage()
    {
        InitializeComponent();
    }

    void DetailPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        // Parameter is item ID
        auto id = unbox_value<int>(e.Parameter());
        Item(ItemViewModel::FromItem(ItemsDataSource::GetItemById(id)));

        ReplaceLastBackStackEntryParameter(e.Parameter());
    }

    // Doctor the navigation parameter for the master page so it
    // will show the correct item in the side-by-side view.
    void DetailPage::ReplaceLastBackStackEntryParameter(IInspectable const& parameter)
    {
        auto backStack = Frame().BackStack();
        auto backStackCount = backStack.Size();

        if (backStackCount > 0)
        {
            auto masterPageEntry = backStack.GetAt(backStackCount - 1);

            auto modifiedEntry = PageStackEntry(masterPageEntry.SourcePageType(), parameter, masterPageEntry.NavigationTransitionInfo());

            backStack.SetAt(backStackCount - 1, modifiedEntry);
        }
    }

    void DetailPage::NavigateBackForWideState(bool useTransition)
    {
        // Evict this page from the cache as we may not need it again.
        NavigationCacheMode(NavigationCacheMode::Disabled);

        if (useTransition)
        {
            Frame().GoBack(EntranceNavigationTransitionInfo());
        }
        else
        {
            Frame().GoBack(SuppressNavigationTransitionInfo());
        }
    }

    bool DetailPage::ShouldGoToWideState()
    {
        return App::Window().Bounds().Width >= 720.0;
    }

    void DetailPage::PageRoot_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        if (ShouldGoToWideState())
        {
            // We shouldn't see this page since we are in "wide master-detail" mode.
            // Play a transition as we are likely navigating back from a separate page.
            NavigateBackForWideState(/* useTransition */ true);
        }
        else
        {
            // Realize the main page content.
            [[maybe_unused]] auto rootPanel = FindName(L"RootPanel");
        }

        m_event_revoker = App::Window().SizeChanged(winrt::auto_revoke, { this, &DetailPage::Window_SizeChanged });
    }


    void DetailPage::Window_SizeChanged(IInspectable const&, Microsoft::UI::Xaml::WindowSizeChangedEventArgs const&)
    {
        if (ShouldGoToWideState())
        {
            // We shouldn't see this page since we are in "wide master-detail" mode.
            NavigateBackForWideState(/* useTransition */ false);
        }
    }
}
