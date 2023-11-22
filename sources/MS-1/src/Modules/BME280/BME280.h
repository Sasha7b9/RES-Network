// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measure.h"


/*
* ��������� �����������, ���������, ��������
*/
namespace BME280
{
    bool Init();

    bool GetMeasures(Measure *temp, Measure *pressure, Measure *humidity, Measure *dew_point);
}
