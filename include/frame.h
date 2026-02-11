#ifndef FRAME_H
#define FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/stc/stc.h>
#endif

#include "prologCom.h"
#include "mundo.h"
#include <stack>

#define MASKOPCODE 2013265920
#define MASKARG1 117440512
#define MASKARG2 16777215
#define MASKRED 16711680
#define MASKGREEN 65280
#define MASKBLUE 255

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    int* mProgramaCompilado;
    int mTamPrograma;

    wxString mDirArchivo;

    prologCom* mComunicadorPROLOG;
    wxStyledTextCtrl* mCodigo;
    Mundo* mMapa;

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCompilar(wxCommandEvent& event);
    void OnEjecutar(wxCommandEvent& event);
    void OnAbrirCodigo(wxCommandEvent& event);
    void OnNuevoCodigo(wxCommandEvent& event);
    void OnGuardarCodigo(wxCommandEvent& event);
    void OnGuardarComo(wxCommandEvent& event);
    void OnCargarMapa(wxCommandEvent& event);

public:

    void iniciarComunicado(char* argv0);
};

enum
{
    ID_Hello = 1,
    TEXT_Main = 2,
    Panel_Main = 3,
    wxID_Proyecto = 4,
    wxID_Debug = 5,
    wxID_DebugG = 6,
    wxID_DebugC = 7,
    wxID_ProyectoE = 8,
    wxID_CARGARCODIGO=9,
    wxID_NUEVOCODIGO=10,
    wxID_GUARDARCODIGO=11,
    wxID_GUARDARCODIGOCOMO=12,
    wxID_CARGARMAPA=13,
};
#endif