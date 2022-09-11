// 2022/08/24 14:56:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


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


// ��������� �������� �������� ������
namespace PoolSensors
{
    // �������� �������� ������
    void AppendReceivedData(uint8 *data, int size);

    void Update();
}
