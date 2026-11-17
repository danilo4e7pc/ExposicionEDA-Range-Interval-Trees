#ifndef COMUN_H
#define COMUN_H

#include <iostream>
#include <vector>
using namespace std;

// Punto en el plano 2D
struct Punto{
    int x;
    int y;
    Punto(){}
    Punto(int px, int py){
        x = px;
        y = py;
    }
};

// Intervalo cerrado [inicio, fin]
struct Intervalo{
    int inicio;
    int fin;
    Intervalo(){}
    Intervalo(int i, int f){
        inicio = i;
        fin = f;
    }
};

// Contadores para mostrar cuanto trabajo hizo cada consulta
struct Metricas{
    long nodosVisitados;
    long comparaciones;
    Metricas(){
        nodosVisitados = 0;
        comparaciones = 0;
    }
    void reiniciar(){
        nodosVisitados = 0;
        comparaciones = 0;
    }
};

// Busqueda binaria: devuelve la primera posicion i tal que v[i].y >= valor
// (si todos son menores devuelve v.size()). Cuenta las comparaciones hechas.
inline int primeraPosicionY(const vector<Punto>& v, int valor, Metricas& m){
    int izq = 0;
    int der = (int)v.size();
    while(izq < der){
        int medio = (izq + der) / 2;
        m.comparaciones++;
        if(v[medio].y < valor) izq = medio + 1;
        else der = medio;
    }
    return izq;
}

#endif
