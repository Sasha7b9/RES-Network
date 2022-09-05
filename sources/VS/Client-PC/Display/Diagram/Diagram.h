// 2022/09/05 08:53:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


/*
*   ������ � ��������
*/


class Diagram : public wxPanel
{
private:

    Diagram(wxWindow *parant);

public:

    struct Pool
    {
        static Diagram *Create(wxWindow *parent);

        static Diagram *GetFirst() { return first; };

    private:

        static Diagram *first;
    };
};
