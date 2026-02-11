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

/* Este compilador se divide en 3 secciones importantes:
1.-Analizador lexico(Scanning):
   En esta sección se divide el codigo en grupos de caracteres llamados
   tokens, tambien se especifica que tipo de token es cada uno.
2.Analizador sintactico(Parsing):
   En esta sección se reciben los tokens de la sección anterior y se
   construye su arbol de analisis sintactico, en base a las reglas de
   sintaxis definidas del lenguaje.
3.Generador de codigo intermedio('ensamblador'):
   En esta sección se transforma el arbol de analisis sintactico en una secuencia de
   codigos tipo "ensamblador" de una maquina abstracta, el cual servira para la ejecución
   del codigo.
4.Traducción de ensamblador a codigo maquina:
   Esta parte traduce todo la secuencia de codigos "ensamblador" en codigo maquina.
*/

/* Esta es la parte del scanning */
escanear(Cadena, Res, Errores):-
   atom_chars(Cadena, ListCarac),!,
   tokenizar(Res,Errores,ListCarac,[]).

tokenizar([Token|Tokens],Errores) -->
   espacio,
   token(Token,Errores1),!,
   tokenizar(Tokens,Errores2),
   {append(Errores1,Errores2,Errores)}.

tokenizar([],[]) --> espacio, [].

espacio --> [Espacio], { code_type(Espacio, space) }, espacio.
espacio --> [Espacio], { code_type(Espacio, end_of_line)}, espacio.
espacio --> [].

token(keyword(Token),[]) --> palabraReservada(ListaChar), {atom_chars(Token,ListaChar)}.
token(nombre(Token),[]) --> palabra2(ListaChar), {atom_chars(Token,ListaChar)}.
token(numeroHex(Token),[]) --> gatoNumHex(ListaChar), {number_chars(Token,ListaChar)}.
token(numero(Token),[]) --> numero(ListaChar), {number_chars(Token,ListaChar)}.
token(especial(Token),[]) --> caractEsp([Token]).
token('',[['Caracter inesperado',Char]]) --> [Char], {\+code_type(Char, end_of_line)}, {\+code_type(Char, space) }.

palabraReservada(['d','e','f','i','n','i','r']) --> ['d','e','f','i','n','i','r'].
palabraReservada(['c','o','m','o']) --> ['c','o','m','o'].
palabraReservada(['h','a','c','e','r']) --> ['h','a','c','e','r'].
palabraReservada(['r','e','p','e','t','i','r']) --> ['r','e','p','e','t','i','r'].
palabraReservada(['m','i','e','n','t','r','a','s']) --> ['m','i','e','n','t','r','a','s'].
palabraReservada(['e','n','t','o','n','c','e','s']) --> ['e','n','t','o','n','c','e','s'].
palabraReservada(['v','e','c','e','s']) --> ['v','e','c','e','s'].
palabraReservada(['s','i']) --> ['s','i'].
palabraReservada(['s','i','n','o']) --> ['s','i','n','o'].

caractEsp([Char]) --> [Char], {Char=')'}.
caractEsp([Char]) --> [Char], {Char='('}.
caractEsp([Char]) --> [Char], {Char='{'}.
caractEsp([Char]) --> [Char], {Char='}'}.
caractEsp([Char]) --> [Char], {Char='.'}.

palabra2([Letra|Palabra]) --> letra(Letra), palabra(Palabra).
palabra2([Letra]) --> letra(Letra).

palabra([Letra|Palabra]) --> letra(Letra), palabra(Palabra).
palabra([Digito|Palabra]) --> digito(Digito), palabra(Palabra).
palabra([Menos|Palabra]) --> [Menos], {Menos='-'}, palabra(Palabra).

palabra([Letra]) --> letra(Letra).
palabra([Digito]) --> digito(Digito).

letra(Letra) --> [Letra], { code_type(Letra, alpha) }.

numero([Digito|Numero]) --> digito(Digito), numero(Numero).
numero([Digito]) --> digito(Digito).

gatoNumHex(['0','x'|NumeroHex]) --> gato(_), numeroHex(NumeroHex).

gato(Gato) --> [Gato], {Gato = '#'}.

