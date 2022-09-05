// 2022/04/29 16:44:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


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


namespace Display
{
    void SwitchMeasure(TypeMeasure::E);

    void Reset();
}
