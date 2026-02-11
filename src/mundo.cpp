#include "mundo.h"

BEGIN_EVENT_TABLE(Mundo, wxPanel)

EVT_PAINT(Mundo::paintEvent)

END_EVENT_TABLE()

/* Representación de la dirección de la pared como entero */
/*
    1 --> Arriba
    2 --> Derecha
    4 --> Abajo
    8 --> Izquierda
*/

Mundo::Mundo(wxFrame* parent) : wxPanel(parent) {
    mRobot = wxBITMAP_PNG(Robot);
    if(!mRobot.IsOk()){
        std::cout<<"Error al cargar la imagen del robot =(";
    }
    mRobot9 = wxBITMAP_PNG(Robot90);
    if(!mRobot9.IsOk()){
        std::cout<<"Error al cargar la imagen del robot =(";
    }
    mRobot18 = wxBITMAP_PNG(Robot180);
    if(!mRobot18.IsOk()){
        std::cout<<"Error al cargar la imagen del robot =(";
    }
    mRobot27 = wxBITMAP_PNG(Robot270);
    if(!mRobot27.IsOk()){
        std::cout<<"Error al cargar la imagen del robot =(";
    }

    mPosRobot.x=0;
    mPosRobot.y=0;
    mDirRobot.x=1;
    mDirRobot.y=0;

    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            mParedes[i][j] = 0;
            mParedes2[i][j] = 0;
        }
    }

    for(int i = 0; i<100; i++){
        mParedes[i][0] = 1;
        mParedes[99][i] = 2;
        mParedes[i][99] = 4;
        mParedes[0][i] = 8;
        mParedes2[i][0] = 1;
        mParedes2[99][i] = 2;
        mParedes2[i][99] = 4;
        mParedes2[0][i] = 8;
    }
    mParedes[0][0] = 9;
    mParedes[99][0] = 3;
    mParedes[0][99] = 12;
    mParedes[99][99] = 6;
    mParedes2[0][0] = 9;
    mParedes2[99][0] = 3;
    mParedes2[0][99] = 12;
    mParedes2[99][99] = 6;
    
    mCamara[0].x = 1;
    mCamara[0].y = 1;
    mCamara[1].x = 28;
    mCamara[1].y = 23;
}

void Mundo::ReiniciarTodo(){
    for(int i = 0; i<100; i++){
        for(int j = 0; j<100; j++){
            mParedes2[i][j] = mParedes[i][j];
        }
    }
    mPosRobot.x=0;
    mPosRobot.y=0;
    mDirRobot.x=1;
    mDirRobot.y=0;
    paintNow();
}


void Mundo::paintEvent(wxPaintEvent & evt){
    wxPaintDC dc(this);
    render(dc);
}

void Mundo::paintNow(){
    wxClientDC dc(this);
    render(dc);
}

void Mundo::render(wxDC&  dc){
    int ancho,largo;
    GetSize(&ancho,&largo);
    //std::cout <<"An: "<<ancho<<" La: "<<largo<<"\n";
    /* Calcular camara */
    int cubosAncho = (ancho/30), cubosLargo = (largo/30);
    unsigned int offsetX = 0, offsetY = 0;
    if(mPosRobot.x >= cubosAncho){
        offsetX = (((mPosRobot.x-cubosAncho) / 4 )+1)*5;
    }
    if(mPosRobot.y >= cubosLargo){
        offsetY = (((mPosRobot.y-cubosLargo) / 4 )+1)*5;
    }

    dc.Clear(); //Limpia el dispositivo en contexto con la brocha de fondo actual

    for(int a=1; a<=cubosLargo; a++){
        dc.DrawText(wxString::Format(wxT("%i"),a+offsetY),12,(a*30)+8);
    }

    for(int a=1; a<=cubosAncho; a++){
        dc.DrawText(wxString::Format(wxT("%i"),a+offsetX),(a*30)+8,15);
    }

    dc.DrawLine( 0, 30, ancho, 30); 
    dc.DrawLine( 30, 0, 30, largo); 

    wxBrush brocha;
    long int mascara = 268435440,mascara2 = 268435456;
    dc.SetPen(wxNullPen); 
    
    for(int i=0; i<cubosLargo; i++){
        for(int j=0; j<cubosAncho; j++){
            int aux = (i+1)*30,aux1=(j+1)*30;
            if(i+offsetX >99 || j+offsetY >99) continue;
            if(mParedes2[i+offsetX][j+offsetY]&mascara2){
                long int color = (mParedes2[i+offsetX][j+offsetY]&mascara)>>4;
                brocha.SetColour(wxColour(color));
                dc.SetBrush(brocha);
                dc.DrawRectangle( aux+4, aux1+4, 22, 22);
            }
            if(mParedes2[i+offsetX][j+offsetY] & 2){
                dc.DrawLine(aux+30,aux1,aux+30,aux1+30);
            }
            if(mParedes2[i+offsetX][j+offsetY] & 4){
                dc.DrawLine(aux,aux1+30,aux+30,aux1+30);
            }
        }
    }

    dc.SetBrush(*wxBLACK_BRUSH);
    for(int i = 1; i<=cubosAncho; i++){
        for(int j=1; j<=cubosLargo; j++){
            dc.DrawCircle(i*30,j*30,3);
        }
    }
    
    switch (Orientacion(mDirRobot)){
        case 1:
            dc.DrawBitmap(mRobot27,((mPosRobot.x%cubosAncho)*30)+32,((mPosRobot.y%cubosLargo)*30)+32,true);
            break;

        case 2:
            dc.DrawBitmap(mRobot,((mPosRobot.x%cubosAncho)*30)+32,((mPosRobot.y%cubosLargo)*30)+32,true);
            break;
        
        case 4:
            dc.DrawBitmap(mRobot9,((mPosRobot.x%cubosAncho)*30)+32,((mPosRobot.y%cubosLargo)*30)+32,true);
            break;

        case 8:
            dc.DrawBitmap(mRobot18,((mPosRobot.x%cubosAncho)*30)+32,((mPosRobot.y%cubosLargo)*30)+32,true);
            break;
    }
}

