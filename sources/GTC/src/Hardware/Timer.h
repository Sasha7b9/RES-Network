#pragma once
#include "defines.h"


#define TIME_MS Timer::CurrentTime()


namespace Timer
{
    uint CurrentTime();
}



// ��������� ��� ������� �������
struct TimeMeterMS
{
    TimeMeterMS();

    // ���������� ������ �������
    void Reset();

    void Pause();

    void Continue();

    // ������� ����������� ������ � ������� ������ Reset()
    uint ElapsedTime();

    void WaitMS(uint);

    // ����� ms ����������� IsFinished() ���������� ������ true
    void FinishAfter(uint ms);

    bool IsFinished() const;

private:

    uint time_reset;        // �� ����� ������� ������������� ElapsedTime()
    uint time_pause;        // � ���� ������ ��������� �� �����
    uint time_finished;     // � ��� ����� IsFinished() ���������� ������ true
};
