#include "Modelo.hpp"
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

Solucion solucionActual; //struct para almacenar la solucion actual
ContadorInstancias contadorInstancias; //para almacenar datos
//función utilizada para contar los caracteres en la función split.
void Modelo::Cuenta(string s, const char Separadorr, int &TotalChars) {
    for (int i = 0; i < s.size(); i++)
        if (s[i] == Separadorr) TotalChars++;
}
//funcion split que recibe un string, un separador y devuelve un vector aplicando split.
void Modelo::split(string Linea, char Separador, vector<string> &TempBuff, int &TotalVector) {
    TempBuff.resize(0);
    TotalVector = 0;
    int Nums = -1;
    int NumPos = -1;
    int ValorNum = 0;
    int TotalChars = 0;
    int TotalEspacios = Linea.length();
    string Valor;
    Cuenta(Linea, Separador, TotalChars);
    if (TotalChars != 0) {
        while (Nums < TotalChars) {
            Nums++;
            ValorNum = Linea.find(Separador, NumPos + 1);
            Valor = Linea.substr(NumPos + 1,ValorNum);
            Valor = Valor.substr(0, Valor.find_first_of(Separador));
            TempBuff.push_back(Valor);
            NumPos = ValorNum;
            TotalEspacios++;
        }
        TotalVector = TempBuff.size();
    }
    else {
        //TempBuff.push_back(Linea.substr(0, Linea.find_first_of(Separador)));
        TotalVector = 0;
    }
}

