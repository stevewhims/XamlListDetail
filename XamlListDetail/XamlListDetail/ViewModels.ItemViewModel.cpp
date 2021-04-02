#include "pch.h"
#include "ViewModels.ItemViewModel.h"
#include "ViewModels.ItemViewModel.g.cpp"

namespace winrt::XamlListDetail::ViewModels::implementation
{
    XamlListDetail::ViewModels::ItemViewModel ItemViewModel::FromItem(XamlListDetail::Data::Item const& item)
    {
        return make<ItemViewModel>(item);
    }
    int32_t ItemViewModel::ItemId()
    {
        return m_id;
    }
    hstring ItemViewModel::DateCreatedHourMinute()
    {
        auto formatter = Windows::Globalization::DateTimeFormatting::DateTimeFormatter(L"hour minute");
        return formatter.Format(m_dateCreated);
    }
    hstring ItemViewModel::Title()
    {
        return m_title;
    }
    hstring ItemViewModel::Text()
    {
        return m_text;
    }
    Windows::Foundation::DateTime ItemViewModel::DateCreated()
    {
        return m_dateCreated;
    }
}
