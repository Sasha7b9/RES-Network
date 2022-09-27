// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Display/Diagram/Diagram.h"
#include "Display/Grid/Grid.h"
#include "Display/Diagram/Canvas.h"


Frame *Frame::self = nullptr;


enum
{
    TOOL_OPEN,
    TOOL_SAVE,
    TOOL_NEW,

    TOOL_UNDO,
    TOOL_REDO,

    TOOL_VIEW_BRIEF,        // ����������� ��� �����������
    TOOL_VIEW_FULL,         // ������ ��� �����������

    MEAS_PRESSURE,          // ��������
    MEAS_ILLUMINATION,      // ������������
    MEAS_HUMIDITY,          // ���������
    MEAS_VELOCITY,          // ��������
    MEAS_TEMPERATURE,       // �����������

    ID_SPEED_1,
    ID_SPEED_2,
    ID_SPEED_5,
    ID_SPEED_30,
    ID_SPEED_60
};


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    SetIcon(wxICON(MAIN_ICON));

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, _("����"));

    wxMenu *menuSettings = new wxMenu();
    wxMenu *menuSpeed = new wxMenu();

    miSpeed1 = new wxMenuItem(menuSpeed, ID_SPEED_1, "1 ���", wxEmptyString, wxITEM_DROPDOWN);
    miSpeed2 = new wxMenuItem(menuSpeed, ID_SPEED_2, "2 ���", wxEmptyString, wxITEM_DROPDOWN);
    miSpeed5 = new wxMenuItem(menuSpeed, ID_SPEED_5, "5 ���", wxEmptyString, wxITEM_DROPDOWN);
    miSpeed30 = new wxMenuItem(menuSpeed, ID_SPEED_30, "30 ���", wxEmptyString, wxITEM_DROPDOWN);
    miSpeed60 = new wxMenuItem(menuSpeed, ID_SPEED_60, "60 ���", wxEmptyString, wxITEM_DROPDOWN);

    menuSpeed->Append(miSpeed1);
    menuSpeed->Append(miSpeed2);
    menuSpeed->Append(miSpeed5);
    menuSpeed->Append(miSpeed30);
    menuSpeed->Append(miSpeed60);

    menuSettings->AppendSubMenu(menuSpeed, "�������� ����������");

    Bind(wxEVT_MENU, &Frame::OnTimeScaleEvent, this, ID_SPEED_1);
    Bind(wxEVT_MENU, &Frame::OnTimeScaleEvent, this, ID_SPEED_2);
    Bind(wxEVT_MENU, &Frame::OnTimeScaleEvent, this, ID_SPEED_5);
    Bind(wxEVT_MENU, &Frame::OnTimeScaleEvent, this, ID_SPEED_30);
    Bind(wxEVT_MENU, &Frame::OnTimeScaleEvent, this, ID_SPEED_60);

    menuBar->Append(menuSettings, _("���������"));

    wxFrameBase::SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);

    Bind(wxEVT_MENU, &Frame::OnViewBrief, this, TOOL_VIEW_BRIEF);
    Bind(wxEVT_MENU, &Frame::OnViewFull, this, TOOL_VIEW_FULL);

    Bind(wxEVT_SIZE, &Frame::OnSize, this);

//    CreateFrameToolBar();

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(Grid::Create(this, FromDIP(wxSize(360, 400))));

    sizer->Add(Diagram::Pool::Create(this));

    SetSizer(sizer);

    SetClientSize(1024, 600);
    wxWindowBase::SetMinClientSize({ 800, 300 });
}


void Frame::CreateFrameToolBar()
{
    toolBar = CreateToolBar();

    AddTool(TOOL_VIEW_BRIEF, _T("������� ���"), "TOOL_VIEW_BRIEF");
    AddTool(TOOL_VIEW_FULL, _T("������ ���"), "TOOL_VIEW_FULL");

    toolBar->AddSeparator();

    AddTool(MEAS_PRESSURE, _T("��������"), "MEAS_PRESSURE");
    AddTool(MEAS_ILLUMINATION, _T("������������"), "MEAS_ILLUMINATION");
    AddTool(MEAS_VELOCITY, _T("��������"), "MEAS_VELOCITY");
    AddTool(MEAS_TEMPERATURE, _T("�����������"), "MEAS_TEMPERATURE");
    AddTool(MEAS_HUMIDITY, _T("���������"), "MEAS_HUMIDITY");

    Bind(wxEVT_MENU, &Frame::OnMeasurePressure, this, MEAS_PRESSURE);
    Bind(wxEVT_MENU, &Frame::OnMeasureIllumination, this, MEAS_ILLUMINATION);
    Bind(wxEVT_MENU, &Frame::OnMeasureHumidity, this, MEAS_HUMIDITY);
    Bind(wxEVT_MENU, &Frame::OnMeasureVelocity, this, MEAS_VELOCITY);
    Bind(wxEVT_MENU, &Frame::OnMeasureTemperature, this, MEAS_TEMPERATURE);

    toolBar->Realize();
}


void Frame::AddTool(int id, const wxString &label, pchar nameResource, pchar nameResourceDisabled)
{
    wxBitmap bitmap(nameResource, wxBITMAP_TYPE_BMP_RESOURCE);

    wxBitmap bitmapDisabled(nameResourceDisabled ? wxBitmap(nameResourceDisabled, wxBITMAP_TYPE_BMP_RESOURCE) : bitmap);

    toolBar->AddTool(id, label, bitmap, bitmapDisabled, wxITEM_NORMAL, label, label);
}


void Frame::OnTimeScaleEvent(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_SPEED_1)
    {
        int i = 0;
    }
}


void Frame::OnViewBrief(wxCommandEvent &)
{
}


void Frame::OnViewFull(wxCommandEvent &)
{

}


void Frame::OnMeasurePressure(wxCommandEvent &)
{

}


void Frame::OnMeasureIllumination(wxCommandEvent &)
{

}


void Frame::OnMeasureHumidity(wxCommandEvent &)
{

}


void Frame::OnMeasureVelocity(wxCommandEvent &)
{

}


void Frame::OnMeasureTemperature(wxCommandEvent &)
{

}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    self = nullptr;

    Close(true);

    OnClose();
}


void Frame::OnCloseWindow(wxCloseEvent &event)
{
    self = nullptr;

    event.Skip();

    OnClose();
}


void Frame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void Frame::OnSize(wxSizeEvent &event)
{
    Diagram::Pool::self->SetSizeArea(GetClientRect().width - Grid::self->GetSize().x, GetClientRect().height);

    wxSize size = { Grid::self->GetSize().GetWidth(), GetClientRect().height };

    Grid::self->SetMinClientSize(size);
    Grid::self->SetClientSize(size);
    Grid::self->SetSize(size);

    event.Skip();
}
