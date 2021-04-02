#include "pch.h"
#include "Data.Item.h"
#include "Data.Item.g.cpp"

namespace winrt::XamlListDetail::Data::implementation
{
    Item::Item(int32_t itemId, Windows::Foundation::DateTime const& dateCreated, hstring const& title, hstring const& text) :
        m_itemId{ itemId }, m_dateCreated{ dateCreated }, m_title{ std::move(title) }, m_text{ std::move(text) }
    {
        
    }
    int32_t Item::ItemId()
    {
        return m_itemId;
    }
    void Item::ItemId(int32_t value)
    {
        m_itemId = value;
    }
    hstring Item::Title()
    {
        return m_title;
    }
    void Item::Title(hstring const& value)
    {
        m_title = value;
    }
    hstring Item::Text()
    {
        return m_text;
    }
    void Item::Text(hstring const& value)
    {
        m_text = value;
    }
    Windows::Foundation::DateTime Item::DateCreated()
    {
        return m_dateCreated;
    }
    void Item::DateCreated(Windows::Foundation::DateTime const& value)
    {
        m_dateCreated = value;
    }
}
