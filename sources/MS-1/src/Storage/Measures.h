// 2023/06/12 16:46:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
//#include "Display/Display.h"


struct Measure
{
    enum E
    {
        Temperature,    // �����������
        Pressure,       // ��������
        Humidity,       // ���������
        DewPoint,       // ����� ����
        Velocity,       // �������� �������
        Latitude,       // ������
        Longitude,      // �������
        Altitude,       // ������
        Azimuth,        // ������ - ���� �� ����������� �� �����
        Count
    };

    union
    {
        double value_d;
        uint   value_32;
    };

    void Clear()
    {
        name = E::Count;
    }

    double GetDouble() const
    {
        return value_d;
    }

    E GetName() const
    {
        return name;
    }

    void Set(E _name, float value)
    {
        Set(_name, (double)value);
    }

    void Set(E _name, double value)
    {
        name = _name;
        SetDouble(value);
    }

    bool InRange() const;

    bool correct;

private:

    void SetDouble(float value)
    {
        SetDouble((double)value);
    }

    void SetDouble(double value)
    {
        value_d = value;
    }

    E name;
};


namespace Measures
{
    // ��������� ��������� ��������� �� ������
    void SetFixed(bool fixed);
    bool IsFixed();
}
