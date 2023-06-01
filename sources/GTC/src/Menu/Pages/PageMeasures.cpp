// 2023/06/01 09:44:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


extern const DPage pageMain;


void ClosePageTemperature()
{
    PageMeasures::Temperature::self->Close();
}


DEF_BUTTN(bClosePageTemperature,
    "�������",
    *PageMeasures::Temperature::self,
    EmptyVV,
    ClosePageTemperature
)


DEF_CHOICE_2(chTemperature,
    "����������",
    *PageMeasures::Temperature::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::Temperature],
    "���", "��"
)


DEF_PAGE_2(pageTemperature,
    "�����������",
    *PageMeasures::self,
    EmptyVV,
    chTemperature,
    bClosePageTemperature
)

DEF_CHOICE_2(chHumidity,
    "���������",
    *PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::Humidity],
    "����", "���"
)

DEF_CHOICE_2(chPressure,
    "��������",
    *PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::Pressure],
    "����", "���"
)

DEF_CHOICE_2(chDewPoint,
    "����� ����",
    *PageMeasures::self,
    EmptyVV,
    gset.display.show_measure[TypeMeasure::DewPoint],
    "����", "���"
)

static void CloseMeasures()
{
    PageMeasures::self->Close();
};

DEF_BUTTN(bCloseMeasures,
    "�������",
    *PageMeasures::self,
    EmptyVV,
    CloseMeasures
)

DEF_PAGE_5(pageMeasures, //-V1027
    "���������",
    pageMain,
    EmptyVV,
    pageTemperature,
    chPressure,
    chHumidity,
    chDewPoint,
    bCloseMeasures
)

const Page *const PageMeasures::self = (const Page *)&pageMeasures;
const Page *const PageMeasures::Temperature::self = (const Page *)&pageTemperature;

