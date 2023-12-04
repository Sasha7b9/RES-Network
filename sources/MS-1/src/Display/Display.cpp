// 2022/04/20 08:53:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Display/Colors.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Utils/Text/Text.h"
#include "Display/Zones.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Measures.h"
#include <cstdlib>
#include <cstring>


namespace Display
{
    struct DMeasure
    {
        String<> current;

        Measure::Name::E name;
        Measure value;              // ��������� ������������� ��������
        int position;               // ������� �������������� �������
        uint time;                  // ����� ���������� ��������� ������� �������������� �������

        DMeasure(Measure::Name::E n) : name(n), position(0), time(0) {}

        void Draw(const int x, const int y, int size = 1);

        String<> Name();

        String<> Units();
    };

    static DMeasure measures[Measure::Name::Count] =
    {
        DMeasure(Measure::Name::Temperature),
        DMeasure(Measure::Name::Pressure),
        DMeasure(Measure::Name::Humidity),
        DMeasure(Measure::Name::DewPoint),
        DMeasure(Measure::Name::Illumination),
        DMeasure(Measure::Name::Velocity),
        DMeasure(Measure::Name::Latitude),
        DMeasure(Measure::Name::Altitude),
        DMeasure(Measure::Name::Longitude),
        DMeasure(Measure::Name::MagneticX),
        DMeasure(Measure::Name::MagneticY),
        DMeasure(Measure::Name::MagneticZ),
        DMeasure(Measure::Name::Time)
    };

    namespace Buffer
    {
        static uint8 buffer[WIDTH * HEIGHT / 2];       // ������������ ����

        uint8 GetPixels(int x, int y)
        {
            return buffer[(y * WIDTH + x) / 2];
        }

        uint8 *GetLine(int x, int y)
        {
            return &buffer[(y * WIDTH + x) / 2];
        }

        static void SetPoint(int x, int y)
        {
            if (x < 0) { return; }
            if (y < 0) { return; }
            if (x >= WIDTH) { return; }
            if (y >= HEIGHT) { return; }

            uint8 *pixels = &buffer[(y * WIDTH + x) / 2];

            uint8 value = *pixels;

            if (x & 0x1)
            {
                value &= 0x0F;
                value |= (Color::GetCurrent() << 4);
            }
            else
            {
                value &= 0xF0;
                value |= Color::GetCurrent();
            }

            *pixels = value;
        }

        static void DrawHLine(int y, int x1, int x2)
        {
            if ((x1 % 2) == 0 && (x2 % 2) == 0)
            {
                uint8 *first_byte = &buffer[y * Display::WIDTH / 2 + x1 / 2];

                uint8 value = (uint8)((uint8)(Color::GetCurrent()) + (Color::GetCurrent() << 4));

                std::memset(first_byte, value, (uint)(x2 - x1) / 2U);
            }
            else
            {
                for (int x = x1; x < x2; x++)
                {
                    Display::Buffer::SetPoint(x, y);
                }
            }
        }

        static void Fill(Color::E color)
        {
            uint8 value = (uint8)((int)(color) | (int)(color << 4));

            std::memset(buffer, value, WIDTH * HEIGHT / 2);
        }
    }
}


namespace Display
{
    bool need_redraw = true;

    static bool mode_compass = false;        // ���� true - ������ ������

    static ZoneFPS zoneFPS;

    static Zone *zones[] =
    {
        &zoneFPS,
        nullptr
    };

    void DrawZones();

    static void DrawMeasures();

    // ������� ���� ��������� �� ���� �����
    static void DrawBigMeasure();

    void DrawTime();

    static void DrawStar();

    // ����� ��������� ������ �������� �� �����. �.�. ��� ��� ������������ �� ����� �� ����������,
    // �� ��������� �� ���� ���� �� ����� ������, ������� ������������� �������������.
    static int CurrentDisplayMeasures();

    static void DrawCompass();
}


void HLine::Draw(int x0, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::DrawHLine(y, x0, x0 + width);
}


void VLine::Draw(int x, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int y = y0; y < y0 + height; y++)
    {
        Display::Buffer::SetPoint(x, y);
    }
}


void Point::Set(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::SetPoint(x, y);
}


void Rectangle::Fill(int x0, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int x = x0; x < x0 + width; x++)
    {
        VLine(height).Draw(x, y0);
    }
}


void Rectangle::DrawFilled(int x, int y, Color::E fill, Color::E draw)
{
    Rectangle(width, height).Draw(x, y, draw);

    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, fill);
}


void Rectangle::Draw(int x, int y, Color::E color)
{
    HLine(width).Draw(x, y, color);
    HLine(width).Draw(x, y + height);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width, y);
}


void Display::SetMeasure(const Measure &measure)
{
    Settings::SaveMeasure(measure);

    DMeasure &value = measures[measure.GetName()];

    if (value.value.GetDouble() == measure.GetDouble()) //-V550
    {
        if (value.current.Size())
        {
            return;
        }
    }

    value.position = 0;
    value.time = TIME_MS;
    value.value = measure;

    value.current.SetFormat("%.1f", (double)measure.GetDouble());
    value.current[6] = '\0';
}