numeroHex([D1,D2,D3,D4,D5,D6]) -->digitoHex(D1),digitoHex(D2),digitoHex(D3),digitoHex(D4),digitoHex(D5),digitoHex(D6).

digitoHex(Digito) --> digito(Digito).
digitoHex(Digito) --> [Digito], {Digito='a'}.
digitoHex(Digito) --> [Digito], {Digito='A'}.
digitoHex(Digito) --> [Digito], {Digito='b'}.
digitoHex(Digito) --> [Digito], {Digito='B'}.
digitoHex(Digito) --> [Digito], {Digito='c'}.
digitoHex(Digito) --> [Digito], {Digito='C'}.
digitoHex(Digito) --> [Digito], {Digito='d'}.
digitoHex(Digito) --> [Digito], {Digito='D'}.
digitoHex(Digito) --> [Digito], {Digito='e'}.
digitoHex(Digito) --> [Digito], {Digito='E'}.
digitoHex(Digito) --> [Digito], {Digito='f'}.
digitoHex(Digito) --> [Digito], {Digito='F'}.

digito(Digito) --> [Digito], { code_type(Digito, digit) }.

/* Esta es la parte del parsing */

analisisSintactico(Cadena,Res,Errores) :-
   escanear(Cadena,Tokens,Errores1),!,
   (Errores1 = [] ->
      programa(Res,[avanzar,girar,apagar],_,Errores2,Tokens,[]),
      Errores = Errores2
   ; Errores = Errores1
   ).

funcionDefinida(F,[F|_]).
funcionDefinida(F,[_|T]) :- funcionDefinida(F,T).

programa(ListDeclProc,LSIn,LSOut,Errores) -->
   listDeclProc(ListDeclProc,LSIn,LSOut),
   ({funcionDefinida(inicio,LSOut)} ; {Errores2 = [['No se definio','inicio']]}),
   ({Errores2=[]} ->
      {Errores = []}
   ;  {Errores = Errores2}
   ).

listDeclProc([DeclProc|ListDeclProc],LSIn,LSOut) -->
   declProc(DeclProc,LSIn,LSOut1),
   listDeclProc(ListDeclProc,LSOut1,LSOut).
listDeclProc([],LSIn,LSIn) --> [].

declProc(proc(Nombre,ListExpr),LSIn,LSNew) -->
   [keyword('definir')],
   [nombre(Nombre)],
   {\+funcionDefinida(Nombre,LSIn)},
   {LSNew = [Nombre|LSIn]},
   [keyword('como')],
   [especial('{')],
   listaExpresion(ListExpr,LSNew),
   [especial('}')].

listaExpresion([Expresion|Lista],LSIn) -->
   expresion(Expresion,LSIn),
   listaExpresion(Lista,LSIn).
listaExpresion([],_) --> [].

expresion(Expresion,LSIn) --> llamadaFuncion(Expresion,LSIn).
expresion(Expresion,LSIn) --> controlSi(Expresion,LSIn).
expresion(Expresion,LSIn) --> controlMientras(Expresion,LSIn).
expresion(Expresion,LSIn) --> controlRepetir(Expresion,LSIn).

llamadaFuncion(fun(Nombre),LSIn) -->
   [nombre(Nombre)],
   [especial('.')],
   {funcionDefinida(Nombre,LSIn)}.

llamadaFuncion(fun(pintar,Parametro),_) -->
   [nombre('pintar')],
   [especial('(')],
   color(Parametro),
   [especial(')')],
   [especial('.')].

color(Color) --> [numeroHex(Color)].
color(0x0000FF) --> [nombre('azul')].
color(0xFF0000) --> [nombre('rojo')].
color(0x00FF00) --> [nombre('verde')].
color(0xFFFFFF) --> [nombre('blanco')].
color(0x000000) --> [nombre('negro')].
color(0xFF00FF) --> [nombre('rosa')].
color(0xFFFF00) --> [nombre('amarillo')].
color(0x663300) --> [nombre('cafe')].
color(0x585F5F) --> [nombre('gris')].
color(0xFF6600) --> [nombre('naranja')].
color(0x660066) --> [nombre('morado')].

