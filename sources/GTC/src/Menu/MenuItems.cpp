// 2022/05/05 12:27:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Menu/Menu.h"
#include "Utils/Text/Text.h"
#include "Utils/Values.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Font/Font.h"
#include "Utils/Math.h"


Item Item::Empty;
const Item *Item::opened_item = &Item::Empty;
Governor::ActiveControl::E Governor::active_control = Governor::ActiveControl::Increase;


void Item::Open() const
{
    if (ToDItem()->funcOnOpenClose)
    {
        ToDItem()->funcOnOpenClose(true);
    }

    opened_item = this;
}


void Item::Close() const
{
    if (ToDItem()->funcOnOpenClose)
    {
        ToDItem()->funcOnOpenClose(false);
    }

    if (this == PageMain::self)
    {
        opened_item = &Page::Empty;
    }
    else
    {
        opened_item = ToDItem()->keeper;
    }
}


void Page::Close() const
{
    Item::Close();

    uint8 *currentItem = ToDPage()->currentItem;

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
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->DrawOpened(x, y, active);     break;
    case TypeItem::Choice:      ToChoice()->DrawOpened(x, y, active);   break;
    case TypeItem::Button:      ToButton()->DrawOpened(x, y, active);   break;
    case TypeItem::Governor:    ToGovernor()->DrawOpened(x, y, active); break;
    case TypeItem::Time:        ToTimeItem()->DrawOpened(x, y, active); break;
    case TypeItem::Count:   break;
    }
}


void Item::DrawClosed(int x, int y, bool active) const
{
    Color::E fill = Color::BLACK;
    Color::E draw = Color::WHITE;

    if (ToDItem()->keeper->CurrentItem() == this)
    {
        fill = Color::MenuItem();
    }

    if (Item::Opened() != Keeper())
    {
        draw = Color::GRAY_25;
        fill = Color::GRAY_10;
    }

    Rectangle(Item::WIDTH, Item::HEIGHT).DrawFilled(x, y, fill, draw);

    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->DrawClosed(x, y, active);     break;
    case TypeItem::Choice:      ToChoice()->DrawClosed(x, y, active);   break;
    case TypeItem::Button:      ToButton()->DrawClosed(x, y, active);   break;
    case TypeItem::Governor:    ToGovernor()->DrawClosed(x, y, active); break;
    case TypeItem::Time:        ToTimeItem()->DrawClosed(x, y, active); break;
    case TypeItem::Count:               break;
    }
}


String<> Item::Title() const
{
    return String<>(ToDItem()->title);
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
        ToDPage()->items[i]->DrawClosed(x, y, active);
        y += Item::HEIGHT;
    }
}


const Item *Page::CurrentItem() const
{
    const Item *const *items = ToDPage()->items;

    uint8 *currentItem = ToDPage()->currentItem;

    return items[*currentItem];
}


void Page::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));
}


pchar Choice::CurrentName() const
{
    const DChoice *choice = ToDChoice();

    return choice->names[*choice->cell];
}


void Choice::DrawOpened(int, int, bool) const
{

}


void Choice::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    String<>(CurrentName()).Draw(x + 130, y + 5);
}


void Button::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    if (ToDButton()->marked && *ToDButton()->marked)
    {
        x += 130;
        String<>("\x85").Draw(x, y + 5);
        String<>("\x86").Draw(x + 10, y + 5);
    }
}


void Button::DrawOpened(int x, int y, bool active) const
{
    DrawClosed(x, y, active);
}


void Governor::DrawClosed(int x, int y, bool active) const
{
    Title().Draw(x + 10, y + 5, Color::MenuLetters(active));

    Int(*ToDGovernor()->value).ToStirng().DrawRelativelyRight(x + 150, y + 5, Color::MenuLetters(active));
}


void Governor::DrawOpened(int x, int y, bool active) const
{
    Color::E fill = Color::BLACK;
    Color::E draw = Color::WHITE;

    if (ToDItem()->keeper->CurrentItem() == this)
    {
        fill = Color::MenuItem();
    }

    if (!active && (Item::Opened() != Keeper()))
    {
        draw = Color::GRAY_25;
        fill = Color::GRAY_10;
    }

    Rectangle(Item::WIDTH, Item::HEIGHT).DrawFilled(x, y, fill, draw);

    Title().Draw(x + 10, y + 5, Color::MenuLetters(true));

    DrawControls(x, y);

    Int(*ToDGovernor()->value).ToStirng().DrawRelativelyRight(x + 150, y + 5, Color::MenuLetters(active));
}


