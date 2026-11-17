#ifndef INTERVAL_TREE_H
#define INTERVAL_TREE_H

#include "comun.h"

// Interval Tree centrado: cada nodo tiene un valor "centro".
// Los intervalos que contienen al centro se quedan en el nodo,
// los que estan totalmente a la izquierda van al hijo izq y los de la derecha al hijo der.
struct NodoIntervalo{
    int centro;
    vector<Intervalo> porInicio;  // intervalos del nodo ordenados por inicio (menor a mayor)
    vector<Intervalo> porFin;     // los mismos, ordenados por fin (mayor a menor)
    NodoIntervalo* izq;
    NodoIntervalo* der;

    NodoIntervalo(){
        centro = 0;
        izq = nullptr;
        der = nullptr;
    }
};

struct IntervalTree{
    NodoIntervalo* raiz;
    Metricas metricas;

    IntervalTree(vector<Intervalo>& intervalos);
    ~IntervalTree();

    NodoIntervalo* construir(vector<Intervalo>& intervalos);
    void liberar(NodoIntervalo* nodo);

    // Devuelve todos los intervalos que contienen al punto q
    void consultar(int q, vector<Intervalo>& resultado);

    // Busqueda lineal (para comparar tiempos)
    void consultarFuerzaBruta(vector<Intervalo>& intervalos, int q, vector<Intervalo>& resultado);
};

#endif
