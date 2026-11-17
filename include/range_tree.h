#ifndef RANGE_TREE_H
#define RANGE_TREE_H

#include "comun.h"

// Cada nodo cubre un tramo de puntos ordenados por x.
// Guarda ademas TODOS los puntos de su tramo ordenados por y (arbol secundario).
struct NodoRango{
    int xmin;
    int xmax;
    vector<Punto> porY;
    NodoRango* izq;
    NodoRango* der;

    NodoRango(){
        xmin = 0;
        xmax = 0;
        izq = nullptr;
        der = nullptr;
    }
};

struct RangeTree2D{
    NodoRango* raiz;
    Metricas metricas;
    bool soloContar;   // true: no guarda los puntos, solo los cuenta (mide el costo de la busqueda)
    long contados;

    RangeTree2D(vector<Punto>& puntosOrdenadosPorX);
    ~RangeTree2D();

    NodoRango* construir(vector<Punto>& p, int ini, int fin);
    void liberar(NodoRango* nodo);

    // Busca los puntos con x1<=x<=x2 y y1<=y<=y2
    void consultar(int x1, int x2, int y1, int y2, vector<Punto>& resultado);
    void consultarNodo(NodoRango* nodo, int x1, int x2, int y1, int y2, vector<Punto>& resultado);
};

#endif
