// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


DEF_CHOICE_2(chTemperature,
    "�����������",
    *PageDisplay::PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::Temperature],
    "����", "���"
)

DEF_CHOICE_2(chHumidity,
    "���������",
    *PageDisplay::PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::Humidity],
    "����", "���"
)

DEF_CHOICE_2(chPressure,
    "��������",
    *PageDisplay::PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::Pressure],
    "����", "���"
)

DEF_CHOICE_2(chDewPoint,
    "����� ����",
    *PageDisplay::PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::DewPoint],
    "����", "���"
)

static void CloseMeasures()
{
    PageDisplay::PageMeasures::self->Close();
};

DEF_BUTTN( bCloseMeasures,
    "�������",
    *PageDisplay::PageMeasures::self,
    EmptyVV,
    CloseMeasures
)

DEF_PAGE_5(pageMeasures, //-V1027
    "���������",
    *PageDisplay::self,
    EmptyVV,
    chTemperature,
    chPressure,
    chHumidity,
    chDewPoint,
    bCloseMeasures
)


void ClosePageDisplay()
{
    PageDisplay::self->Close();
}

DEF_BUTTN( bClosePageDisplay,
    "�������",
    *PageDisplay::self,
    EmptyVV,
    ClosePageDisplay
)

DEF_PAGE_2(pageDisplay, //-V1027
    "�������",
    pageMain,
    EmptyVV,
    pageMeasures,
    bClosePageDisplay
)

const Page * const PageDisplay::self = (const Page *)&pageDisplay;
const Page * const PageDisplay::PageMeasures::self = (const Page *)&pageMeasures;
