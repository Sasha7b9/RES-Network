// 2022/09/05 08:52:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Diagram/Diagram.h"
#include "Display/Diagram/Canvas.h"


Diagram *Diagram::Pool::first = nullptr;


Diagram *Diagram::Pool::Create(wxWindow *parent)
{
    first = new Diagram(parent);

    return first;
}


Diagram::Diagram(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(new Canvas(this));

    SetSizer(sizer);
}