bool Mundo::AvanzarRobot(){
    if(!FrenteLibre()){
        wxMessageBox("Choque con una pared =(","Resultado de consulta", wxOK | wxICON_INFORMATION);
        paintNow();
        return false;
    }
    mPosRobot += mDirRobot;
    if(mPosRobot.x <0 || mPosRobot.x >99 || mPosRobot.y<0 || mPosRobot.y>99){
        wxMessageBox("Choque con una pared =(","Resultado de consulta", wxOK | wxICON_INFORMATION);
        mPosRobot -= mDirRobot;
        return false; 
    }
    paintNow();
    return true;
}

void Mundo::GirarRobot(){
    mDirRobot = RotarDireccion(mDirRobot);
    paintNow();
}

wxPoint Mundo::RotarDireccion(wxPoint pDir){
    wxPoint regreso = pDir;
    if(regreso.x==1){
        regreso.x =0;
        regreso.y =1;
    }else if(regreso.y==1){
        regreso.x =-1;
        regreso.y =0;
    }else if(regreso.x==-1){
        regreso.x =0;
        regreso.y =-1;
    }else{
        regreso.x =1;
        regreso.y =0;
    }
    return regreso;
}

wxPoint Mundo::RotarDireccionIzq(wxPoint pDir){
    wxPoint regreso = pDir;
    if(regreso.x==1){
        regreso.x =0;
        regreso.y =-1;
    }else if(regreso.y==1){
        regreso.x =1;
        regreso.y =0;
    }else if(regreso.x==-1){
        regreso.x =0;
        regreso.y =1;
    }else{
        regreso.x =-1;
        regreso.y =0;
    }
    return regreso;
}

bool Mundo::FrenteLibre(){
    return !(mParedes2[mPosRobot.x][mPosRobot.y]&Orientacion(mDirRobot));
}

bool Mundo::DerechaLibre(){
    return !(mParedes2[mPosRobot.x][mPosRobot.y]&Orientacion(RotarDireccion(mDirRobot)));
}

bool Mundo::IzquierdaLibre(){
    return !(mParedes2[mPosRobot.x][mPosRobot.y]&Orientacion(RotarDireccionIzq(mDirRobot)));
}

void Mundo::Pintar(long int pColor){
    mParedes2[mPosRobot.x][mPosRobot.y] &= MASKPAREDES;
    mParedes2[mPosRobot.x][mPosRobot.y] |= (pColor << 4) | DIBUJAR;
    paintNow();
}

bool Mundo::ParadoEnColor(long int* pColor){
    if(mParedes2[mPosRobot.x][mPosRobot.y] & DIBUJAR){
        *pColor = (mParedes2[mPosRobot.x][mPosRobot.y]& MASKCOLOR)>>4;
        return true;
    }
    return false;
}

unsigned short int Mundo::Orientacion(wxPoint pDireccion){
    if(pDireccion.y==-1){
        return 1;
    }else if(pDireccion.x==1){
        return 2;
    }else if(pDireccion.y==1){
        return 4;
    }
    return 8;
}

unsigned short int Mundo::OrientacionActual(){
    return Orientacion(mDirRobot);
}

bool Mundo::CargarMapa(wxString dirMapa){
    std::ifstream file(dirMapa, std::ios::in);

    if (!file) {
        std::cout <<"No se pudo abrir el archivo";
        return false;
    }

    file.seekg (0, file.end);
    int tam = file.tellg();
    file.seekg (0, file.beg);

    if(tam < 10000) return false;

    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            file >> mParedes[j][i]; 
        }
    }
    ReiniciarTodo();
    file.close();

    return true;
}