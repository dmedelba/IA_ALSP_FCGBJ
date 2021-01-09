#ifndef Modelo_hpp
#define Modelo_hpp

#include <stdio.h>
#include <queue>
#include <string>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

struct Avion{
public:
    int Ei; // tiempo más temprano de aterrizaje del avión i
    int Ti; // tiempo objetivo de aterrizaje del avión i.
    int Li; // tiempo más tardío de aterrizaje del avión i.
    double gi; // penalización si aterriza antes que Ti.
    double hi; // penalización si aterriza después que Ti.
    vector<int> dominio; // dominio de posibles tiempos para el avión i.

    //indice? para saber que avion es, este lo indica la posición dentro del array
};
struct ContadorInstancias{
public:
    int variablesInstanciadas;
    int numeroChequeos;
    int numeroRetornos;
    string nombreInstancia;
};

struct Solucion{
public:
    vector<int> vectorTiempos; //se almacena la solucion actual
    int costoSolucionActual;  //costo de la solucion almacenada
    clock_t tiempoInicio; // tiempos para calcular lo que se demora
    clock_t tiempoFinal; 
    double tiempoTotal; // final-inicial
};

class Modelo {
private: 
    int p; // numero de aviones
    void split(string Linea, char Separador, vector<string> &TempBuff, int &TotalVector);
    void Cuenta(string s, const char Separadorr, int &TotalChars);
    void imprimirMatrizSeparacion();

public:
    Modelo(queue<string> lineas, string nombre_instancia); // cola donde almaceno el archivo.
    void solve(); // Resolucion del problema.
    int** matrizSeparacion; //matriz de separacion entre pares de aviones.
    vector<Avion> Aviones; //lista que tendrá todos los aviones.
    vector<int> mejorSolucion; // almaceno la mejor solucion encontrada
    int evaluarSolucion(vector<int> Solucion); //funcion que evalua la solución y retorna su coste.
    void imprimirVectorSolucion(vector<int> Solucion); //funcion para imprimir la solución encontrada.
    vector<Avion> filtrarDominios(vector<Avion> Aviones, int instanciaActual, int valorInstancia); //se filtra el dominio de las otras instancias
    bool dominioVacio(vector<Avion> Aviones, int instanciaActual); //se verifica si existe algun dominio posterior vacio
    int ForwardChecking(vector<Avion> Aviones, int instanciaActual, vector<int> solucion, int dominioActual); //FC+GBJ
    void my_handler(int s); //funcion que se ejecuta cuando ocurre control + c 
  

};



#endif