// 2023/09/08 22:14:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/MemoryStorage.h"
#include "Hardware/Timer.h"
#include "Modules/W25Q80DV/W25Q80DV.h"


namespace MemoryStorage
{
    struct Page
    {
        static const int SIZE = W25Q80DV::SIZE_PAGE;

        Page(uint _address) : address(_address) { }

    private:

        uint address;
    };

    // ���� ��������� ����� ������, ����� ����� �������� � ���
    namespace Cache
    {
        static const int SIZE = 512;        // ������ ��� ������ ������ (��� 512 ������ �������� �� 3256 ��, ��� 8192 - �� 3149 ��)
        static uint8 buffer[SIZE] __attribute__((section("CCM_DATA")));

        static uint begin = (uint)-1;               // ������ ������ � ����

        static bool IsDirty()
        {
            return begin >= W25Q80DV::SIZE;
        }

        static uint End()
        {
            uint end = begin + SIZE;

            if (end > W25Q80DV::SIZE)
            {
                end = W25Q80DV::SIZE;
            }

            return end;
        }

        static void MarkAsDirty()
        {
            begin = (uint)-1;
        }

        // ���������� true, ���� �������� ������ �� ������ address, �������� size
        static bool Consist(uint address, int size)
        {
            if (IsDirty() || begin > address)
            {
                return false;
            }

            return address + (uint)size < End();
        }

        static void Fill(uint address)
        {
            begin = address;

            uint size = W25Q80DV::SIZE - begin;

            if (size > SIZE)
            {
                size = SIZE;
            }

            std::memcpy(buffer, MemBuffer<SIZE>().Read(begin), SIZE);
        }

        static void Read(uint address, void *out, int size)
        {
            if (!Consist(address, size))
            {
                Fill(address);
            }

            uint shift = address - begin;

            std::memcpy(out, &buffer[shift], (uint)size);
        }

        static void Write(uint address, const Measurements &meas)
        {
            W25Q80DV::WriteBuffer<sizeof(meas)>(address, &meas);

            if (Consist(address, sizeof(meas)))
            {
                MarkAsDirty();
            }
        }
    }

    static Record first_record((uint)-1);
    static Record last_record((uint)-1);
}


int Record::GetNumber()
{
    if (!MemoryStorage::last_record.IsExist())
    {
        return 0;
    }

    return MemoryStorage::last_record.GetMeasurements().number + 1;
}


void MemoryStorage::Init()
{
    uint time_start = TIME_MS;

    for (uint address = 0; address < W25Q80DV::SIZE; address += Record::SIZE)
    {
        Record record(address);

        if (record.IsExist() &&
            !record.IsEmpty() &&                // ���� ���� ��� ���-�� ���� ��������
            !record.IsErased())                 // � �� �����
        {
            if (record.IsCorrect())
            {                                   // ������������ ������ � ��������� ������
                if (!first_record.IsExist())
                {
                    first_record = record;
                }

                if (!last_record.IsExist())
                {
                    last_record = record;
                }

                if (record > last_record)
                {
                    last_record = record;
                }

                if (record < first_record)
                {
                    first_record = record;
                }
            }
            else
            {
                record.Erase();         // ������� ������������ ������
            }
        }
    }

    uint time = TIME_MS - time_start;

    time = time;
}


void MemoryStorage::Append(Measurements &meas)
{
    /*
    *       ������� ��������.
    *   1. ������ ������ ���������� ���������� �������, ������� ������������� ����� �������:
    *       - ���� ��������� ������, �� ���� ����� 1.
    *       - ���� � ��������� ��� ���� ������, �� ���� ����� ����� "����� ��������� ������ + 1"
    *   2. ���� ��������� ����� � ������:
    *       - ���� ������� ���, �� ���� ����� ����� 0
    *       - ���� ����� ��������� ������ ������ ������ ������ ������, �� ������ ����� ���� �� ��������� ������� � ������ ��������� �������
    */

    int number = Record::GetNumber();

    if (!last_record.IsExist())
    {
        Record(0).Write(meas, number);
    }
    els e
    {

    }
}


bool Record::IsEmpty()
{
    static bool first = true;
    static Measurements empty_meas;

    if (first)
    {
        first = false;

        std::memset(&empty_meas, 0xFF, sizeof(empty_meas));
    }

    Measurements &meas = GetMeasurements();

    return std::memcmp(&meas, &empty_meas, sizeof(meas)) == 0;
}


bool Record::IsErased()
{
    return GetMeasurements().number == 0;
}


bool Record::IsCorrect()
{
    Measurements &meas = GetMeasurements();

    return (meas.control_field == 0) && (meas.CalculateCRC() == meas.crc);
}


void Record::Erase()
{
    W25Q80DV::WriteUInt(address, 0);

    if (MemoryStorage::Cache::Consist(address, sizeof(uint)))
    {
        MemoryStorage::Cache::MarkAsDirty();
    }
}


Measurements &Record::GetMeasurements()
{
    return value_meas.GetMeasurements(address);
}


Measurements &Record::ValueMeasurements::GetMeasurements(uint addr)
{
    if (!is_valid)
    {
        is_valid = true;

        MemoryStorage::Cache::Read(addr, &measurements, sizeof(measurements));
    }

    return measurements;
}


void Record::Write(Measurements &meas, int number)
{
    meas.number = number;
    meas.crc = meas.CalculateCRC();

    MemoryStorage::Cache::Write(address, meas);

    MemoryStorage::last_record = *this;
}
