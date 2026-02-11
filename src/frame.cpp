#include "frame.h"

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Roberto")
{
    SetIcon( wxICON(ApplicationIcon) );
    wxTopLevelWindow::Maximize(true);
    wxImage::AddHandler(new wxPNGHandler);

    wxMenu *menuProyecto = new wxMenu;
    menuProyecto->Append(wxID_Proyecto, "Compilar", "Compilar programa");
    menuProyecto->Append(wxID_ProyectoE, "Ejecutar", "Ejecutar codigo maquina");
  
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_NUEVOCODIGO, "Nuevo", "Borra el programa actual e inicia uno nuevo");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_CARGARCODIGO, "Abrir archivo", "Abre el codigo de un archivo");
    menuFile->Append(wxID_CARGARMAPA, "Abrir mapa", "Abre un mapa");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_GUARDARCODIGO, "Guardar", "Guarda el codigo actual");
    menuFile->Append(wxID_GUARDARCODIGOCOMO, "Guardar como...", "Guarda el codigo actual como un nuevo archivo");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, "Salir", "Salir del programa");
    
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "Acerca de ...", L"Informaci\u00F3n acerca del programa");
    
    wxMenuBar *menuBar = new wxMenuBar;

    menuBar->Append(menuFile, "&Archivo");
    menuBar->Append(menuProyecto, "&Proyecto");
    menuBar->Append(menuHelp, "&Ayuda");
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText("Proyecto de PLP");

    Bind(wxEVT_MENU, &MyFrame::OnCompilar, this, wxID_Proyecto);
    Bind(wxEVT_MENU, &MyFrame::OnEjecutar, this, wxID_ProyectoE);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbrirCodigo, this, wxID_CARGARCODIGO);
    Bind(wxEVT_MENU, &MyFrame::OnNuevoCodigo, this, wxID_NUEVOCODIGO);
    Bind(wxEVT_MENU, &MyFrame::OnGuardarCodigo, this, wxID_GUARDARCODIGO);
    Bind(wxEVT_MENU, &MyFrame::OnGuardarComo, this, wxID_GUARDARCODIGOCOMO);
    Bind(wxEVT_MENU, &MyFrame::OnCargarMapa, this, wxID_CARGARMAPA);

    wxBoxSizer* topsizer = new wxBoxSizer( wxHORIZONTAL );
    
    mCodigo = new wxStyledTextCtrl(this, TEXT_Main, wxDefaultPosition, wxSize(100,150),wxTE_MULTILINE | wxTE_RICH | wxHSCROLL , wxTextCtrlNameStr);
    mCodigo->SetMarginType(0,wxSTC_MARGIN_NUMBER);
    mCodigo->SetMarginWidth(0,30);
    mCodigo->SetMarginWidth(1,0);
    mCodigo->SetMarginWidth(2,0);
    mCodigo->SetScrollWidth(100);

    mCodigo->StyleSetFontAttr(0,12,"",false,false,false);
    mMapa = new Mundo(this);
    topsizer->Add(mCodigo,1,wxEXPAND|wxALL, 0);
    topsizer->Add(mMapa,2,wxEXPAND|wxALL,0);

    SetSizer( topsizer );      // use the sizer for layout

    topsizer->SetSizeHints( this );   // set size hints to honour minimum size

    mProgramaCompilado = nullptr;
    mTamPrograma = 0;
}

void MyFrame::iniciarComunicado(char* argv0){
    mComunicadorPROLOG = new prologCom(argv0);
}