controlSi(si(Condicion,ListExpr,LisExprElse),LSIn) -->
   [keyword('si')],
   condicion(Condicion),
   [keyword('entonces')],
   [especial('{')],
   listaExpresion(ListExpr,LSIn),
   [especial('}')],
   elseSi(LisExprElse,LSIn).

elseSi(ListExprElse,LSIn) -->
   [keyword('sino')],
   [especial('{')],
   listaExpresion(ListExprElse,LSIn),
   [especial('}')].

elseSi([],_) --> [].

controlMientras(mientras(Condicion,ListExpresion),LSIn) -->
   [keyword('mientras')],
   condicion(Condicion),
   [keyword('hacer')],
   [especial('{')],
   listaExpresion(ListExpresion,LSIn),
   [especial('}')].

controlRepetir(repetir(Veces,ListExpresion),LSIn) -->
   [keyword('repetir')],
   [numero(Veces)],
   [keyword('veces')],
   [especial('{')],
   listaExpresion(ListExpresion,LSIn),
   [especial('}')].

condicion([ClausulaY|ClausulasY]) --> clausulaY(ClausulaY),clausulasY(ClausulasY).

clausulasY([ClausulaY|ClausulasY]) --> [nombre('o')],clausulaY(ClausulaY),clausulasY(ClausulasY).
clausulasY([]) --> [].

clausulaY([ClausulaNo|ClausulasNo]) --> clausulaNo(ClausulaNo),clausulasNo(ClausulasNo).

clausulasNo([ClausulaNo|ClausulasNo]) -->[nombre('y')],clausulaNo(ClausulaNo),clausulasNo(ClausulasNo).
clausulasNo([]) --> [].

clausulaNo(no(ClausulaAtomica)) --> [nombre('no')], clausulaAtomica(ClausulaAtomica).
clausulaNo(ClausulaAtomica) --> clausulaAtomica(ClausulaAtomica).

clausulaAtomica('fl') --> [nombre('frente-libre')].
clausulaAtomica('fb') --> [nombre('frente-bloqueado')].
clausulaAtomica('dl') --> [nombre('derecha-libre')].
clausulaAtomica('db') --> [nombre('derecha-bloqueada')].
clausulaAtomica('il') --> [nombre('izquierda-libre')].
clausulaAtomica('ib') --> [nombre('izquierda-bloqueada')].
clausulaAtomica('on') --> [nombre('orientado-al-norte')].
clausulaAtomica('os') --> [nombre('orientado-al-sur')].
clausulaAtomica('oe') --> [nombre('orientado-al-este')].
clausulaAtomica('oo') --> [nombre('orientado-al-oeste')].
clausulaAtomica(consul(pc,Parametro)) -->
   [nombre('parado-en-color')],
   [especial('(')],
   color(Parametro),
   [especial(')')].

/*
Esta es la parte de generación de codigo "ensamblador"

Esta parte recibe un parse tree de la parte de parsing y genera
una lista con comandos tipo "ensamblador" para la maquina abstracta
*/

generarEnsamblador(Cadena,[['JMP','Finicio']|Res],Errores) :-
   analisisSintactico(Cadena,Res1,Errores1),!,
   (Errores1 = [] ->
      traducir_Listproc(Res1,Res,[0,0,0],_)
   ; Errores = Errores1
   ).

traducir_Listproc([Proc|ListProc],Res,CIn,COut) :-
   traducir_Proc(Proc,Res1,CIn,COut1),
   traducir_Listproc(ListProc,Res2,COut1,COut),
   append(Res1,Res2,Res).

traducir_Listproc([],[],CIn,CIn).

traducir_Proc(proc(Nombre,ListExpresion),[['BRN',NombreBranch]|Res],CIn,COut) :-
   atom_concat('F', Nombre, NombreBranch),
   traducir_listExpresion(ListExpresion,Res1,CIn,COut),
   traducir_finalDeProc(NombreBranch,InstruccionFinal),
   append(Res1,[InstruccionFinal],Res).

traducir_finalDeProc('Finicio',['APA']).
traducir_finalDeProc(_,['PST']).

