#include "nui_unittest_client.h"
#include <nui/sys/window.h>
static void PumpMessage()
{
    MSG msg = { 0 };
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int GUIMain()
{
    using namespace nui;

    //MessageLoop loop;

    nui::Window::Initialize();
    UITestClient client;
    client.init();

    PumpMessage();
    //client.fini();
    return 0;
}

