// 2023/06/01 09:44:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


#define DEF_GOVERNOR_MIN(_name, page_self, _min, _max, type) \
DEF_GOVERNOR(_name, "������ ���", *page_self, EmptyFuncOpenClose, _min, _max, gset.measures.min[type])

#define DEF_GOVERNOR_MAX(_name, page_self, _min, _max, type)    \
DEF_GOVERNOR(_name, "������ ����", *page_self, EmptyFuncOpenClose, _min, _max, gset.measures.max[type])


extern const DPage pageMain;


//-------------------------------------------------------------------------------------
void ClosePageTemperature()
{
    PageMeasures::Temperature::self->Close();
}

DEF_BUTTN(bClosePageTemperature,
    "�������",
    *PageMeasures::Temperature::self,
    EmptyFuncOpenClose,
    ClosePageTemperature
)

DEF_CHOICE_2(chTemperature,
    "����������",
    *PageMeasures::Temperature::self,
    EmptyFuncOpenClose,
    gset.display.show_measure[TypeMeasure::Temperature],
    "���", "��"
)


DEF_GOVERNOR_MIN(gTemperatureMin, PageMeasures::Temperature::self, -30, 60, TypeMeasure::Temperature)

DEF_GOVERNOR_MAX(gTemperatureMax, PageMeasures::Temperature::self, -30, 60, TypeMeasure::Temperature);


DEF_PAGE_4(pageTemperature,
    "�����������",
    *PageMeasures::self,
    EmptyFuncOpenClose,
    chTemperature,
    gTemperatureMin,
    gTemperatureMax,
    bClosePageTemperature
)


//------------------------------------------------------------------------------------
void ClosePagePressure()
{
    PageMeasures::Pressure::self->Close();
}

DEF_BUTTN(bClosePagePressure,
    "�������",
    *PageMeasures::Pressure::self,
    EmptyFuncOpenClose,
    ClosePagePressure
)

DEF_CHOICE_2(chPressure,
    "��������",
    *PageMeasures::Pressure::self,
    EmptyFuncOpenClose,
    gset.display.show_measure[TypeMeasure::Pressure],
    "����", "���"
)

DEF_GOVERNOR_MIN(gPressureMin, PageMeasures::Pressure::self, 225, 825, TypeMeasure::Pressure);

DEF_GOVERNOR_MAX(gPressureMax, PageMeasures::Pressure::self, 225, 825, TypeMeasure::Pressure);

DEF_PAGE_4(pagePressure,
    "��������",
    *PageMeasures::self,
    EmptyFuncOpenClose,
    chPressure,
    gPressureMin,
    gPressureMax,
    bClosePagePressure
)


//-------------------------------------------------------------------------------------
void ClosePageHumidity()
{
    PageMeasures::Humidity::self->Close();
}

DEF_BUTTN(bClosePageHumidity,
    "�������",
    *PageMeasures::Humidity::self,
    EmptyFuncOpenClose,
    ClosePageHumidity
)

DEF_CHOICE_2(chHumidity,
    "���������",
    *PageMeasures::Humidity::self,
    EmptyFuncOpenClose,
    gset.display.show_measure[TypeMeasure::Humidity],
    "���", "��"
)

DEF_GOVERNOR_MIN(gHumidityMin, PageMeasures::Humidity::self, 10, 98, TypeMeasure::Humidity);

DEF_GOVERNOR_MAX(gHumidityMax, PageMeasures::Humidity::self, 10, 98, TypeMeasure::Humidity);

DEF_PAGE_4(pageHumidity,
    "���������",
    *PageMeasures::self,
    EmptyFuncOpenClose,
    chHumidity,
    gHumidityMin,
    gHumidityMax,
    bClosePageHumidity
)


//------------------------------------------------------------------------------------
void ClosePageDewPoint()
{
    PageMeasures::DewPoint::self->Close();
}

DEF_BUTTN(bClosePageDewPoint,
    "�������",
    *PageMeasures::DewPoint::self,
    EmptyFuncOpenClose,
    ClosePageDewPoint
)

DEF_CHOICE_2(chDewPoint,
    "����� ����",
    *PageMeasures::DewPoint::self,
    EmptyFuncOpenClose,
    gset.display.show_measure[TypeMeasure::DewPoint],
    "����", "���"
)

DEF_GOVERNOR_MIN(gDewPointMin, PageMeasures::DewPoint::self, -100, 100, TypeMeasure::DewPoint);

DEF_GOVERNOR_MAX(gDewPointMax, PageMeasures::DewPoint::self, -100, 100, TypeMeasure::DewPoint);

DEF_PAGE_4(pageDewPoint,
    "����� ����",
    *PageMeasures::self,
    EmptyFuncOpenClose,
    chDewPoint,
    gDewPointMin,
    gDewPointMax,
    bClosePageDewPoint
)


static void CloseMeasures()
{
    PageMeasures::self->Close();
};

DEF_BUTTN(bCloseMeasures,
    "�������",
    *PageMeasures::self,
    EmptyFuncOpenClose,
    CloseMeasures
)

DEF_PAGE_5(pageMeasures, //-V1027
    "���������",
    pageMain,
    EmptyFuncOpenClose,
    pageTemperature,
    pagePressure,
    pageHumidity,
    pageDewPoint,
    bCloseMeasures
)

const Page *const PageMeasures::self = (const Page *)&pageMeasures;
const Page *const PageMeasures::Temperature::self = (const Page *)&pageTemperature;
const Page *const PageMeasures::Pressure::self = (const Page *)&pagePressure;
const Page *const PageMeasures::Humidity::self = (const Page *)&pageHumidity;
const Page *const PageMeasures::DewPoint::self = (const Page *)&pageDewPoint;
