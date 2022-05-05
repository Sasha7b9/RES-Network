// 2022/04/29 16:45:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Frame.h"
#include "Hardware/Timer.h"
#include "Utils/Text/String.h"


namespace Display
{
    bool show_measure[5] = { true, true, true, true, true };

    // ����� ������������ ��������
    wxBitmap bitmap(Display::WIDTH, Display::HEIGHT);

    // ����� ����� ��������
    wxMemoryDC memDC;

    wxPen pen = *wxWHITE_PEN;
    wxBrush brush = *wxWHITE_BRUSH;

    void BeginScene(Color);

    void EndScene();
}


void Display::Update()
{
    TimeMeterMS meter_fps;

    BeginScene(Color::BLACK);

    int x0 = 10;
    int dX = 125;
    int y0 = 15;
    int dY = 22;

    String<>("�������� :").Draw(x0, y0, Color::_1);         String<>("���").Draw(x0 + dX, y0);
    String<>("������������ :").Draw(x0, y0 + dY);           String<>("��").Draw(x0 + dX, y0 + dY);
    String<>("��������� :").Draw(x0, y0 + 4 * dY);          String<>("%%").Draw(x0 + dX, y0 + 4 * dY);
    String<>("�������� :").Draw(x0, y0 + 2 * dY);           String<>("�/�").Draw(x0 + dX, y0 + 2 * dY);
    String<>("����������� :").Draw(x0, y0 + 3 * dY);        String<>("��").Draw(x0 + dX, y0 + 3 * dY);

//    DrawMeasures();

//    DrawZones();

//    zoneFPS.string.SetFormat("%02d ms", meter_fps.ElapsedTime());

    EndScene();
}


void Display::BeginScene(Color color)
{
    memDC.SelectObject(bitmap);

    color.SetAsCurrent();

    memDC.DrawRectangle(0, 0, WIDTH, HEIGHT);
}


void Display::EndScene()
{
    memDC.SelectObject(wxNullBitmap);

    if (Frame::Self())
    {
        Frame::Self()->Refresh();
    }
}


void Display::SetMeasure(int , float )
{

}


void Display::SwitchMeasure(int type)
{
    show_measure[type] = !show_measure[type];
}
