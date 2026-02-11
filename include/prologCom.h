#ifndef PROLOGCOM_H
#define PROLOGCOM_H
/* Esta clase manejara la comunicacion entre PROLOG y C++ */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "SWI-cpp.h"
#include <vector>
#include <string>

class prologCom{
    private:
        PlEngine* mEngine;

    public:
        bool Escanear(const char* pCodigo,int** pArrCodes,int* pTamArr,char** pErrores);

        prologCom(char* argv0);
        ~prologCom();
};
#endif  