// Sasha7b9@tut.by (c)
#include "defines.h"
#include "Hardware/InterCom.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/HC12/HC12.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
//#include <cstring>
//#include <cstdlib>


namespace InterCom
{
    Direction::E direction = Direction::_None;

    Buffer<uint8, 12> CreateMessage(TypeMeasure::E type, float value)
    {
        Buffer<uint8, 12> message;

        message[0] = 'A';
        message[1] = 'B';
        message[3] = 'C';

        uint8 buffer[5];
        buffer[0] = (uint8)type;
        memcpy(&buffer[1], &value, 4);

        uint hash = Math::CalculateHash(buffer, 5);

        memcpy(&message[4], &hash, 4);

        memcpy(&message[8], buffer, 5);

        return message;
    }
}



void InterCom::SetDirection(Direction::E dir)
{
    direction = dir;
}


void InterCom::Send(TypeMeasure::E type, float measure)
{
    static const pchar names[TypeMeasure::Count] =
    {
        "��������",
        "������������",
        "���������",
        "��������",
        "�����������"
    };

    String<> message("%s : %f", names[type], measure);

    if (direction & Direction::Display)
    {
        Display::SetMeasure(type, measure);
    }

    Buffer<uint8, 12> data = CreateMessage(type, measure);

    if (direction & Direction::CDC)
    {
        CDC::Transmit(data.Data(), 12);
    }

    if (direction & Direction::HC12)
    {
        HC12::Transmit(message.c_str());
    }
}
