// 2022/05/05 12:27:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Menu/Menu.h"
#include "Utils/Text/String.h"
#include "Utils/Values.h"


Item Item::Empty;
const Item *Item::opened_item = &Item::Empty;


void Item::Open() const
{
    opened_item = this;
}


void Item::Close() const
{
    if (this == PageMain::self)
    {
        opened_item = &Page::Empty;
    }
    else
    {
        opened_item = ReinterpretToDItem()->keeper;
    }
}


void Page::Close() const
{
    Item::Close();

    uint8 *currentItem = ReinterpretToDPage()->currentItem;

    if (*currentItem == NumItems() - 1)
    {
        *currentItem = 0;
    }
}


void Item::Draw(int x, int y, bool active) const
{
    if (IsOpened())
    {
        DrawOpened(x, y, active);
    }
    else
    {
        DrawClosed(x, y, active);
    }
}


bool Item::IsOpened() const
{
    if (IsPage())
    {
        return (this == opened_item) || (Item::Opened()->Keeper() == this);
    }

    return (this == opened_item);
}


void Item::DrawOpened(int x, int y, bool active) const
{
    if (IsPage())
    {
        ToPage()->DrawOpened(x, y, active);
    }
    else if (IsGovernor())
    {
        ReinterpretToGovernor()->DrawOpened(x, y, active);
    }
}


void Item::DrawClosed(int x, int y, bool active) const
{
    Color::E fill = Color::BLACK;
    Color::E draw = Color::WHITE;

    if (ReinterpretToDItem()->keeper->CurrentItem() == this)
    {
        fill = Color::GREEN_50;
    }

    if (Item::Opened() != Keeper())
    {
        draw = Color::GRAY_25;
        fill = Color::GRAY_10;
    }

    Rectangle(Item::WIDTH, Item::HEIGHT).DrawFilled(x, y, fill, draw);

    if (IsPage())
    {
        ToPage()->DrawClosed(x, y, active);
    }
    else if (IsChoice())
    {
        ReinterpretToChoice()->DrawClosed(x, y, active);
    }
    else if (IsButton())
    {
        ReinterpretToButton()->DrawClosed(x, y, active);
    }
    else if (IsGovernor())
    {
        ReinterpretToGovernor()->DrawClosed(x, y, active);
    }
}


String<> Item::Title() const
{
    return String<>(ReinterpretToDItem()->title);
}


void Page::DrawOpened(int x, int y, bool active) const
{
    DrawTitle(x, y);

    DrawItems(x, y + Menu::Title::HEIGHT, active);
}


void Page::DrawTitle(int x, int y) const
{
    Rectangle(Item::WIDTH, 27).DrawFilled(x, y, Color::BLACK, Color::WHITE);

    Title().Draw(x + 10, y + 10, Color::BLUE);
}


void Page::DrawItems(int x, int y, bool active) const
{
    for (int i = FirstItemOnScreen(); i < LastItemOnScreen(); i++)
    {
        ReinterpretToDPage()->items[i]->DrawClosed(x, y, active);
        y += Item::HEIGHT;
    }
}


const Item *Page::CurrentItem() const
{
    const Item *const *items = ReinterpretToDPage()->items;

    uint8 *currentItem = ReinterpretToDPage()->currentItem;

    return items[*currentItem];
}


void Page::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));
}


pchar Choice::CurrentName() const
{
    const DChoice *choice = ReinterpretToDChoice();

    return choice->names[*choice->cell];
}


void Choice::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    String<>(CurrentName()).Draw(x + 130, y + 5);
}


void Button::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));
}


void Governor::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    Int(*RetinterpretToDGovernor()->value).ToStirng().Draw(x + 100, y + 5, Color::MenuLetters(active));
}


void Governor::DrawOpened(int x, int y, bool active) const
{
    Color::E fill = Color::BLACK;
    Color::E draw = Color::WHITE;

    if (ReinterpretToDItem()->keeper->CurrentItem() == this)
    {
        fill = Color::GREEN_50;
    }

    if (!active && (Item::Opened() != Keeper()))
    {
        draw = Color::GRAY_25;
        fill = Color::GRAY_10;
    }

    Rectangle(Item::WIDTH, Item::HEIGHT).DrawFilled(x, y, fill, draw);
}


int Page::FirstItemOnScreen() const
{
    return (*ReinterpretToDPage()->currentItem / Page::NUM_ITEMS_ON_SCREEN) * Page::NUM_ITEMS_ON_SCREEN;
}


int Page::LastItemOnScreen() const
{
    int result = FirstItemOnScreen() + Page::NUM_ITEMS_ON_SCREEN;

    if (result > NumItems())
    {
        result = NumItems();
    }

    return result;
}


int Page::NumItems() const
{
    for (int i = 0; ; i++)
    {
        if (ReinterpretToDPage()->items[i] == nullptr)
        {
            return i;
        }
    }
}


void Page::ShortPressure() const
{
    uint8 *currentItem = ReinterpretToDPage()->currentItem;

    *currentItem = (uint8)(*currentItem + 1);

    if (*currentItem == NumItems())
    {
        *currentItem = 0;
    }
}


void Page::LongPressure() const
{
    const Item *item = CurrentItem();

    if (item->IsPage())
    {
        item->ToPage()->Open();
    }
    else if (item->IsChoice())
    {
        item->ReinterpretToChoice()->LongPressure();
    }
    else if (item->IsButton())
    {
        item->ReinterpretToButton()->LongPressure();
    }
    else if (item->IsGovernor())
    {
        item->ReinterpretToGovernor()->LongPressure();
    }
}


void Page::DoubleClick() const
{

}


void Choice::ShortPressure() const
{

}


void Button::ShortPressure() const
{

}


void Choice::LongPressure() const
{
    uint8 *cell = ReinterpretToDChoice()->cell;

    *cell = (uint8)(*cell + 1);

    if (*cell == ReinterpretToDChoice()->count)
    {
        *cell = 0;
    }
}


void Choice::DoubleClick() const
{

}


void Item::ShortPressure() const
{
    switch (ReinterpretToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->ShortPressure();        break;
    case TypeItem::Choice:      ReinterpretToChoice()->ShortPressure();     break;
    case TypeItem::Button:      ReinterpretToButton()->ShortPressure();     break;
    case TypeItem::Governor:    ReinterpretToGovernor()->ShortPressure();   break;
    case TypeItem::Count:                                                   break;
    }
}


void Item::LongPressure() const
{
    switch (ReinterpretToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->LongPressure();     break;
    case TypeItem::Choice:      ReinterpretToChoice()->LongPressure();  break;
    case TypeItem::Button:      ReinterpretToChoice()->LongPressure();  break;
    case TypeItem::Governor:    ReinterpretToGovernor()->LongPressure();break;
    case TypeItem::Count:                                               break;
    }
}


void Item::DoubleClick() const
{
    switch (ReinterpretToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->DoubleClick();      break;
    case TypeItem::Choice:      ReinterpretToChoice()->DoubleClick();   break;
    case TypeItem::Button:      ReinterpretToChoice()->DoubleClick();   break;
    case TypeItem::Governor:    ReinterpretToGovernor()->DoubleClick(); break;
    case TypeItem::Count:                                               break;
    }
}


void Governor::ShortPressure() const
{

}


void Governor::LongPressure() const
{
    if (Item::Opened() != this)
    {
        Open();
    }
    else
    {
        Close();
    }
}


void Governor::DoubleClick() const
{

}
