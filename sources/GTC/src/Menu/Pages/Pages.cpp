// 2022/05/05 12:26:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


static void CloseMainPage()
{
    PageMain::self->Close();
}


static void OnOpenClose_MainPage(bool open)
{
    if (open)
    {

    }
    else
    {
        gset.Save();
    }
}


DEF_BUTTN(bCloseMainPage,
    "�������",
    pageMain,
    EmptyFuncOpenClose,
    CloseMainPage
)

DEF_PAGE_5(pageMain, //-V1027
    "����",
    Page::Empty,
    OnOpenClose_MainPage,
    *PageMeasures::self,
    *PageDisplay::self,
    *PageHC12::self,
    *PageSystem::self,
    bCloseMainPage
)

const Page * const PageMain::self = (const Page *)&pageMain;
