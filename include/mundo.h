#ifndef MUNDO_H
#define MUNDO_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <fstream>

#include "stdio.h"
#include <vector>

#define DIBUJAR 268435456
#define MASKCOLOR 268435440
#define MASKPAREDES 15

class Mundo : public wxPanel{
    private:
        wxBitmap mRobot;
        wxBitmap mRobot9;
        wxBitmap mRobot18;
        wxBitmap mRobot27;
        int mParedes[100][100];
        int mParedes2[100][100];
        wxPoint mPosRobot;
        wxPoint mDirRobot;
        wxPoint mCamara[2];

    public:
        Mundo(wxFrame* parent);

        void paintEvent(wxPaintEvent &evt);
        void paintNow();

        void render(wxDC& dc);
        
        DECLARE_EVENT_TABLE();
    public:
        bool AvanzarRobot();
        void GirarRobot();
        wxPoint RotarDireccion(wxPoint pDir);
        wxPoint RotarDireccionIzq(wxPoint pDir);
        bool FrenteLibre();
        bool DerechaLibre();
        bool IzquierdaLibre();
        void Pintar(long int pColor);
        bool ParadoEnColor(long int* pColor);
        unsigned short int Orientacion(wxPoint pDireccion);
        unsigned short int OrientacionActual();
        void ReiniciarTodo();
        bool CargarMapa(wxString dirMapa);
};


#endif