void Display::DMeasure::Draw(const int x0, const int y0, int size)
{
    int width_zone = 38;
    int height_zone = 12;
    int y_zone = y0;

    Rectangle(width_zone, height_zone).Fill(x0, y_zone, Color::BLACK);

    Font::Text::DrawBig(x0, y0, size, current.c_str(), Color::WHITE);
}


void Display::DrawZones()
{
    for (int i = 0; true; i++)
    {
        Zone *zone = zones[i];

        if (zone == nullptr)
        {
            break;
        }

        zone->Draw();
    }
}


void Display::BeginScene(Color::E color)
{
    Buffer::Fill(color);
}


void Display::EndScene()
{
    DrawStar();

    ST7735::WriteBuffer(0, 0, WIDTH, HEIGHT);
}


void Display::Update()
{
    if (mode_compass)
    {
        DrawCompass();
    }
    else
    {
        TimeMeterMS meter_fps;

        need_redraw = true;

        if (Menu::Opened())
        {
            Menu::Draw();

            need_redraw = true;
        }
        else
        {
            if (gset.display.typeDisplaydInfo.IsAllMeasures())
            {
                if (need_redraw)
                {
                    BeginScene(Color::BLACK);
                }

                DrawMeasures();

                //DrawTime();

                if (need_redraw)
                {
                    EndScene();

                    need_redraw = false;
                }

                //DrawZones();
            }
            else
            {
                DrawBigMeasure();
            }

            zoneFPS.string.SetFormat("%02d ms", meter_fps.ElapsedTime());
        }
    }
}


void Display::DrawCompass()
{

}


void Display::DrawMeasures()
{
    Font::Set(TypeFont::_12_10);

    const int x0 = 3;

    // ������ ����� ����� ��������
    const int d_lines = (Display::HEIGHT - 5 * Font::Height()) / (5 + 1);

    const int y0 = d_lines;
    const int dY = d_lines + Font::Height();

    static const Measure::Name::E names[Measure::Name::Count] =
    {
        Measure::Name::Temperature,
        Measure::Name::Pressure,
        Measure::Name::Humidity,
        Measure::Name::DewPoint,
        Measure::Name::Illumination,
        Measure::Name::Velocity,
        Measure::Name::Latitude,
        Measure::Name::Longitude,
        Measure::Name::Altitude,
        Measure::Name::MagneticX,
        Measure::Name::MagneticY,
        Measure::Name::MagneticZ,
        Measure::Name::Time
    };

    int y = y0;

    for (int i = CurrentDisplayMeasures(); i < (CurrentDisplayMeasures() + 5); i++)
    {
        if (i < Measure::Name::Count)
        {
            int x = 93;
            int width = 30;
            int height = 15;

            if (need_redraw && measures[names[i]].current.Size())
            {
                String<>("%s", measures[names[i]].Name().c_str()).Draw(x0, y, Color::WHITE);
                measures[names[i]].Units().Draw(x + 41, y);
                measures[names[i]].Draw(x, y);

                ST7735::WriteBuffer(x - 1, y, width, height);
                y += dY;
            }
        }
    }
}


int Display::CurrentDisplayMeasures()
{
    const int num_displays = Measure::Name::Count / MEAS_ON_DISPLAY + 1;

    int secs = TIME_MS / 1000;

    int secs_5 = secs / 5;

    return (secs_5 % num_displays) * MEAS_ON_DISPLAY;
}


void Display::DrawTime()
{
    int width = 160;
    int height = 16;
    int y = 105;

    Font::Set(TypeFont::_12_10);

    Rectangle(width, height).Fill(4, y - 1, Color::BLACK);

    PackedTime time = HAL_RTC::GetTime();

    String<>("%02d:%02d:%02d", time.hours, time.minutes, time.seconds).Draw(5, 105, Color::WHITE);

    String<>("%02d:%02d:%04d", time.day, time.month, time.year + 2000).Draw(80, 105);

    ST7735::WriteBuffer(0, y, width, height);
}


void Display::DrawStar()
{
    if (((TIME_MS / 500) % 2) == 0)
    {
        String<>("*").Draw(156, 0, Color::WHITE);
    }
}


void Display::DrawBigMeasure()
{
    Font::Set(TypeFont::_8);

    BeginScene(Color::BLACK);

    static const int x[Measure::Name::Count] =
    {
        30,
        12,
        28,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10
    };

    DMeasure &measure = measures[gset.display.typeDisplaydInfo.value];

    Font::Text::DrawBig(x[measure.value.GetName()], 15, 2, measure.Name().c_str(), Color::_1);

    measures[measure.value.GetName()].Draw(27, 50, 4);

    Font::Text::DrawBig(68, 95, 2, measure.Units().c_str(), Color::_1);

    EndScene();
}


String<> Display::DMeasure::Name()
{
    static const pchar names[Measure::Name::Count] =
    {
        "�����������",
        "��������",
        "���������",
        "����� ����",
        "������������",
        "��������",
        "������",
        "�������",
        "������",
        "���� X",
        "���� Y",
        "���� Z",
        "�����"
    };

    String<> result(names[name]);

    return result;
}

String<> Display::DMeasure::Units()
{
    static const pchar units[Measure::Name::Count] =
    {
        "��",
        "���",
        "%%",
        "��",
        "��",
        "�/�",
        "�",
        "�",
        "�",
        "�",
        "�",
        "�",
        ""
    };

    return String<>(units[name]);
}
