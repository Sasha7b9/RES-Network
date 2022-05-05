// 2022/05/05 12:27:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"


struct TypeItem
{
    enum E
    {
        Page,
        Choice,
        Count
    };
};


struct Page;


struct Item
{
    static const int WIDTH = Display::WIDTH - 1;
    static const int HEIGHT = 20;

    TypeItem::E type;
    pchar       title;
    const Page *keeper;

    Item(TypeItem::E _type, pchar _title, const Page *_keeper) : type(_type), title(_title), keeper(_keeper) { }

    virtual ~Item() {}

    pchar Title() const { return title; }

    virtual void Draw(int x, int y) const;

    virtual void DrawOpened(int x, int y) const {}

    virtual void DrawClosed(int x, int y) const {}

    bool Opened() const;

    bool IsPage() const { return type == TypeItem::Page; }

    Page *ReinterpetToPage() { return (Page *)this; }
};


struct Page : public Item
{
    static const int NUM_ITEMS_ON_SCREEN = 5;

    Item **items;

    uint8 currentItem;

    Page(pchar title, const Page *keeper, Item **_items) : Item(TypeItem::Page, title, keeper), items(_items) {}

    virtual void DrawOpened(int x, int y) const;

    // �������� ��������� ����
    void SelectNextItem();

    // �������� ��������� �������� �����
    void ChangeCurrentItem();

    void Open();

private:

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y) const;

    virtual void DrawClosed(int x, int y) const;

    // ������ ��������� ���� �� ������� ��������
    int FirstItemOnScreen() const;

    // ��������� ��������� ���� �� ������� ��������
    int LastItemOnScreen() const;

    int NumItems() const;
};


struct Choice : public Item
{
    uint8 *cell;
    uint8 count;
    pchar names[2];

    Choice(pchar title, const Page *keeper, uint8 *_cell, uint8 _count, pchar name0, pchar name1) :
        Item(TypeItem::Choice, title, keeper), cell(_cell), count(_count)
    {
        names[0] = name0;
        names[1] = name1;
    }

    virtual void DrawClosed(int x, int y) const;
};
