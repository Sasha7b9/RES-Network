// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Frame.h"
#include "Display/Display.h"
#include <wx/statline.h>


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
    MEAS_TEMPERATURE        // �����������
};


class Screen : public wxPanel
{
public:
    Screen(wxWindow *parent) : wxPanel(parent)
    {
        SetMinSize({ Display::WIDTH, Display::HEIGHT });
        SetDoubleBuffered(true);
        Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    }

    void OnPaint(wxPaintEvent &)
    {
        wxPaintDC dc(this);

        dc.DrawBitmap(wxBitmap(Display::bitmap), 0, 0);
    }
};


static Screen *screen = nullptr;


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    SetMinSize({ 600, 600 });

    self = this;

    SetIcon(wxICON(MAIN_ICON));

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, _("����"));

    wxMenu *menuSettings = new wxMenu;
    menuBar->Append(menuSettings, _("���������"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    menuBar->Append(menuHelp, _("������"));

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);

    Bind(wxEVT_MENU, &Frame::OnViewBrief, this, TOOL_VIEW_BRIEF);
    Bind(wxEVT_MENU, &Frame::OnViewFull, this, TOOL_VIEW_FULL);

    Bind(wxEVT_PAINT, &Frame::OnPaint, this);

    CreateFrameToolBar();

    screen = new Screen(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(screen);
    SetSizer(sizer);
}


void Frame::CreateFrameToolBar()
{
    toolBar = CreateToolBar();

    AddTool(TOOL_VIEW_BRIEF, _T("������� ���"), "TOOL_VIEW_BRIEF");
    AddTool(TOOL_VIEW_FULL, _T("������ ���"), "TOOL_VIEW_FULL");

    toolBar->AddSeparator();

    AddTool(MEAS_PRESSURE, _T("��������"), "MEAS_PRESSURE");
    AddTool(MEAS_ILLUMINATION, _T("������������"), "MEAS_ILLUMINATION");
    AddTool(MEAS_HUMIDITY, _T("���������"), "MEAS_HUMIDITY");
    AddTool(MEAS_VELOCITY, _T("��������"), "MEAS_VELOCITY");
    AddTool(MEAS_TEMPERATURE, _T("�����������"), "MEAS_TEMPERATURE");

    toolBar->Realize();
}


void Frame::AddTool(int id, const wxString &label, pchar nameResource, pchar nameResourceDisabled)
{
    wxBitmap bitmap(nameResource, wxBITMAP_TYPE_BMP_RESOURCE);

    wxBitmap bitmapDisabled(nameResourceDisabled ? wxBitmap(nameResourceDisabled, wxBITMAP_TYPE_BMP_RESOURCE) : bitmap);

    toolBar->AddTool(id, label, bitmap, bitmapDisabled, wxITEM_NORMAL, label, label);
}


void Frame::OnViewBrief(wxCommandEvent &)
{

}


void Frame::OnViewFull(wxCommandEvent &)
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


void Frame::OnPaint(wxPaintEvent &)
{
    screen->Refresh();
}
