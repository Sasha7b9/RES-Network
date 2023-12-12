// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Storage/Measures.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/HC12/HC12.h"
#include "Modules/BME280/BME280.h"
#include "Modules/CG-Anem/CG-Anem.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Hardware/Timer.h"
#include "Hardware/InterCom.h"
#include "Display/Display.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Display/StartScreen.h"
#include "Hardware/EnergySwitch.h"
#include "Modules/GY511/GY511.h"
#include "Modules/NEO-M8N/NEO-M8N.h"
#include "Storage/Storage.h"
#include "Menu/Menu.h"
#include "Tests/Tests.h"
#include <cmath>


namespace Device
{
    static void ProcessMeasure(const Measure &, uint time);

    static void RunTests();
}


void Device::Init()
{
    HAL::Init();

    ST7735::Init();         // �������

    EnergySwitch::Init();

    Settings::Load(&gset);

    BME280::Init();         // �����������, ��������, ���������, ����� ����

    GY511::Init();          // ������

    HC12::Init();           // �����������

    Keyboard::Init();

    W25Q80DV::Test::Run();  // ���������� ������

    Beeper::Init();

    InterCom::SetDirection((Direction::E)(Direction::CDC | Direction::HC12 | Direction::Display));

    HAL_USART2::Init();

    Storage::Init();

    RunTests();

//    HAL_IWDG::Init();
}


void Device::Update()
{
//    HAL_IWDG::Update();

    NEO_M8N::Update();

    Measure temp;
    Measure pressure;
    Measure humidity;
    Measure dew_point;
    Measure velocity;
    Measure azimuth;
    Measure latitude;
    Measure longitude;
    Measure altitude;

    uint time = TIME_MS;

    if (BME280::GetMeasures(&temp, &pressure, &humidity, &dew_point))
    {
        ProcessMeasure(temp, time);
        ProcessMeasure(pressure, time);
        ProcessMeasure(humidity, time);
        ProcessMeasure(dew_point, time);
    }

    if (CG_Anem::GetMeasure(&velocity))
    {
        ProcessMeasure(velocity, time);
    }

    if (GY511::GetMagnetic(&azimuth))
    {
        ProcessMeasure(azimuth, time);
    }

    NEO_M8N::GetMeasures(&latitude, &longitude, &altitude);

    ProcessMeasure(latitude, time);
    ProcessMeasure(longitude, time);
    ProcessMeasure(altitude, time);

    if (!Menu::IsOpened())
    {
        Beeper::Update();
    }

    Keyboard::Update();

    Display::Update(TIME_MS);

    HAL_ADC::GetVoltage();

    EnergySwitch::Update();
}


void Device::ProcessMeasure(const Measure &measure, uint time)
{
    if (measure.correct)
    {
        InterCom::Send(measure, time);

        Storage::AppendMeasure(measure);
    }
}


void Device::RunTests()
{
    Test::Settings::Run();
}
