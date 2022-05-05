// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


static const Choice chPressure
(
    "��������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Pressure], 2,
    "����", "���"
);

static const Choice chIllumination
(
    "������������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Illumination], 2,
    "����", "���"
);

static const Choice chVelocity
(
    "��������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Velocity], 2,
    "����", "���"
);

static const Choice chTemperature
(
    "�����������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Temperature], 2,
    "����", "���"
);

static const Choice chHumidity
(
    "���������", PageDisplay::PageMeasures::self, &gset.display.show_measure[TypeMeasure::Humidity], 2,
    "����", "���"
);

static void CloseMeasures()
{
    PageDisplay::PageMeasures::self->Close();
};


static const Button bCloseMeasures
(
    "�������", PageDisplay::PageMeasures::self, CloseMeasures
);


static const Item *itemsMeasures[] =
{
    &chPressure,
    &chIllumination,
    &chVelocity,
    &chTemperature,
    &chHumidity,
    &bCloseMeasures,
    nullptr
};


static uint8 ciPageMeasures = 0;

static const Page pageMeasures
(
    "���������",
    PageDisplay::self,
    itemsMeasures,
    &ciPageMeasures
);


void ClosePageDisplay()
{
    PageDisplay::self->Close();
}


static const Button bClosePageDisplay
(
    "�������", PageDisplay::self, ClosePageDisplay
);


static const Item *itemsDisplay[] =
{
    PageDisplay::PageMeasures::self,
    &bClosePageDisplay,
    nullptr
};


uint8 ciPageDisplay = 0;


static const Page pageDisplay
(
    "�������",
    PageMain::self,
    itemsDisplay,
    &ciPageDisplay
);


const Page *PageDisplay::self = &pageDisplay;
const Page *PageDisplay::PageMeasures::self = &pageMeasures;