void TimeItem::DrawClosed(int x, int y, bool) const
{
    PackedTime time = HAL_RTC::GetTime();

    y += 4;

    x += 15;

    String<>("%02d:%02d:%02d", time.hours, time.minutes, time.seconds).Draw(x, y, Color::WHITE);

    String<>("%02d:%02d:%04d", time.day, time.month, time.year + 2000).Draw(x + 70, y);
}


void TimeItem::LongPressure(Key::E) const
{
    if (IsOpened())
    {
        *ToDTimeItem()->state = 0;
    }
    else
    {
        Open();
    }
}


void TimeItem::ChangeCurrentField(Key::E key) const
{
    const DTimeItem *data = ToDTimeItem();

    if (*data->cur_field < 6)
    {
        int max[6] = { 23, 59, 59, 31, 12, 99 };

        PackedTime &time = *data->time;

        int *values[6] = { &time.hours, &time.minutes, &time.seconds,
                            &time.day, &time.month, &time.year };

        if (key == Key::_1)
        {
            Math::CircleIncrease(values[*data->cur_field], 0, max[*data->cur_field]);
        }
        else if (key == Key::_2)
        {
            Math::CircleDecrease(values[*data->cur_field], 0, max[*data->cur_field]);
        }
    }
}


void TimeItem::ShortPressure(Key::E key) const
{
    if (IsOpened())
    {
        const DTimeItem *data = ToDTimeItem();

        if (*data->state == 0)
        {
            if (key == Key::_1)
            {
                Math::CircleIncrease<int>(data->cur_field, 0, 7);
            }
            else if (key == Key::_2)
            {
                if (*data->cur_field == 6)
                {
                    HAL_RTC::SetTime(*data->time);
                    Close();
                }
                else if (*data->cur_field == 7)
                {
                    Close();
                }
                else
                {
                    *data->state = 1;
                }
            }
        }
        else if (*data->state == 1)
        {
            ChangeCurrentField(key);
        }
    }
}


void TimeItem::DrawOpened(int, int, bool) const
{
    const DTimeItem *data = ToDTimeItem();

    Display::BeginScene(Color::BLACK);

    Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0, Color::WHITE);

    int x0 = 20;
    int y0 = 20;
    int dX = 48;
    int dY = 40;

    int values[6] = { data->time->hours, data->time->minutes, data->time->seconds,
        data->time->day, data->time->month, data->time->year };

    for (int i = 0; i < 6; i++)
    {
        Color::E color = Color::WHITE;

        int x = x0 + dX * (i % 3);
        int y = y0 + (i < 3 ? 0 : dY);

        if (i == *data->cur_field)
        {
            Rectangle(29, 27).Fill(x - 2, y - 2, Color::WHITE);
            color = Color::BLACK;
        }

        Font::Text::DrawBig(x, y, 2, String<>("%02d", values[i]).c_str(), color);
    }

    int x = 105;
    int y = 101;
    int size = 18;
    int dT = 3;

    Rectangle rect(size, size);
    rect.Draw(x, y, Color::WHITE);
    String<>(*data->state == 0 ? "�" : "\x80").Draw(x + dT + 3, y + dT);
    x += 27;
    rect.Draw(x, y);
    String<>(*data->state == 0 ? "�" : "\x81").Draw(x + dT + 3, y + dT);


    Color::E color = Color::WHITE;

    if (*data->cur_field == 6)
    {
        Rectangle(56, 15).Fill(23, y + dT - 15, Color::WHITE);
        color = Color::BLACK;
    }

    String<>("���������").Draw(25, y + dT - 13, color);

    color = Color::WHITE;

    if (*data->cur_field == 7)
    {
        Rectangle(56, 15).Fill(23, y + dT + 5, Color::WHITE);
        color = Color::BLACK;
    }

    String<>("�����").Draw(25, y + dT + 7, color);
}


void Governor::DrawControls(int x, int y) const
{
    DrawControl(x + 90, y + 3, String<>("\x83"), active_control == ActiveControl::Increase);

    DrawControl(x + 90, y + 12, String<>("\x82"), active_control == ActiveControl::Decrease);

    DrawControl(x + 105, y + 5, String<>("\x84"), active_control == ActiveControl::Close);
}


void Governor::DrawControl(int x, int y, const String<> &symbol, bool active) const
{
    Color::E color_draw = Color::MenuLetters(true);

    if (active)
    {
        int width = 10;
        int height = 6;

        if (active_control == ActiveControl::Close)
        {
            width = 11;
            height = 11;
        }

        Rectangle(width, height).Fill(x - 1, y - 1, Color::MenuLetters(true));
        color_draw = Color::MenuItem();
    }

    symbol.Draw(x, y, color_draw);
}