//imprimimos la matriz de separación para observar la correcta creación de esta representación.
void Modelo::imprimirMatrizSeparacion(){
    cout << "Matriz de separaciones de la instancia:" << endl << endl;
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < p; j++) {
            cout << matrizSeparacion[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}


//se crea el modelo al momento de leer lo que trae la instancia (arreglo de aviones y matriz de separaciones) para su solución.
Modelo::Modelo(queue<string> lineas, string nombre_instancia) {
    contadorInstancias.nombreInstancia = nombre_instancia;
    p = stoi(lineas.front());
    lineas.pop();
    Aviones = *new vector<Avion>;
    
    //leemos el resto de las lineas maxLectura indica hasta donde tiene que leer cantidadaviones*2
    int maxLectura = 0; 
    int indiceAvion = 0;
    int datosoSeparacion = 0; // si = 0 , son los datos del avión. si= 1, son los tiempos de separación del avión.
    matrizSeparacion = new int*[p]; //matriz para agregar los tiempo de separación
    while (maxLectura < p*2){
        string lineaActual = lineas.front();
        lineas.pop(); //saco la linea que ya leí.
        vector <string> listaLinea(0);
        int totalVector;
        split(lineaActual, *" ",listaLinea,totalVector);
        if (datosoSeparacion == 0){
            //datos del avión, creo el avion y lo agregó a la lista de aviones.  
            Avion nuevoAvion;
            nuevoAvion.Ei = stoi(listaLinea[1]);
            nuevoAvion.Ti = stoi(listaLinea[2]);
            nuevoAvion.Li = stoi(listaLinea[3]);
            nuevoAvion.gi = stod(listaLinea[4]);
            nuevoAvion.hi = stod(listaLinea[5]);

            vector<int> nuevoDominio;
            //Creamos su dominio que irá de Ei a Li
            for (int i=nuevoAvion.Ei; i<= nuevoAvion.Li; i++){
                nuevoDominio.push_back(i);
            }
            nuevoAvion.dominio = nuevoDominio;

            //agrego el avión al arreglo de aviones
            Aviones.push_back(nuevoAvion);
            datosoSeparacion = 1;

        }
        else{
            //tiempos de separación agrego a matriz.
            matrizSeparacion[indiceAvion] = new int[p];
            //cada linea la agrego a la matriz
            for (int i = 1; i<listaLinea.size(); i++){
                matrizSeparacion[indiceAvion][i-1] = stoi(listaLinea[i]);
            };
            //aumento el indice de avión ya que cambio de avión
            datosoSeparacion = 0;
            indiceAvion++;
        }
        maxLectura++;
    }
    //imprimo matriz para verificar 
    //imprimirMatrizSeparacion();
    //instanciamos el primer avión con su primer valor del dominio.

}

//función encargada de filtrar el dominio de las siguientes instancias, según la separación temporal. 
vector<Avion> Modelo::filtrarDominios(vector<Avion> Aviones, int instanciaActual, int valorInstancia){
    //valor de la variable actual
    //crear un nuevo vector de aviones para devolver
    for (int i = instanciaActual+1; i < Aviones.size(); i++){  //aviones no instanciados
        //filtro los dominios
        for(int di= 0; di < Aviones[i].dominio.size(); di ++){
            contadorInstancias.numeroChequeos +=1; //por cada varible verificada de los dominios se suma la cantidad de chequeo.
            if(abs(valorInstancia - Aviones[i].dominio[di])< matrizSeparacion[instanciaActual][i]){
                //cout<< "Elimino el dominio" << Aviones[i].dominio[di] << endl;
                Aviones[i].dominio.erase(Aviones[i].dominio.begin() + di);
                di--;
            }
        }
        
    }
    return Aviones;   
}

//función que verifica si el dominio de la actual instancia es vacio o no, para poder hacer el salto correspondiente.
bool Modelo::dominioVacio(vector<Avion> Aviones, int instanciaActual){
    //recorre los dominio y retorna True si existe uno vacio, post filtrado.
    for (int i = instanciaActual; i < Aviones.size(); i++){
        if (Aviones[i].dominio.empty()){
            return true;
        }
    }
    return false;

}

int Modelo::evaluarSolucion(vector<int> Solucion){
    //cada indice representa el avión correspondiente en el arreglo Aviones
    int resultado = 0;
    for (int i = 0; i < Solucion.size(); i++){
        //a1 *gi + bi*h1
        int a1 = max(0, Aviones[i].Ti-Solucion[i]);
        int b1 = max(0, Solucion[i]-Aviones[i].Ti);
        resultado += a1*Aviones[i].gi + b1*Aviones[i].hi;
    }
    return resultado;    
}

void Modelo::imprimirVectorSolucion(vector<int> Solucion){
    cout << "Tiempos asignados: "<< endl;
        for (int i=0; i<Solucion.size();i++){
            cout <<"Avion " << i+1 << ": "<< Solucion[i] << endl;
        }
}
//forward checking, recibe el vector de aviones, la instancia acutal y la posición del dominio.
//return 1: encuentra solución ; return 0: se devuelve (GBJ)
int Modelo::ForwardChecking(vector<Avion> Aviones, int instanciaActual, vector<int> solucion, int dominioActual){
    contadorInstancias.variablesInstanciadas += 1; // se aumenta la cantidad de instanciaciones.
    int valorInstancia = solucion[instanciaActual];

    //ultima instancia
    if (instanciaActual == p-1){
        
        //evaluamos la solución para ver si la almacenamos como mejor solución o no.
        int costoSolucion = evaluarSolucion(solucion);

        if(costoSolucion < solucionActual.costoSolucionActual){
            //encontramos una solución mejor y almacenamos el tiempo que demoro en encontrarla.
            solucionActual.tiempoFinal = clock();
            double tiempo = (double)(solucionActual.tiempoFinal - solucionActual.tiempoInicio) / CLOCKS_PER_SEC;
            solucionActual.tiempoTotal = tiempo;
  
            solucionActual.costoSolucionActual = costoSolucion; //actualizamos costo
            solucionActual.vectorTiempos = solucion; //actualizamos vector solucion.
            cout << "Costo: " << solucionActual.costoSolucionActual <<" Tiempo: "<< solucionActual.tiempoTotal <<endl;
            //imprimirVectorSolucion(solucionActual.vectorTiempos);
        }

        
        return 1; //retorna 1 cuando encuentra solución
    }
    //FILTRAR DOMINIO
    vector<Avion> avionesFiltrados = filtrarDominios(Aviones, instanciaActual, valorInstancia);
    //me devuelve un arreglo de aviones con sus dominios filtrados.
    //si no quedan variables por intanciar tengo que volver a la union de la última instancia instanciada: GBJ.
   
    //si existe algun dominio vacio no instancia, se devuelve.
    if(dominioVacio(avionesFiltrados, instanciaActual)){
        //devolver a la ultima instancia GBJ.
        //cout << "| DOMINIO VACIO| DOMINIO VACIO | DOMINIO VACIO| DOMINIO VACIO |"<<endl;
        contadorInstancias.numeroRetornos += 1; //retorno aumentamos la cantidad de retornos
        return 0; //señala que hay un back, a la última variable instanciada posible de asignar un valor.
    }
    else{
        //se recorre las ramas de forma recursiva, esto permite volver a la ultima variable instanciada.
        for(int i=0; i<avionesFiltrados[instanciaActual+1].dominio.size();i++){
            solucion[instanciaActual+1] = avionesFiltrados[instanciaActual+1].dominio[i]; //el primer valor disponible del dominio
            int response = ForwardChecking(avionesFiltrados, instanciaActual+1, solucion, i);
            if(response ==0){
                //otro retorno GBJ;
                contadorInstancias.numeroRetornos += 1;
                return 0;
            }
        }
        //hay soluciion pero se devuelve, aumentamos numero de retorno
        contadorInstancias.numeroRetornos += 1;
        return 1;
    }

}

//funcion que resuelve el modelo llamando a FC +GBJ.
void Modelo::solve() {
    vector <int>solucion(p); //vector para ir almacenando la instanciación de las variables.
    solucion[0] = Aviones[0].Ei;


    //variables no instanciadas  = -1.
    for (int i = 1; i < p; i++){
        solucion[i] = -1;
    }
    
    //creamos la primera solucion, la mejor hasta el momento
    solucionActual.vectorTiempos = solucion; 
    solucionActual.costoSolucionActual = 999999; //numero grande para comenzar a minimizar
    //se instancia la primera variable
    int primerValor = Aviones[0].dominio[0];
    //todos los valores para x1 = dominio_0, sea una busqueda completa
    solucionActual.tiempoInicio = clock();  //comienza tiempo
    //busqueda completa debo probar con todos los posibles valores del primer avión a instanciar.
    vector<Avion> AvionesCopia(Aviones);

    contadorInstancias.variablesInstanciadas = 0; //comenzamos cuenta de instanciaciones
    contadorInstancias.numeroChequeos = 0; //numero de chequeos.
    contadorInstancias.numeroRetornos = 0; //numero de retornos totales

    for (int i = 0; i < Aviones[0].dominio.size(); i++){
        solucion[0] = Aviones[0].dominio[i];
        ForwardChecking(AvionesCopia, 0, solucion,i);

    }
    // al salir del for, significa que encontró la mejor solución, la imprimimos:
    cout <<endl;
    cout << "|-------- Búsqueda finalizada --------|" <<endl;
    imprimirVectorSolucion(solucionActual.vectorTiempos);
    cout << "-";
    cout << "Costo: " << solucionActual.costoSolucionActual <<" | Tiempo: "<< solucionActual.tiempoTotal <<endl;
    cout << "|-------------------------------------|" <<endl;
    cout <<"Cantidad instanciaciones: " << contadorInstancias.variablesInstanciadas << endl;
    cout << "Cantidad de chequeos (vista forward al dominio): " << contadorInstancias.numeroChequeos << endl;
    cout << "Cantidad de retornos totales: " << contadorInstancias.numeroRetornos << endl;

   
}