traducir_listExpresion([Expresion|ListExpresion],Res,CIn,COut) :-
   traducir_expresion(Expresion,Res1,CIn,COut1),
   traducir_listExpresion(ListExpresion,Res2,COut1,COut),
   append(Res1,Res2,Res).
traducir_listExpresion([],[],CIn,CIn).

traducir_expresion(Expresion,Res,CIn,CIn) :-
   traducir_funcion(Expresion,Res).
traducir_expresion(Expresion,Res,CIn,COut):-
   traducir_controlSi(Expresion,Res,CIn,COut).
traducir_expresion(Expresion,Res,CIn,COut):-
   traducir_controlMientras(Expresion,Res,CIn,COut).
traducir_expresion(Expresion,Res,CIn,COut):-
   traducir_controlRepetir(Expresion,Res,CIn,COut).

traducir_funcion(fun(avanzar),[['AVA']]).
traducir_funcion(fun(girar),[['GIR']]).
traducir_funcion(fun(apagar),[['APA']]).
traducir_funcion(fun(pintar,Color),[['PIN',Color]]).
traducir_funcion(fun(Nombre),[['AST'],['JMP',NombreBranch]]):- atom_concat('F', Nombre, NombreBranch).

traducir_controlSi(si(Condicion,ListExpresion,ListExpresionElse),Res,[CIf|CT],COut):-
   atom_concat('IFExp',CIf,BranchExp),
   atom_concat('IFElse',CIf,BranchElse),
   atom_concat('IFSal',CIf,BranchSal),
   CIfNew is CIf+1,
   traducir_condicion(Condicion,Res1,BranchExp,[CIfNew|CT],COut1),
   traducir_listExpresion(ListExpresion,Res2,COut1,COut2),
   traducir_listExpresion(ListExpresionElse,Res3,COut2,COut),
   append(Res1,[['JMP',BranchElse],['BRN',BranchExp]|Res2],Res4),
   append(Res4,[['JMP',BranchSal],['BRN',BranchElse]|Res3],Res5),
   append(Res5,[['BRN',BranchSal]],Res).

traducir_controlMientras(mientras(Condicion,ListExpr),[['BRN',BranchCond]|Res],[CIf,CCond,CMi],COut):-
   atom_concat('WhileCond',CMi,BranchCond),
   atom_concat('WhileExp',CMi,BranchExp),
   atom_concat('WhileSal',CMi,BranchSal),
   CMiNew is CMi+1,
   traducir_condicion(Condicion,Res1,BranchExp,[CIf,CCond,CMiNew],COut1),
   traducir_listExpresion(ListExpr,Res2,COut1,COut),
   append(Res1,[['JMP',BranchSal],['BRN',BranchExp]|Res2],Res3),
   append(Res3,[['JMP',BranchCond],['BRN',BranchSal]],Res).

traducir_controlRepetir(repetir(Veces,ListExpr),[['CVA',Veces],['BRN',BranchRep]|Res],[CIf,CCond,CMi],COut):-
   atom_concat('ForRep',CMi,BranchRep),
   CMiNew is CMi+1,
   traducir_listExpresion(ListExpr,Res1,[CIf,CCond,CMiNew],COut),
   append(Res1,[['RVA'],['BNQ',BranchRep]],Res).

traducir_condicion(Condicion,Res,BranchExp,CIn,COut):-
   traducir_clausulasY(Condicion,Res,BranchExp,CIn,COut).

traducir_clausulasY([ClausulaY|ClausulasY],Res,BranchExp,[CIf,CCond|CT],COut):-
   CCondNew is CCond+1,
   atom_concat('Cond',CCond,BranchCond),
   traducir_clausulaY(ClausulaY,Res1,BranchCond),
   traducir_clausulasY(ClausulasY,Res2,BranchExp,[CIf,CCondNew|CT],COut),
   append(Res1,[['JMP',BranchExp],['BRN',BranchCond]|Res2],Res).
traducir_clausulasY([],[],_,CIn,CIn).

traducir_clausulaY(ClausulaY,Res,BranchCond):-
   traducir_clausulasNo(ClausulaY,Res,BranchCond).

