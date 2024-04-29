﻿// 2024/04/29 08:37:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/Server/Server.h"
#include "Frame.h"


namespace ServerMeasures
{
    static bool is_connected = false;

    static wxSocketClient *socket = nullptr;

    // Послан запрос на соединение. Ждём
    static bool wait_connection = false;
}


void ServerMeasures::Init()
{
    if (socket == nullptr)
    {
        socket = new wxSocketClient();

        socket->SetEventHandler(*Frame::self, SOCKET_ID);

        socket->SetNotify(wxSOCKET_CONNECTION_FLAG |
            wxSOCKET_INPUT_FLAG |
            wxSOCKET_LOST_FLAG);

        socket->Notify(true);
    }

    if (!socket->IsConnected() && !wait_connection)
    {
        wait_connection = true;

        wxIPaddress *addr;
        wxIPV4address addr4;
        addr = &addr4;

        wxString host_name;
        g_file_config->Read("ip", &host_name, "");

        int port = 0;
        g_file_config->Read("port", &port, 0);

        LOG_WRITE("Connect to %s:%d", host_name.c_str().AsChar(), port);

        addr->Hostname(host_name);
        addr->Service((uint16)port);

        socket->Connect(*addr, true);
    }
}


void ServerMeasures::DeInit()
{
    if (socket)
    {
        delete socket;
        socket = nullptr;
    }

    wait_connection = false;
    is_connected = false;
}


void ServerMeasures::Update()
{
    if (is_connected)
    {
        return;
    }

    Init();
}


void ServerMeasures::Send(TypeMeasure::E type, float value)
{
    if (!is_connected)
    {
        return;
    }

    /*
    * { "param":"20", "time":"30" }
    */

    wxDateTime time = wxDateTime::Now();

    wxString message = wxString::Format("{\"%s\":\"%f\",\"time\":\"%d-%d-%d %d:%d:%d\"}\r\n", TypeMeasure::GetTitle(type), value,
        time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    socket->Write(message.GetData(), message.Length());
}



void ServerMeasures::CallbackOnSocketEvent(wxSocketEvent &event)
{
    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        LOG_WRITE("wxSOCKET_INPUT");
        break;

    case wxSOCKET_LOST:
        LOG_WRITE("wxSOCKET_LOST");
        DeInit();
        break;

    case wxSOCKET_CONNECTION:
        LOG_WRITE("wxSOCKET_CONNECTION");
        wait_connection = false;
        is_connected = true;
        break;

    case wxSOCKET_OUTPUT:
        LOG_WRITE("wxSOCKET_OUTPUT");
        break;
    }
}
