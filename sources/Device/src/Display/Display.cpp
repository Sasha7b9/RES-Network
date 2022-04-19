// Sasha7b9@tut.by (c)
#include "main.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Display/Colors.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"


void Display::Update()
{
    TimeMeterMS meter;
    static uint time = 0;

    Color::E color2 = Color::WHITE;
    Color::E color1 = Color::BLACK;

    BeginScene(Color::GRAY_25);

    Rectangle(140, 30).Fill(10, 5, Color::BLUE);

    static int y0 = 40;

    HLine(20).Draw(5, y0, color2);

    y0++;

    if (y0 == HEIGHT)
    {
        y0 = 40;
    }

    Font::Set(TypeFont::_8);

    String<>("�������� ������").Draw(40, 10, color1);

    int y = 45;
    int x = 30;
    int dY = 15;

    String<>("�������� : 100 ��a").Draw(x, y, Color::GREEN);
    String<>("������������ : 100 ����").Draw(x, y + dY);
    String<>("�������� : 10 ��/���").Draw(x, y + 2 * dY);
    String<>("����������� : 23 �").Draw(x, y + 3 * dY);
    String<>("��������� : 100%%").Draw(x, y + 4 * dY);

    String<>("%d ms", time).Draw(125, 25, color1);

    EndScene();

    time = meter.ElapsedTime();
}
