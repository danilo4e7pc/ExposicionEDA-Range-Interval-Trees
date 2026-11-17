#include "layered_range_tree.h"

LayeredRangeTree::LayeredRangeTree(vector<Punto>& puntosOrdenadosPorX){
    raiz = nullptr;
    soloContar = false;
    contados = 0;
    if(!puntosOrdenadosPorX.empty()){
        raiz = construir(puntosOrdenadosPorX, 0, (int)puntosOrdenadosPorX.size() - 1);
    }
}

LayeredRangeTree::~LayeredRangeTree(){
    liberar(raiz);
}

void LayeredRangeTree::liberar(NodoLayered* nodo){
    if(nodo == nullptr) return;
    liberar(nodo->izq);
    liberar(nodo->der);
    delete nodo;
}

NodoLayered* LayeredRangeTree::construir(vector<Punto>& p, int ini, int fin){
    NodoLayered* nodo = new NodoLayered();
    nodo->xmin = p[ini].x;
    nodo->xmax = p[fin].x;

    // Caso base: hoja
    if(ini == fin){
        nodo->porY.push_back(p[ini]);
        return nodo;
    }

    int medio = (ini + fin) / 2;
    nodo->izq = construir(p, ini, medio);
    nodo->der = construir(p, medio + 1, fin);

    // Merge de los hijos. Antes de colocar cada elemento anotamos
    // cuantos elementos de cada hijo ya fueron colocados.
    vector<Punto>& A = nodo->izq->porY;
    vector<Punto>& B = nodo->der->porY;
    int i = 0;
    int j = 0;
    while(i < (int)A.size() || j < (int)B.size()){
        nodo->haciaIzq.push_back(i);
        nodo->haciaDer.push_back(j);

        bool tomarIzq;
        if(i >= (int)A.size()) tomarIzq = false;
        else if(j >= (int)B.size()) tomarIzq = true;
        else tomarIzq = (A[i].y <= B[j].y);

        if(tomarIzq){
            nodo->porY.push_back(A[i]);
            i++;
        } else {
            nodo->porY.push_back(B[j]);
            j++;
        }
    }
    // Posicion extra al final (para cuando "hasta" == tamanio)
    nodo->haciaIzq.push_back(i);
    nodo->haciaDer.push_back(j);
    return nodo;
}

void LayeredRangeTree::consultar(int x1, int x2, int y1, int y2, vector<Punto>& resultado){
    resultado.clear();
    if(raiz == nullptr) return;

    // UNICA busqueda binaria de toda la consulta (en la raiz) -> O(log n)
    int desde = primeraPosicionY(raiz->porY, y1, metricas);
    int hasta = primeraPosicionY(raiz->porY, y2 + 1, metricas);

    consultarNodo(raiz, x1, x2, desde, hasta, resultado);
}

void LayeredRangeTree::consultarNodo(NodoLayered* nodo, int x1, int x2, int desde, int hasta, vector<Punto>& resultado){
    if(nodo == nullptr) return;
    metricas.nodosVisitados++;

    // Ningun punto de este nodo cae en el rango y: podamos
    metricas.comparaciones++;
    if(desde >= hasta) return;

    // Totalmente fuera en x
    metricas.comparaciones += 2;
    if(nodo->xmax < x1 || nodo->xmin > x2) return;

    // Totalmente dentro en x: ya conocemos las posiciones, solo recorremos
    metricas.comparaciones += 2;
    if(x1 <= nodo->xmin && nodo->xmax <= x2){
        if(soloContar){
            contados += hasta - desde;
        } else {
            for(int i = desde; i < hasta; i++){
                resultado.push_back(nodo->porY[i]);
            }
        }
        return;
    }

    // Parcial: traducimos las posiciones a los hijos en O(1) usando los punteros
    consultarNodo(nodo->izq, x1, x2, nodo->haciaIzq[desde], nodo->haciaIzq[hasta], resultado);
    consultarNodo(nodo->der, x1, x2, nodo->haciaDer[desde], nodo->haciaDer[hasta], resultado);
}
