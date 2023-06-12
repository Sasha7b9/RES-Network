// 2022/05/05 12:26:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Measures.h"


extern const DPage pageMain;


static void FixateMeasures(bool)
{
    Measures::SetFixed(!Measures::IsFixed());
}


DEF_BUTTN(bFixateMeasures,
    "��������",
    pageMain,
    FixateMeasures
)


static void CloseMainPage(bool)
{
    PageMain::self->Close();
}


DEF_BUTTN(bCloseMainPage,
    "�������",
    pageMain,
    CloseMainPage
)


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


DEF_PAGE_6(pageMain, //-V1027
    "����",
    Page::Empty,
    OnOpenClose_MainPage,
    bFixateMeasures,
    *PageMeasures::self,
    *PageDisplay::self,
    *PageHC12::self,
    *PageSystem::self,
    bCloseMainPage
)

const Page * const PageMain::self = (const Page *)&pageMain;
