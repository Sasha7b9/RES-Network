// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


static Choice chPressure
(
    "��������", PageDisplay::self, &gset.display.show_measure[TypeMeasure::Pressure], 2
);


static Choice chIllumination
(
    "������������", PageDisplay::self, &gset.display.show_measure[TypeMeasure::Illumination], 2
);


static Item *itemsMeasures[] =
{
    &chPressure,
    &chIllumination,
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
