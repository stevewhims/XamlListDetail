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
#include "ListDetailPage.xaml.h"
#if __has_include("ListDetailPage.g.cpp")
#include "ListDetailPage.g.cpp"
#endif

using namespace winrt;
using namespace XamlListDetail::Data;
using namespace XamlListDetail::ViewModels;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media::Animation;
using namespace Microsoft::UI::Xaml::Navigation;

namespace winrt::XamlListDetail::implementation
{
    ListDetailPage::ListDetailPage()
    {
        InitializeComponent();
        m_items = single_threaded_observable_vector<ItemViewModel>();
        for (auto&& item : ItemsDataSource::GetAllItems())
        {
            m_items.Append(ItemViewModel::FromItem(item));
        }
        ListView().ItemsSource(m_items);
    }

    void ListDetailPage::OnNavigatedTo(NavigationEventArgs const& e)
    {
        m_lastSelectedItem = nullptr;
        if (e.Parameter())
        {
            // Parameter is item ID
            auto id = unbox_value<int>(e.Parameter());

            for (auto&& item : m_items)
            {
                if (item.ItemId() == id)
                {
                    m_lastSelectedItem = item;
                    break;
                }
            }
        }

        UpdateForVisualState(AdaptiveStates().CurrentState());

        // Don't play a content transition for first item load.
        // Sometimes, this content will be animated as part of the page transition.
        DisableContentTransitions();
    }

    void ListDetailPage::AdaptiveStates_CurrentStateChanged(IInspectable const&, VisualStateChangedEventArgs const& e)
    {
        UpdateForVisualState(e.NewState(), e.OldState());
    }

    void ListDetailPage::UpdateForVisualState(VisualState const& newState, VisualState const& oldState)
    {
        auto isNarrow = newState == NarrowState();

        if (isNarrow && oldState == DefaultState() && m_lastSelectedItem != nullptr)
        {
            // Resize down to the detail item. Don't play a transition.
            Frame().Navigate(xaml_typename<DetailPage>(), box_value(m_lastSelectedItem.ItemId()), SuppressNavigationTransitionInfo());
        }

        EntranceNavigationTransitionInfo::SetIsTargetElement(ListView(), isNarrow);
        if (DetailContentPresenter() != nullptr)
        {
            EntranceNavigationTransitionInfo::SetIsTargetElement(DetailContentPresenter(), !isNarrow);
        }
    }

    void ListDetailPage::EnableContentTransitions()
    {
        DetailContentPresenter().ContentTransitions().ReplaceAll({ EntranceThemeTransition() });
    }

    void ListDetailPage::DisableContentTransitions()
    {
        if (DetailContentPresenter() != nullptr)
        {
            DetailContentPresenter().ContentTransitions().Clear();
        }
    }

    void ListDetailPage::LayoutRoot_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        // Assure we are displaying the correct item. This is necessary in certain adaptive cases.
        ListView().SelectedItem(m_lastSelectedItem);
    }

    void ListDetailPage::ListView_ItemClick(IInspectable const&, ItemClickEventArgs const& e)
    {
        auto clickedItem = e.ClickedItem().as<ItemViewModel>();
        m_lastSelectedItem = clickedItem;

        if (AdaptiveStates().CurrentState() == NarrowState())
        {
            // Use "drill in" transition for navigating from master list to detail view
            Frame().Navigate(xaml_typename<DetailPage>(), box_value(clickedItem.ItemId()), DrillInNavigationTransitionInfo());
        }
        else
        {
            // Play a refresh animation when the user switches detail items.
            EnableContentTransitions();
        }
    }
}
