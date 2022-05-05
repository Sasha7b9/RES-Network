// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


static Choice chPressure
(
    "��������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Pressure], 2
);

static Choice chIllumination
(
    "������������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Illumination], 2
);

static Choice chVelocity
(
    "��������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Velocity], 2
);

static Choice chTemperature
(
    "�����������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Temperature], 2
);

static Choice chHumidity
(
    "���������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Humidity], 2
);


static Item *itemsMeasures[] =
{
    &chPressure,
    &chIllumination,
    &chVelocity,
    &chTemperature,
    &chHumidity,
    nullptr
};


static Page pageMeasures
{
    "���������",
    PageDisplay::self,
    itemsMeasures
};


static Item *itemsDisplay[] =
{
    PageDisplay::PageMeasures::self,
    nullptr
};


static Page pageDisplay
(
    "�������",
    MainPage::self,
    itemsDisplay
);


Page *PageDisplay::self = &pageDisplay;
Page *PageDisplay::PageMeasures::self = &pageMeasures;
