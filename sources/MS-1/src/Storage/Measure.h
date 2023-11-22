// 2023/11/21 21:27:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Measure
{
    struct Type
    {
        enum E
        {
            Double,     // ��, ����� �������
            Uint,       // ����� � �������� ����� 2000 �
            Count
        };
    };

    struct Name
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
            MagneticX,
            MagneticY,
            MagneticZ,
            Time,
            Count
        };
    };

    union
    {
        double value_d;
        uint   value_32;
    };

    void Clear()
    {
        type = Type::Count;
        name = Name::Count;
    }

    bool IsDouble() const
    {
        return type == Type::Double;
    }

    void SetDouble(double value)
    {
        type = Type::Double;
        value_d = value;
    }

    void SetDouble(float value)
    {
        SetDouble((double)value);
    }

    double GetDouble() const
    {
        return value_d;
    }

    Name::E GetName() const
    {
        return name;
    }

    Type::E GetType() const
    {
        return type;
    }

private:

    Type::E type;

    Name::E name;
};
