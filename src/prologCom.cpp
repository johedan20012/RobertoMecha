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