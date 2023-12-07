// 2023/11/21 21:27:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Measure
{
    enum E
    {
        Temperature,    // �����������
        Pressure,       // ��������
        Humidity,       // ���������
        DewPoint,       // ����� ����
        Illumination,   // �����������
        Velocity,       // �������� �������
        Latitude,       // ������
        Longitude,      // �������
        Altitude,       // ������
        Azimuth,        // ������ - ���� �� ����������� �� �����
        Time,
        Count
    };

    struct Type
    {
        enum E
        {
            Double,     // ��, ����� �������
            Uint,       // ����� � �������� ����� 2000 �
            TypeCount
        };
    };

    union
    {
        double value_d;
        uint   value_32;
    };

    void Clear()
    {
        type = Type::TypeCount;
        name = E::Count;
    }

    bool IsDouble() const
    {
        return type == Type::Double;
    }

    double GetDouble() const
    {
        return value_d;
    }

    E GetName() const
    {
        return name;
    }

    Type::E GetType() const
    {
        return type;
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


private:

    void SetDouble(float value)
    {
        SetDouble((double)value);
    }

    void SetDouble(double value)
    {
        type = Type::Double;
        value_d = value;
    }

    Type::E type;

    E name;
};
