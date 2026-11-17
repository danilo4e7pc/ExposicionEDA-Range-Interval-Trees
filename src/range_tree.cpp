#include "range_tree.h"

RangeTree2D::RangeTree2D(vector<Punto>& puntosOrdenadosPorX){
    raiz = nullptr;
    soloContar = false;
    contados = 0;
    if(!puntosOrdenadosPorX.empty()){
        raiz = construir(puntosOrdenadosPorX, 0, (int)puntosOrdenadosPorX.size() - 1);
    }
}

RangeTree2D::~RangeTree2D(){
    liberar(raiz);
}

void RangeTree2D::liberar(NodoRango* nodo){
    if(nodo == nullptr) return;
    liberar(nodo->izq);
    liberar(nodo->der);
    delete nodo;
}

// Construye el nodo que cubre p[ini..fin]
NodoRango* RangeTree2D::construir(vector<Punto>& p, int ini, int fin){
    NodoRango* nodo = new NodoRango();
    nodo->xmin = p[ini].x;
    nodo->xmax = p[fin].x;

    // Caso base: una hoja con un solo punto
    if(ini == fin){
        nodo->porY.push_back(p[ini]);
        return nodo;
    }

    int medio = (ini + fin) / 2;
    nodo->izq = construir(p, ini, medio);
    nodo->der = construir(p, medio + 1, fin);

    // Mezcla (merge) de los hijos, que ya estan ordenados por y
    vector<Punto>& A = nodo->izq->porY;
    vector<Punto>& B = nodo->der->porY;
    int i = 0;
    int j = 0;
    while(i < (int)A.size() && j < (int)B.size()){
        if(A[i].y <= B[j].y){
            nodo->porY.push_back(A[i]);
            i++;
        } else {
            nodo->porY.push_back(B[j]);
            j++;
        }
    }
    while(i < (int)A.size()){
        nodo->porY.push_back(A[i]);
        i++;
    }
    while(j < (int)B.size()){
        nodo->porY.push_back(B[j]);
        j++;
    }
    return nodo;
}

void RangeTree2D::consultar(int x1, int x2, int y1, int y2, vector<Punto>& resultado){
    resultado.clear();
    consultarNodo(raiz, x1, x2, y1, y2, resultado);
}

void RangeTree2D::consultarNodo(NodoRango* nodo, int x1, int x2, int y1, int y2, vector<Punto>& resultado){
    if(nodo == nullptr) return;
    metricas.nodosVisitados++;

    // 1) El nodo esta totalmente fuera del rango en x: no hay nada que buscar
    metricas.comparaciones += 2;
    if(nodo->xmax < x1 || nodo->xmin > x2) return;

    // 2) El nodo esta totalmente dentro del rango en x:
    //    buscamos en su lista ordenada por y con DOS busquedas binarias -> O(log n)
    metricas.comparaciones += 2;
    if(x1 <= nodo->xmin && nodo->xmax <= x2){
        int desde = primeraPosicionY(nodo->porY, y1, metricas);
        int hasta = primeraPosicionY(nodo->porY, y2 + 1, metricas);
        if(soloContar){
            contados += hasta - desde;
        } else {
            for(int i = desde; i < hasta; i++){
                resultado.push_back(nodo->porY[i]);
            }
        }
        return;
    }

    // 3) Cubre el rango solo en parte: bajamos a los hijos
    consultarNodo(nodo->izq, x1, x2, y1, y2, resultado);
    consultarNodo(nodo->der, x1, x2, y1, y2, resultado);
}
