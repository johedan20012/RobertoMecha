/*
RobertoMecha: Emulador de Robot programable con un lenguaje interpretado.
Copyright (C) 2026  Luis Kevin Johedan Ramirez Fuentes

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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