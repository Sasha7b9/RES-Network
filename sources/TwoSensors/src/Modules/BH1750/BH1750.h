// 2022/02/21 15:39:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

/*
* ������ ������������
*/

namespace BH1750
{
    bool Init();

    bool GetMeasure(float *illumination);
}
