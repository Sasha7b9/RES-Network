// 2022/08/24 14:56:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Display/Grid/Grid.h"
#include "Display/Diagram/Diagram.h"
#include <map>
#include <vector>


using namespace std;


map<uint, Sensor> Sensor::Pool::pool;


pchar TypeMeasure::GetTitle(TypeMeasure::E type)
{
    static const pchar titles[Count] =
    {
        "��������",
        "������������",
        "�����������",
        "���������",
        "��������"
    };

    return titles[type];
}


pchar TypeMeasure::GetUnits(TypeMeasure::E type)
{
    static const pchar units[Count] =
    {
        "���",
        "��",
        "�",
        "%%",
        "�/�"
    };

    return units[type];
}


void Sensor::Pool::AppendMeasure(uint id, uint8 type, float value)
{
    auto sensor = pool.find(id);

    if (sensor == pool.end())
    {
        pool.emplace(pair<uint, Sensor>(id, Sensor(id)));
    }

    sensor = pool.find(id);

    if (sensor != pool.end())
    {
        sensor->second.AppendMeasure(type, value);

        Grid::self->SetMeasure(id, type, value);
    }
}


void Sensor::AppendMeasure(uint8 type, float value)
{
    if (type < TypeMeasure::Count)
    {
        DataPoint point(value);

        if (measures[type].Size() && (measures[type].Last().time == point.time))
        {
            // � ��� ����� ��������� ��� �������� - �����������
            static int counter = 0;
            LOG_WRITE("Same time %d", counter++);
        }
        else
        {
            measures[type].PushBack(point);
        }
    }
    else
    {
        LOG_ERROR_TRACE("Bad type measure %d", type);
    }
}


DataPoint::DataPoint(float _value) : value(_value), time(Clock::CurrentTime())
{
}


float DataArray::Min(int from_end) const
{
    float result = 1e10f;

    int index = (int)array.size() - from_end;

    if (index < 0)
    {
        index = 0;
    }

    for (uint i = index; i < array.size(); i++)
    {
        if (array[i].value < result)
        {
            result = array[i].value;
        }
    }

    return result;
}


float DataArray::Max(int from_end) const
{
    float result = -1e10f;

    int index = (int)array.size() - from_end;

    if (index < 0)
    {
        index = 0;
    }

    for (uint i = index; i < array.size(); i++)
    {
        if (array[i].value > result)
        {
            result = array[i].value;
        }
    }

    return result;
}
