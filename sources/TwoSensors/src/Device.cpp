// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Modules/HC12/HC12.h"
#include "Modules/BME280/BME280.h"
#include "Modules/ST7735/ST7735.h"
#include "Hardware/Timer.h"
#include "Hardware/InterCom.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Hardware/EnergySwitch.h"
#include "Storage/Storage.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"


namespace Device
{
    static void ProcessMeasure(const Measure &, uint time);
}


void Device::Init()
{
    HAL::Init();

    ST7735::Init();         // �������

    EnergySwitch::Init();

    BME280::Init();         // �����������, ��������, ���������, ����� ����

    HC12::Init();           // �����������

    Keyboard::Init();

    Beeper::Init();

    InterCom::SetDirection((Direction::E)(Direction::HC12 | Direction::Display));

    HAL_USART2::Init();

    W25Q80DV::Init();

    Storage::Init();
}


void Device::Update()
{
    Measure temp;
    Measure pressure;
    Measure humidity;
    Measure dew_point;
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

    Storage::SaveMeasures();

    SCPI::Update();
}


void Device::ProcessMeasure(const Measure &measure, uint time)
{
    if (measure.correct)
    {
        InterCom::Send(measure, time);

        Storage::AppendMeasure(measure);
    }
}
