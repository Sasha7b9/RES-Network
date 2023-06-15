// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


void ClosePageDisplay(bool)
{
    PageDisplay::self->Close();
}

DEF_BUTTN( bClosePageDisplay,
    "�������",
    *PageDisplay::self,
    ClosePageDisplay,
    nullptr
)

DEF_PAGE_1(pageDisplay, //-V1027
    "�������",
    pageMain,
    nullptr,
    bClosePageDisplay
)

const Page * const PageDisplay::self = (const Page *)&pageDisplay;
