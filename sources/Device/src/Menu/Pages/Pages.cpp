// 2022/05/05 12:26:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"


static void CloseMainPage()
{
    PageMain::self->Close();
}


static const Button bCloseMainPage
(
    "�������", PageMain::self, CloseMainPage
);


static const Item *items[] =
{
    PageDisplay::self,
    PageHC12::self,
    &bCloseMainPage,
    nullptr
};


static uint8 ciPageMain = 0;


static const Page pageMain
(
    "����",
    nullptr,
    items,
    &ciPageMain
);


const Page * const PageMain::self = &pageMain;
