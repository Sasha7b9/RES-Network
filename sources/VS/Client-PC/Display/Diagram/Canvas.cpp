// 2022/09/05 08:47:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Diagram/Canvas.h"
#include "Data/Sensors.h"
#include <map>
#include <ctime>


using namespace std;


Canvas::Canvas(wxWindow *parent, TypeMeasure::E _type) : wxPanel(parent, wxID_ANY),
    type(_type)
{
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

    SetMinClientSize({ 100, 100 });
    SetClientSize(100, 100);
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxClientDC dc(this);

    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(wxPen(wxColor(0, 0, 0)));

    dc.DrawRectangle(GetClientRect());

    static const wxString labels[TypeMeasure::Count] =
    {
        "��������",
        "������������",
        "�����������",
        "���������",
        "��������"
    };

    dc.DrawText(labels[type], 1, 0);

    time_t now = ::time(0);

    tm time = *localtime(&now);

    int x = GetClientSize().GetWidth() - time.tm_sec % 10;
    int y = GetClientSize().GetHeight();

    dc.SetPen(wxPen(wxColor(200, 200, 200)));

    while (x > 0)
    {
        dc.DrawLine(x, 0, x, y);

        x -= 20;
    }

    DrawAllSensors(dc);

    Update();
}


void Canvas::SetSizeArea(int width, int height)
{
    SetMinClientSize({ width, height } );
    SetClientSize({ width, height });
}


void Canvas::DrawAllSensors(wxClientDC &dc)
{
    const map<uint, Sensor> &pool = Sensor::Pool::GetPool();

    if (pool.empty())
    {
        return;
    }

    for (auto element : pool)
    {
        const Sensor &sensor = element.second;

        const DataArray &measures = sensor.GetMeasures(type);

        if (measures.Size())
        {
            DrawSensor(dc, measures);
        }
    }
}


void Canvas::DrawSensor(wxClientDC &dc, const DataArray &)
{
}