traducir_clausulasNo([ClausulaNo|ClausulasNo],Res,BranchCond):-
   traducir_clausulaNo(ClausulaNo,Res1,BranchCond),
   traducir_clausulasNo(ClausulasNo,Res2,BranchCond),
   append(Res1,Res2,Res).
traducir_clausulasNo([],[],_).

traducir_clausulaNo(no(ClausulaAtomica),[Res,['BEQ',BranchCond]],BranchCond):-
   traducir_clausulaAtomica(ClausulaAtomica,Res).
traducir_clausulaNo(ClausulaAtomica,[Res,['BNQ',BranchCond]],BranchCond):-
   traducir_clausulaAtomica(ClausulaAtomica,Res).

traducir_clausulaAtomica('fl',['CMP',0,1]).
traducir_clausulaAtomica('fb',['CMP',0,0]).
traducir_clausulaAtomica('dl',['CMP',1,1]).
traducir_clausulaAtomica('db',['CMP',1,0]).
traducir_clausulaAtomica('il',['CMP',2,1]).
traducir_clausulaAtomica('ib',['CMP',2,0]).
traducir_clausulaAtomica('on',['CMP',3,1]).
traducir_clausulaAtomica('os',['CMP',3,4]).
traducir_clausulaAtomica('oe',['CMP',3,2]).
traducir_clausulaAtomica('oo',['CMP',3,8]).
traducir_clausulaAtomica(consul(pc,Color),['CMP',4,Color]).

/* Ultima fase del compilador
   Esta parte traduce el codigo "ensamblador" en lenguaje
   maquina
*/

compilar(Cadena,Res,Tam,Errores) :-
   generarEnsamblador(Cadena,CodigoEn,Errores1),
   (Errores1 = [] ->
      obtenerPosDeBranchs(CodigoEn,0,CodigoBranchFree,ListPos),
      ensamblar(CodigoBranchFree,ListPos,Res,Tam),
      Errores = []
   ; Errores = Errores1
   ).


obtenerPosDeBranchs([['BRN',Nombre]|T],Cont,Res,[[Nombre,Cont]|ListPos]):-
   obtenerPosDeBranchs(T,Cont,Res,ListPos).

obtenerPosDeBranchs([H|T],Cont,[H|Res],ListPos):-
   Cont2 is Cont+1,
   obtenerPosDeBranchs(T,Cont2,Res,ListPos).

obtenerPosDeBranchs([],_,[],[]).

obtenerPosDeBranch(Nombre,[[Nombre,Pos]|_],Pos).
obtenerPosDeBranch(Nombre,[_|T],Res) :-
   obtenerPosDeBranch(Nombre,T,Res).
obtenerPosDeBranch(_,[],0).

ensamblar([Ins|T],ListPos,[InsEnsa|Res],NTam):-
   ensamblarIns(Ins,InsEnsa,ListPos),
   ensamblar(T,ListPos,Res,Tam),
   NTam is Tam +1.
ensamblar([],_,[],0).

ensamblarIns(['AVA'],134217728,_).
ensamblarIns(['GIR'],268435456,_).
ensamblarIns(['PIN',Color],Code,_):-
   Code is 402653184 + Color.
ensamblarIns(['BNQ',Branch],Code,ListPos):-
   obtenerPosDeBranch(Branch,ListPos,PosBranch),
   Code is 536870912 + PosBranch.
ensamblarIns(['APA'],671088640,_).
ensamblarIns(['CMP',N1,N2],Code,_):-
   Code is 805306368 + (N1<<24) + N2.
ensamblarIns(['JMP',Branch],Code,ListPos):-
   obtenerPosDeBranch(Branch,ListPos,PosBranch),
   Code is 939524096 + PosBranch.
ensamblarIns(['BEQ',Branch],Code,ListPos):-
   obtenerPosDeBranch(Branch,ListPos,PosBranch),
   Code is 1073741824 + PosBranch.
ensamblarIns(['AST'],1207959552,_).
ensamblarIns(['PST'],1342177280,_).
ensamblarIns(['CVA',Valor],Code,_):-
   Code is 1476395008 + Valor.
ensamblarIns(['RVA'],1610612736,_).
