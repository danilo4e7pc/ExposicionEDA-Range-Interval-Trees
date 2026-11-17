#ifndef LAYERED_RANGE_TREE_H
#define LAYERED_RANGE_TREE_H

#include "comun.h"

// Igual que el Range Tree, pero con punteros extra (fractional cascading):
// haciaIzq[i] = cuantos elementos de porY[0..i-1] pertenecen al hijo izquierdo
// haciaDer[i] = cuantos pertenecen al hijo derecho
// Asi, una posicion en este nodo se traduce a la posicion en cada hijo en O(1),
// sin volver a hacer busqueda binaria.
struct NodoLayered{
    int xmin;
    int xmax;
    vector<Punto> porY;
    vector<int> haciaIzq;
    vector<int> haciaDer;
    NodoLayered* izq;
    NodoLayered* der;

    NodoLayered(){
        xmin = 0;
        xmax = 0;
        izq = nullptr;
        der = nullptr;
    }
};

struct LayeredRangeTree{
    NodoLayered* raiz;
    Metricas metricas;
    bool soloContar;   // true: no guarda los puntos, solo los cuenta (mide el costo de la busqueda)
    long contados;

    LayeredRangeTree(vector<Punto>& puntosOrdenadosPorX);
    ~LayeredRangeTree();

    NodoLayered* construir(vector<Punto>& p, int ini, int fin);
    void liberar(NodoLayered* nodo);

    void consultar(int x1, int x2, int y1, int y2, vector<Punto>& resultado);
    // desde/hasta: posiciones [desde, hasta) del rango y dentro de nodo->porY
    void consultarNodo(NodoLayered* nodo, int x1, int x2, int desde, int hasta, vector<Punto>& resultado);
};

#endif
