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
    static wxMemoryDC memDC;

    static wxPen pen = *wxWHITE_PEN;
    static wxBrush brush = *wxWHITE_BRUSH;

    void BeginScene(Color &);

    void DrawText(int x, int y, const wxString &, Color &);

    void EndScene();

    void SetColor(Color &);
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


void Display::BeginScene(Color &color)
{
    memDC.SelectObject(bitmap);

    SetColor(color);

    memDC.DrawRectangle(0, 0, WIDTH, HEIGHT);
}


void Display::DrawText(int x, int y, const wxString &text, Color &color)
{
    SetColor(color);

    const wxFont font = memDC.GetFont();

    wxFont new_font = font;
    new_font.SetPointSize(20);

    memDC.SetFont(new_font);

    memDC.DrawText(text, x, y);
}


void Display::EndScene()
{
    memDC.SelectObject(wxNullBitmap);

    if (Frame::Self())
    {
        Frame::Self()->Refresh();
    }
}


void Display::SetColor(Color &color)
{
    pen.SetColour(color.value);
    brush.SetColour(color.value);

    memDC.SetBrush(brush);
    memDC.SetPen(pen);

    memDC.SetTextForeground(color.value);
}


void Display::SetMeasure(int , float )
{

}


void Display::SwitchMeasure(int type)
{
    show_measure[type] = !show_measure[type];
}