void MyFrame::OnExit(wxCommandEvent& event){
    delete mComunicadorPROLOG;
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event){
    wxMessageBox(L"Proyecto para la materia de principios de lenguajes de programaci\u00F3n por el alumno Luis Kevin Johedan Ram\u00EDrez Fuentes",
                 "Acerca de Roberto", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnCompilar(wxCommandEvent& event){
    wxString codigo = mCodigo->GetText();
    std::string codigoChido = codigo.ToStdString();
    if(mProgramaCompilado!=nullptr){
        free(mProgramaCompilado);
        mProgramaCompilado = nullptr;
    }
    char* errores = nullptr;
    if(!mComunicadorPROLOG->Escanear(codigoChido.c_str(),&mProgramaCompilado,&mTamPrograma,&errores)){
        if(errores!=nullptr){
            wxMessageBox(errores,"Resultado de consulta", wxOK | wxICON_ERROR);
        }else{
            wxMessageBox("Error de sintaxis","Resultado de consulta", wxOK | wxICON_ERROR);
        }
        return;
    }
    wxMessageBox("Programa compilado con exito","Resultado de consulta", wxOK | wxICON_INFORMATION);

}

void MyFrame::OnEjecutar(wxCommandEvent& event){
    if(mProgramaCompilado == nullptr){
        wxMessageBox("Todavia no se ha compilado el programa","Resultado de consulta", wxOK | wxICON_ERROR);
        return;
    }

    mMapa->ReiniciarTodo();

    std::stack<int> callStack;
    std::stack<unsigned int> varStack;
    unsigned int contadorPrograma =0;
    bool bandera = false;

    while(contadorPrograma <mTamPrograma){
        Sleep(10);
        unsigned int nuevoContador = contadorPrograma+1;
        unsigned int opcode = (mProgramaCompilado[contadorPrograma] & MASKOPCODE) >> 27;
        unsigned int argumento1 = (mProgramaCompilado[contadorPrograma] & MASKARG1)>>24;
        unsigned int argumento2 = (mProgramaCompilado[contadorPrograma] & MASKARG2);
        switch (opcode){
            case 1:
                if(!mMapa->AvanzarRobot()) return; // Choco con una pared
                break;
            
            case 2:
                mMapa->GirarRobot();
                break;

            {case 3:
                long int color2 = (argumento2&MASKGREEN) | ((argumento2&MASKRED)>>16) | ((argumento2&MASKBLUE)<<16);
                mMapa->Pintar(color2);
                break;}
            case 4:
                if(!bandera)nuevoContador = argumento2;
                break;
            case 5:
                nuevoContador = mTamPrograma;
                break;
            case 6:
                switch (argumento1){
                    case 0:
                        bandera = mMapa->FrenteLibre();   
                        if(argumento2==0){
                            bandera = !bandera;
                        } 
                        break;
                    case 1:
                        bandera = mMapa->DerechaLibre();
                        if(argumento2==0){
                            bandera = !bandera;
                        }
                        break;
                    case 2:
                        bandera = mMapa->IzquierdaLibre();
                        if(argumento2==0){
                            bandera = !bandera;
                        }
                        break;
                    case 3:
                        bandera = (argumento2 == mMapa->OrientacionActual());
                        break;
                    case 4:
                        long int color,color2 = (argumento2&MASKGREEN) | ((argumento2&MASKRED)>>16) | ((argumento2&MASKBLUE)<<16);
                        if(mMapa->ParadoEnColor(&color)){
                            bandera = (color2 == color);
                        }else{
                            bandera = false;
                        }
                        break;
                }
                break;
            case 7:
                nuevoContador = argumento2;
                break;
            case 8:
                if(bandera)nuevoContador = argumento2;
                break;
            case 9:
                callStack.push(contadorPrograma+2);
                break;
            case 10:
                nuevoContador = callStack.top();
                callStack.pop();
                break;
            case 11:
                varStack.push(argumento2);
                break;
            {case 12:
                unsigned int aux = varStack.top();
                varStack.pop();
                aux--;
                if(aux==0){
                    bandera = true;
                }else{
                    varStack.push(aux);
                }
                break;
            }
        }
        if(opcode != 6 && opcode != 12)bandera = false;
        contadorPrograma = nuevoContador;
        wxYield();
    }
    wxMessageBox(L"Ejecución terminada","Resultado de consulta", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnAbrirCodigo(wxCommandEvent& event){
    wxFileDialog *DialogoArchivos = new wxFileDialog(
		this, _("Abrir"), wxEmptyString, wxEmptyString,
		_("Archivos de texto (*.txt)|*.txt"),
		wxFD_OPEN);

	if (DialogoArchivos->ShowModal() == wxID_OK){
		mDirArchivo = DialogoArchivos->GetPath();

        mCodigo->LoadFile(mDirArchivo);

		SetTitle(wxString("Roberto - Archivo ") << DialogoArchivos->GetFilename());
	}
}

void MyFrame::OnNuevoCodigo(wxCommandEvent& event){
    mCodigo->ClearAll();

    mDirArchivo = wxT("C:/");

    SetTitle(_("Roberto - Sin titulo *"));
}

void MyFrame::OnGuardarCodigo(wxCommandEvent& event){
    mCodigo->SaveFile(mDirArchivo);
}

void MyFrame::OnGuardarComo(wxCommandEvent& event){
    wxFileDialog *DialogoSalvar = new wxFileDialog(
		this, _("Guardar como..."), wxEmptyString, wxEmptyString,
		_("Archivos de texto (*.txt)|*.txt"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	if (DialogoSalvar->ShowModal() == wxID_OK){
		mDirArchivo = DialogoSalvar->GetPath();
		mCodigo->SaveFile(mDirArchivo); 
		SetTitle(wxString("Roberto - Archivo ") << DialogoSalvar->GetFilename());
	}

	DialogoSalvar->Destroy();
}

void MyFrame::OnCargarMapa(wxCommandEvent& event){
    wxFileDialog *DialogoArchivos = new wxFileDialog(
		this, _("Abrir"), wxEmptyString, wxEmptyString,
		_("Archivos de texto (*.txt)|*.txt"),
		wxFD_OPEN);

	if (DialogoArchivos->ShowModal() == wxID_OK){
		wxString dirMapa = DialogoArchivos->GetPath();

        if(!mMapa->CargarMapa(dirMapa)){
            wxMessageBox("Error al cargar el mapa","Resultado de consulta", wxOK | wxICON_ERROR);
        }
    }
}