int Page::FirstItemOnScreen() const
{
    return (*ToDPage()->currentItem / Page::NUM_ITEMS_ON_SCREEN) * Page::NUM_ITEMS_ON_SCREEN;
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
        if (ToDPage()->items[i] == nullptr)
        {
            return i;
        }
    }
}


void Page::ShortPressure(Key::E key) const
{
    if (key == Key::_1)
    {
        uint8 *currentItem = ToDPage()->currentItem;

        *currentItem = (uint8)(*currentItem + 1);

        if (*currentItem == NumItems())
        {
            *currentItem = 0;
        }
    }
    else if (key == Key::_2)
    {
        LongPressure(key);
    }
}


void Page::LongPressure(Key::E key) const
{
    const Item *item = CurrentItem();

    const DItem *data = item->ToDItem();

    switch (data->type)
    {
    case TypeItem::Page:        item->ToPage()->Open();                break;
    case TypeItem::Choice:      item->ToChoice()->LongPressure();      break;
    case TypeItem::Button:      item->ToButton()->LongPressure();      break;
    case TypeItem::Governor:    item->ToGovernor()->LongPressure();    break;
    case TypeItem::Time:        item->ToTimeItem()->LongPressure(key); break;
    case TypeItem::Count:                                              break;
    }
}


void Page::DoubleClick() const
{

}


void Choice::ShortPressure(Key::E) const
{

}


void Button::ShortPressure(Key::E) const
{

}


void Choice::LongPressure() const
{
    uint8 *cell = ToDChoice()->cell;

    *cell = (uint8)(*cell + 1);

    if (*cell == ToDChoice()->count)
    {
        *cell = 0;
    }
}


void Choice::DoubleClick() const
{

}


void Button::DoubleClick() const
{

}


void Item::ShortPressure(Key::E key) const
{
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->ShortPressure(key);      break;
    case TypeItem::Choice:      ToChoice()->ShortPressure(key);    break;
    case TypeItem::Button:      ToButton()->ShortPressure(key);    break;
    case TypeItem::Governor:    ToGovernor()->ShortPressure(key);  break;
    case TypeItem::Time:        ToTimeItem()->ShortPressure(key);  break;
    case TypeItem::Count:                                          break;
    }
}


void Item::LongPressure(Key::E key) const
{
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->LongPressure(key);        break;
    case TypeItem::Choice:      ToChoice()->LongPressure();         break;
    case TypeItem::Button:      ToButton()->LongPressure();         break;
    case TypeItem::Governor:    ToGovernor()->LongPressure();       break;
    case TypeItem::Time:        ToTimeItem()->LongPressure(key);    break;
    case TypeItem::Count:                                           break;
    }
}


void Item::DoubleClick() const
{
    switch (ToDItem()->type)
    {
    case TypeItem::Page:        ToPage()->DoubleClick();        break;
    case TypeItem::Choice:      ToChoice()->DoubleClick();      break;
    case TypeItem::Button:      ToButton()->DoubleClick();      break;
    case TypeItem::Governor:    ToGovernor()->DoubleClick();    break;
    case TypeItem::Time:        ToTimeItem()->DoubleClick();    break;
    case TypeItem::Count:                                       break;
    }
}


void Governor::ShortPressure(Key::E key) const
{
    if (Item::Opened())
    {
        if (key == Key::_1)
        {
            Math::CircleIncrease<int>((int *)&active_control, 0, ActiveControl::Count - 1);
        }
        else if (key == Key::_2)
        {
            if (active_control == ActiveControl::Close)
            {
                active_control = ActiveControl::Increase;
                Close();
            }
            else
            {
                const DGovernor *data = ToDGovernor();

                int *value = data->value;

                if (active_control == ActiveControl::Increase && *value < data->max)
                {
                    *value = *value + 1;
                }
                else if (active_control == ActiveControl::Decrease && *value > data->min)
                {
                    *value = *value - 1;
                }
            }
        }
    }
    else
    {
        if (key == Key::_2)
        {
            Open();
        }
    }
}


void Governor::LongPressure() const
{
    if (Item::Opened() != this)
    {
        Open();
    }
//    else
//    {
//        Math::CircleIncrease<int>((int *)&active_control, 0, ActiveControl::Count - 1);
//    }
}


void Governor::DoubleClick() const
{

}


void TimeItem::DoubleClick() const
{

}
