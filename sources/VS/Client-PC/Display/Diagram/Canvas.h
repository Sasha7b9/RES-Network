// 2022/09/05 08:47:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"


/*
*   ����� ��������������� ���������
*/


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent, TypeMeasure::E);

    void SetSizeArea(int, int);

private:

    TypeMeasure::E type;

    void OnPaint(wxPaintEvent &);

    // ���������� ��������� �� ���� ��������
    void DrawAllSensors(wxClientDC &);

    // ���������� ��������� �� ������ �������
    void DrawSensor(wxClientDC &, const std::vector<DataPoint> &);
};
