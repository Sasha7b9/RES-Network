// 2022/08/24 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <map>


struct TypeMeasure
{
    enum E
    {
        Pressure,           // ��������
        Illumination,       // ������������
        Temperature,        // �����������
        Humidity,           // ���������
        Velocity,           // ��������
        Count
    };

    static pchar GetTitle(TypeMeasure::E);
    static pchar GetUnits(TypeMeasure::E);
};


struct DataPoint
{
    DataPoint(float);
    float value;
    std::tm time;
};


struct DataArray
{
    int Size() const { return (int)array.size(); }

    void PushBack(const DataPoint &point) { array.push_back(point); }

    std::vector<DataPoint> array;
};


class Sensor
{
public:

    Sensor(uint _id) : id(_id) {};

    void AppendMeasure(uint8 type, float value);

    const DataArray &GetMeasures(TypeMeasure::E type) const { return measures[type]; }

private:

    const uint id;

    DataArray measures[TypeMeasure::Count];

public:

    struct Pool
    {
        static void AppendMeasure(uint id, uint8 type, float value);

        static const std::map<uint, Sensor> &GetPool() { return pool; };

        static int CountSensors() { return pool.size(); };

    private:

        static std::map<uint, Sensor> pool;
    };
};
