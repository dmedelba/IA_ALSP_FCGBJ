#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include "Modelo.hpp"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

void my_handler(int s){
    extern struct ContadorInstancias contadorInstancias;
    extern struct Solucion solucionActual;
    clock_t tiempoFinal = clock();
    double tiempo = (double)(tiempoFinal - solucionActual.tiempoInicio) / CLOCKS_PER_SEC;
    //Se imprime la cantidad de instancias
    cout << "|--------------------Solución y datos hasta el momento --------------------------|" <<endl;
    cout << "   Cantidad instanciaciones: " << contadorInstancias.variablesInstanciadas << endl;
    cout << "   Cantidad de chequeos (vista forward al dominio): " << contadorInstancias.numeroChequeos << endl;
    cout << "   Cantidad de retornos totales: " << contadorInstancias.numeroRetornos << endl;
    cout << "   Tiempo de ejecución total: " << tiempo << endl;
    cout << "|-------------------------MEJOR SOLUCIÓN ENCONTRADA------------------------------|" <<endl;
    //imprimir solucion 
    for (int i = 0; i < solucionActual.vectorTiempos.size(); i++){
        cout << "    Avion " << i+1 << " :" << solucionActual.vectorTiempos[i] <<endl;
    }
    cout << "|--------------------------------------------------------------------------------|" <<endl;
    cout << "   Costo: " << solucionActual.costoSolucionActual << endl;
    cout << "   Tiempo: " << solucionActual.tiempoTotal << endl;
    cout << "|--------------------------------------------------------------------------------|" <<endl;
    //se guarda en el archivo
    ofstream output;
    string nombreArchivo = "solucion_"+ contadorInstancias.nombreInstancia + ".txt";
    output.open(nombreArchivo);
    output << "|--------------------Solución y datos hasta el momento --------------------------|" <<"\n";
    output << "    Cantidad instanciaciones: " << contadorInstancias.variablesInstanciadas << "\n";
    output << "    Cantidad de chequeos (vista forward al dominio): " << contadorInstancias.numeroChequeos << "\n";
    output << "    Cantidad de retornos totales: " << contadorInstancias.numeroRetornos << "\n";
    output << "    Tiempo de ejecución total: " << tiempo<< "\n";
    output << "|-------------------------MEJOR SOLUCIÓN ENCONTRADA------------------------------|" <<endl;
    //imprimir solucion 
    for (int i = 0; i < solucionActual.vectorTiempos.size(); i++){
        output << "    Avion " << i+1 << " :" << solucionActual.vectorTiempos[i] <<"\n";
    }
    output << "|--------------------------------------------------------------------------------|" <<"\n";
    output << "    Costo: " << solucionActual.costoSolucionActual << "\n";
    output << "    Tiempo: " << solucionActual.tiempoTotal <<"\n"; 
    output << "|--------------------------------------------------------------------------------|" <<"\n";
    output.close();

    exit(1);
}

int main(int argc, const char * argv[]) {

    string nombre_instancia;
    string ruta;

    cout << "Ingrese el nombre de la instancia SIN su extensión: ";
    cin >> nombre_instancia;
    //creamos la ruta correspondiente

    ruta = "Instancias/" + nombre_instancia + ".txt";
    cout << "Leyendo la instancia:" << ruta <<endl;
    //comprobamos que la instancia exista
    ifstream infile(ruta);

    while (true) {
        if (infile) {
            cout << "Nombre de instancia correcta!" << endl;
            break;
        } else {
            cout << "El archivo ingresado NO existe." << endl;
        }
        cout << "Ingrese el nombre de la instancia con su extensión:";
        cin >> nombre_instancia;
        ruta = "Instancias/" + nombre_instancia;

        infile.open(ruta);
    }

    //recorremos la instancia y se guarda cada fila en una cola para luego obtener los valores requeridos.
    string linea;
    queue<string> instancia = *new queue<string>;
    while (getline(infile,linea)) {
        instancia.push(linea);
    }

    //una vez leida la instancia se crea el modelo a partir de la cola creada anteriormente.
    Modelo m = *new Modelo(instancia,nombre_instancia);

    //control + c
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    
    m.solve();
    return 0;
}