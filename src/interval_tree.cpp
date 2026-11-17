#include "interval_tree.h"
#include <algorithm>

// Comparadores simples para ordenar
bool menorInicio(const Intervalo& a, const Intervalo& b){
    return a.inicio < b.inicio;
}

bool mayorFin(const Intervalo& a, const Intervalo& b){
    return a.fin > b.fin;
}

IntervalTree::IntervalTree(vector<Intervalo>& intervalos){
    raiz = construir(intervalos);
}

IntervalTree::~IntervalTree(){
    liberar(raiz);
}

void IntervalTree::liberar(NodoIntervalo* nodo){
    if(nodo == nullptr) return;
    liberar(nodo->izq);
    liberar(nodo->der);
    delete nodo;
}

NodoIntervalo* IntervalTree::construir(vector<Intervalo>& intervalos){
    if(intervalos.empty()) return nullptr;

    // El centro es la mediana de los puntos medios: asi el arbol queda balanceado
    vector<int> medios;
    for(int i = 0; i < (int)intervalos.size(); i++){
        medios.push_back((intervalos[i].inicio + intervalos[i].fin) / 2);
    }
    sort(medios.begin(), medios.end());

    NodoIntervalo* nodo = new NodoIntervalo();
    nodo->centro = medios[medios.size() / 2];

    // Reparto de intervalos
    vector<Intervalo> izquierda;
    vector<Intervalo> derecha;
    for(int i = 0; i < (int)intervalos.size(); i++){
        Intervalo it = intervalos[i];
        if(it.fin < nodo->centro) izquierda.push_back(it);
        else if(it.inicio > nodo->centro) derecha.push_back(it);
        else nodo->porInicio.push_back(it);  // contiene al centro
    }

    nodo->porFin = nodo->porInicio;
    sort(nodo->porInicio.begin(), nodo->porInicio.end(), menorInicio);
    sort(nodo->porFin.begin(), nodo->porFin.end(), mayorFin);

    nodo->izq = construir(izquierda);
    nodo->der = construir(derecha);
    return nodo;
}

void IntervalTree::consultar(int q, vector<Intervalo>& resultado){
    resultado.clear();
    NodoIntervalo* actual = raiz;

    // Un solo camino de la raiz hacia abajo -> O(log n) nodos
    while(actual != nullptr){
        metricas.nodosVisitados++;
        metricas.comparaciones++;

        if(q == actual->centro){
            // Todos los intervalos del nodo contienen al centro = contienen a q
            for(int i = 0; i < (int)actual->porInicio.size(); i++){
                resultado.push_back(actual->porInicio[i]);
            }
            return;
        }

        if(q < actual->centro){
            // Todos terminan despues (o en) el centro, asi que basta ver que inicien en <= q.
            // Paramos apenas uno no cumple (estan ordenados por inicio).
            int i = 0;
            while(i < (int)actual->porInicio.size()){
                metricas.comparaciones++;
                if(actual->porInicio[i].inicio > q) break;
                resultado.push_back(actual->porInicio[i]);
                i++;
            }
            actual = actual->izq;
        } else {
            // Simetrico: basta ver que terminen en >= q
            int i = 0;
            while(i < (int)actual->porFin.size()){
                metricas.comparaciones++;
                if(actual->porFin[i].fin < q) break;
                resultado.push_back(actual->porFin[i]);
                i++;
            }
            actual = actual->der;
        }
    }
}

void IntervalTree::consultarFuerzaBruta(vector<Intervalo>& intervalos, int q, vector<Intervalo>& resultado){
    resultado.clear();
    for(int i = 0; i < (int)intervalos.size(); i++){
        metricas.comparaciones++;
        if(intervalos[i].inicio <= q && q <= intervalos[i].fin){
            resultado.push_back(intervalos[i]);
        }
    }
}
