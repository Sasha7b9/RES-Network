// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Measures.h"
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
#include <cmath>


void Device::Init()
{
    HAL::Init();

    EnergySwitch::Init();

    gset.Load();

    gset.Reset();

    ST7735::Init();         // �������

    BME280::Init();         // �����������, ��������, ���������, ����� ����

    GY511::Init();          // ������

    NEO_M8N::Init();        // ����������

    HC12::Init();           // �����������

    Keyboard::Init();

    W25Q80DV::Test::Run();  // ���������� ������

    Beeper::Init();

    Beeper::Start(4000);

    InterCom::SetDirection((Direction::E)(Direction::CDC | Direction::HC12 | Direction::Display));
}


void Device::Update()
{
    if (Beeper::Running() && TIME_MS > 2000)
    {
        Beeper::Stop();
    }

    Measure temp;
    Measure pressure;
    Measure humidity;
    Measure dew_point;
    Measure velocity;
    Measure magneticX;
    Measure magneticY;
    Measure magneticZ;
    Measure latitude;
    Measure longitude;
    Measure altitude;

    if (BME280::GetMeasures(&temp, &pressure, &humidity, &dew_point))
    {
        InterCom::Send(TypeMeasure::Temperature, temp);
        InterCom::Send(TypeMeasure::Pressure, pressure);
        InterCom::Send(TypeMeasure::Humidity, humidity);
        InterCom::Send(TypeMeasure::DewPoint, dew_point);

        bool in_range = Measures::InRange(TypeMeasure::Temperature, temp.GetDouble()) &&
            Measures::InRange(TypeMeasure::Pressure, pressure.GetDouble()) &&
            Measures::InRange(TypeMeasure::Humidity, humidity.GetDouble()) &&
            Measures::InRange(TypeMeasure::DewPoint, dew_point.GetDouble());

        if (in_range)
        {
            Beeper::Stop();
        }
        else
        {
            Beeper::Start(100);
        }
    }

    if (CG_Anem::GetMeasure(&velocity))
    {
        InterCom::Send(TypeMeasure::Velocity, velocity);
    }

    if (GY511::GetMagnetic(&magneticX, &magneticY, &magneticZ))
    {
        InterCom::Send(TypeMeasure::MagneticX, magneticX);
        InterCom::Send(TypeMeasure::MagneticY, magneticY);
        InterCom::Send(TypeMeasure::MagneticZ, magneticZ);
    }

    if (NEO_M8N::GetMeasures(&latitude, &longitude, &altitude))
    {
        InterCom::Send(TypeMeasure::Latitude, latitude);
        InterCom::Send(TypeMeasure::Longitude, longitude);
        InterCom::Send(TypeMeasure::Altitude, altitude);
    }

    GY511::Update();

    Keyboard::Update();

    Display::Update();

    HAL_ADC::GetVoltage();

    EnergySwitch::Update();
}
