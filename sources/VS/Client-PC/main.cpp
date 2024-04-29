// 2022/04/29 13:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Communicator/Communicator.h"
#include "Communicator/LAN/ServerTCP.h"
#include "Communicator/ComPort/ComPort.h"
#include "Frame.h"
#include "Data/ReceivedData.h"
#include "Display/Diagram/Diagram.h"
#include "Utils/Clock.h"
#include "Controls/ConsoleSCPI.h"
#include "Communicator/Server/Server.h"


using namespace std;


void Application::Init()
{
    Log::Init();

    Communicator::Init();
}


void Application::Update()
{
    Clock::Update();

    Communicator::Update();

    ReceivedData::Update();

    Diagram::Pool::self->UpdateArea();

    ServerMeasures::Update();
}
