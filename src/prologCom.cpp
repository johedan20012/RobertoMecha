#include "prologCom.h"

bool prologCom::Escanear(const char* pCodigo,int** pArrCodes,int* pTamArr,char** pErrores){
    PlTerm codigo(pCodigo);
    PlTermv av(4);
    av[0]=codigo;

    PlQuery compilar("compilar",av);
    if(compilar.next_solution()){
        PlTail lista2(av[3]);
        PlTerm termino;
        if(lista2.next(termino)){
            *pErrores = (char*)termino;
            return false;
        }

        PlTail lista(av[1]);
        wxString regreso;
        int i=0; 
        if(!PL_get_integer(av[2],pTamArr))return false;
        
        (*pArrCodes) = (int*)malloc(sizeof(int)*(*pTamArr));
        
        while(lista.next(termino)){
            if(i>=(*pTamArr))return false;
            
            if(termino.type() == PL_INTEGER){
                int numero = long(termino);
                (*pArrCodes)[i] = numero; 
            }else{
                std::cout<<"PROLOG regreso un No entero"<<"\n";
                return false;
            }
            i++;
        }
        return true;
    }
    std::cout<<"Error de sintaxis"<<"\n";
    return false;
}

prologCom::prologCom(char* argv0){
    mEngine = new PlEngine(argv0);
}

prologCom::~prologCom(){
    delete mEngine;
}