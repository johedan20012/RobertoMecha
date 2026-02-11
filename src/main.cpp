#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "frame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxString argumento = wxGetApp().argv[0];
    char argv0[1024];
    strncpy(argv0, (const char*)argumento.mb_str(wxConvUTF8), 1023);
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    frame->iniciarComunicado(argv0);

    FreeConsole();
    return true;
}