// 2022/05/05 13:14:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


static const DChoice chPressure =
{
    TypeItem::Choice,
    "��������",
    PageDisplay::PageMeasures::self,
    &gset.display.show_measure[TypeMeasure::Pressure], 2,
    "����", "���"
};

static const DChoice chIllumination =
{
    TypeItem::Choice,
    "������������",
    PageDisplay::PageMeasures::self,
    &gset.display.show_measure[TypeMeasure::Illumination], 2,
    "����", "���"
};

static const DChoice chVelocity =
{
    TypeItem::Choice,
    "��������",
    PageDisplay::PageMeasures::self,
    &gset.display.show_measure[TypeMeasure::Velocity], 2,
    "����", "���"
};

static const DChoice chTemperature =
{
    TypeItem::Choice,
    "�����������",
    PageDisplay::PageMeasures::self,
    &gset.display.show_measure[TypeMeasure::Temperature], 2,
    "����", "���"
};

static const DChoice chHumidity =
{
    TypeItem::Choice,
    "���������",
    PageDisplay::PageMeasures::self,
    &gset.display.show_measure[TypeMeasure::Humidity], 2,
    "����", "���"
};

static void CloseMeasures()
{
    PageDisplay::PageMeasures::self->Close();
};


static const DButton bCloseMeasures =
{
    TypeItem::Button,
    "�������", PageDisplay::PageMeasures::self, CloseMeasures
};


static const Item * const itemsMeasures[] =
{
    (Item *)&chPressure,
    (Item *)&chIllumination,
    (Item *)&chVelocity,
    (Item *)&chTemperature,
    (Item *)&chHumidity,
    (Item *)&bCloseMeasures,
    nullptr
};


static uint8 ciPageMeasures = 0;

static const DPage pageMeasures =
{
    TypeItem::Page,
    "���������",
    PageDisplay::self,
    itemsMeasures,
    &ciPageMeasures
};


void ClosePageDisplay()
{
    PageDisplay::self->Close();
}


static const DButton bClosePageDisplay =
{
    TypeItem::Button,
    "�������", PageDisplay::self, ClosePageDisplay
};


static const Item * const itemsDisplay[] =
{
    PageDisplay::PageMeasures::self,
    (Item *)&bClosePageDisplay,
    nullptr
};


uint8 ciPageDisplay = 0;


static const DPage pageDisplay =
{
    TypeItem::Page,
    "�������",
    PageMain::self,
    itemsDisplay,
    &ciPageDisplay
};


const Page * const PageDisplay::self = (const Page *)&pageDisplay;
const Page * const PageDisplay::PageMeasures::self = (const Page *)&pageMeasures;
