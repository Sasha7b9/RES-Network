// 2022/05/05 12:26:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"


static void CloseMainPage()
{
    PageMain::self->Close();
}


static Button bCloseMainPage
(
    "�������", PageMain::self, CloseMainPage
);


static Item *items[] =
{
    PageDisplay::self,
    PageHC12::self,
    &bCloseMainPage,
    nullptr
};


static Page pageMain
(
    "����",
    nullptr,
    items
);


Page *PageMain::self = &pageMain;
