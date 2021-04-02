#pragma once
#include "ViewModels.ItemViewModel.g.h"

namespace winrt::XamlListDetail::ViewModels::implementation
{
    struct ItemViewModel : ItemViewModelT<ItemViewModel>
    {
        ItemViewModel() = default;

        ItemViewModel(XamlListDetail::Data::Item const& item) :
            m_id{ item.ItemId() }, m_title{ item.Title() },
            m_text{ item.Text() }, m_dateCreated{ item.DateCreated() }
        {
        }

        static XamlListDetail::ViewModels::ItemViewModel FromItem(XamlListDetail::Data::Item const& item);
        int32_t ItemId();
        hstring DateCreatedHourMinute();
        hstring Title();
        hstring Text();
        Windows::Foundation::DateTime DateCreated();

    private:
        int const m_id;
        hstring const m_title;
        hstring m_text;
        Windows::Foundation::DateTime const m_dateCreated;
    };
}
namespace winrt::XamlListDetail::ViewModels::factory_implementation
{
    struct ItemViewModel : ItemViewModelT<ItemViewModel, implementation::ItemViewModel>
    {
    };
}
