#pragma once
#include "Data.Item.g.h"

namespace winrt::XamlListDetail::Data::implementation
{
    struct Item : ItemT<Item>
    {
        Item() = default;

        Item(int32_t itemId, Windows::Foundation::DateTime const& dateCreated, hstring const& title, hstring const& text);
        int32_t ItemId();
        void ItemId(int32_t value);
        hstring Title();
        void Title(hstring const& value);
        hstring Text();
        void Text(hstring const& value);
        Windows::Foundation::DateTime DateCreated();
        void DateCreated(Windows::Foundation::DateTime const& value);

    private:
        int m_itemId;
        Windows::Foundation::DateTime m_dateCreated;
        hstring m_title;
        hstring m_text;
    };
}
namespace winrt::XamlListDetail::Data::factory_implementation
{
    struct Item : ItemT<Item, implementation::Item>
    {
    };
}
