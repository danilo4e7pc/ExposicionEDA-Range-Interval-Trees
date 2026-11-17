#include "range_tree.h"
#include "layered_range_tree.h"
#include "interval_tree.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
using namespace std;

const int NUM_PUNTOS = 200000;
const int NUM_INTERVALOS = 200000;
const int REPETICIONES = 2000;   // para medir tiempos tan pequeños con precision

// Generador pseudoaleatorio sencillo con semilla fija: los datos siempre son los mismos
unsigned int semilla = 12345;
int siguienteAleatorio(int maximo){
    semilla = semilla * 1103515245u + 12345u;
    return (int)((semilla / 65536u) % (unsigned int)maximo);
}

bool menorX(const Punto& a, const Punto& b){
    return a.x < b.x;
}

vector<Punto> generarPuntos(){
    vector<Punto> puntos;
    for(int i = 0; i < NUM_PUNTOS; i++){
        puntos.push_back(Punto(siguienteAleatorio(100000), siguienteAleatorio(100000)));
    }
    sort(puntos.begin(), puntos.end(), menorX);  // los arboles piden los puntos ordenados por x
    return puntos;
}

vector<Intervalo> generarIntervalos(){
    vector<Intervalo> intervalos;
    for(int i = 0; i < NUM_INTERVALOS; i++){
        int inicio = siguienteAleatorio(100000);
        int largo = siguienteAleatorio(50) + 1;     // intervalos cortos
        intervalos.push_back(Intervalo(inicio, inicio + largo));
    }
    return intervalos;
}

// Tiempo en milisegundos desde un instante dado
double msDesde(chrono::high_resolution_clock::time_point inicio){
    chrono::duration<double, milli> d = chrono::high_resolution_clock::now() - inicio;
    return d.count();
}

// Lee un entero validando la entrada
int leerEntero(const string& mensaje){
    int valor;
    cout << mensaje;
    while(!(cin >> valor)){
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Entrada invalida. " << mensaje;
    }
    return valor;
}

void imprimirFila(const string& nombre, int k, long nodos, long comps, double usPorConsulta){
    cout << left << setw(22) << nombre
         << right << setw(12) << k
         << setw(12) << nodos
         << setw(16) << comps
         << setw(22) << fixed << setprecision(3) << usPorConsulta << endl;
}

void imprimirEncabezado(){
    cout << left << setw(22) << "Estructura"
         << right << setw(12) << "Resultados"
         << setw(12) << "Nodos"
         << setw(16) << "Comparaciones"
         << setw(22) << "Tiempo (us/consulta)" << endl;
    cout << string(84, '-') << endl;
}

// ---------------- OPCION 1: Range Tree vs Layered Range Tree ----------------
void opcionRangeTrees(RangeTree2D& rt, LayeredRangeTree& lrt){
    cout << "\n=== RANGE TREE 2D vs LAYERED RANGE TREE ===" << endl;
    cout << "Conjunto: " << NUM_PUNTOS << " puntos predefinidos con x, y en [0, 99999]" << endl;

    int x1 = leerEntero("x minimo: ");
    int x2 = leerEntero("x maximo: ");
    int y1 = leerEntero("y minimo: ");
    int y2 = leerEntero("y maximo: ");
    if(x1 > x2) swap(x1, x2);
    if(y1 > y2) swap(y1, y2);

    vector<Punto> resRT;
    vector<Punto> resLRT;

    // Una consulta para contar nodos y comparaciones
    rt.metricas.reiniciar();
    rt.consultar(x1, x2, y1, y2, resRT);
    long nodosRT = rt.metricas.nodosVisitados;
    long compsRT = rt.metricas.comparaciones;

    lrt.metricas.reiniciar();
    lrt.consultar(x1, x2, y1, y2, resLRT);
    long nodosLRT = lrt.metricas.nodosVisitados;
    long compsLRT = lrt.metricas.comparaciones;
    int kRT = (int)resRT.size();
    int kLRT = (int)resLRT.size();
    vector<Punto> ejemplos = resLRT;

    // Muchas repeticiones para medir el tiempo. Solo contamos los puntos (sin listarlos)
    // para medir el costo de la BUSQUEDA y no el de copiar los resultados.
    rt.soloContar = true;
    lrt.soloContar = true;
    chrono::high_resolution_clock::time_point t0 = chrono::high_resolution_clock::now();
    for(int i = 0; i < REPETICIONES; i++) rt.consultar(x1, x2, y1, y2, resRT);
    double usRT = msDesde(t0) * 1000.0 / REPETICIONES;

    t0 = chrono::high_resolution_clock::now();
    for(int i = 0; i < REPETICIONES; i++) lrt.consultar(x1, x2, y1, y2, resLRT);
    double usLRT = msDesde(t0) * 1000.0 / REPETICIONES;
    rt.soloContar = false;
    lrt.soloContar = false;

    cout << "\nRango: x en [" << x1 << ", " << x2 << "], y en [" << y1 << ", " << y2 << "]" << endl;
    cout << "Primeros puntos encontrados: ";
    for(int i = 0; i < (int)ejemplos.size() && i < 5; i++){
        cout << "(" << ejemplos[i].x << "," << ejemplos[i].y << ") ";
    }
    cout << endl << endl;

    imprimirEncabezado();
    imprimirFila("Range Tree 2D", kRT, nodosRT, compsRT, usRT);
    imprimirFila("Layered Range Tree", kLRT, nodosLRT, compsLRT, usLRT);

    cout << "\nComplejidad Range Tree 2D:     O(log^2 n + k)  (una busqueda binaria por cada nodo canonico)" << endl;
    cout << "Complejidad Layered Range Tree: O(log n + k)    (una sola busqueda binaria + punteros)" << endl;
    if(usLRT > 0){
        cout << "El Layered fue " << setprecision(2) << usRT / usLRT << " veces mas rapido." << endl;
    }
    if(kRT != kLRT){
        cout << "[ERROR] Los resultados no coinciden" << endl;
    }
}

// ---------------- OPCION 2: Interval Tree ----------------
void opcionIntervalTree(IntervalTree& it, vector<Intervalo>& intervalos){
    cout << "\n=== INTERVAL TREE (busqueda de un query point) ===" << endl;
    cout << "Conjunto: " << NUM_INTERVALOS << " intervalos predefinidos en [0, 100050]" << endl;
    cout << "Ejemplos: ";
    for(int i = 0; i < 5; i++){
        cout << "[" << intervalos[i].inicio << "," << intervalos[i].fin << "] ";
    }
    cout << endl;

    int q = leerEntero("Query point q: ");

    vector<Intervalo> resIT;
    vector<Intervalo> resBruta;

    it.metricas.reiniciar();
    it.consultar(q, resIT);
    long nodosIT = it.metricas.nodosVisitados;
    long compsIT = it.metricas.comparaciones;

    it.metricas.reiniciar();
    it.consultarFuerzaBruta(intervalos, q, resBruta);
    long compsBruta = it.metricas.comparaciones;

    chrono::high_resolution_clock::time_point t0 = chrono::high_resolution_clock::now();
    for(int i = 0; i < REPETICIONES; i++) it.consultar(q, resIT);
    double usIT = msDesde(t0) * 1000.0 / REPETICIONES;

    t0 = chrono::high_resolution_clock::now();
    for(int i = 0; i < REPETICIONES; i++) it.consultarFuerzaBruta(intervalos, q, resBruta);
    double usBruta = msDesde(t0) * 1000.0 / REPETICIONES;

    cout << "\nIntervalos que contienen a q = " << q << ". Primeros: ";
    for(int i = 0; i < (int)resIT.size() && i < 5; i++){
        cout << "[" << resIT[i].inicio << "," << resIT[i].fin << "] ";
    }
    cout << endl << endl;

    imprimirEncabezado();
    imprimirFila("Interval Tree", (int)resIT.size(), nodosIT, compsIT, usIT);
    imprimirFila("Busqueda lineal", (int)resBruta.size(), 0, compsBruta, usBruta);

    cout << "\nComplejidad Interval Tree:  O(log n + k)" << endl;
    cout << "Complejidad busqueda lineal: O(n)" << endl;
    if(usIT > 0){
        cout << "El Interval Tree fue " << setprecision(2) << usBruta / usIT << " veces mas rapido." << endl;
    }
    if(resIT.size() != resBruta.size()){
        cout << "[ERROR] Los resultados no coinciden" << endl;
    }
}

int main(){
    cout << "ESTRUCTURAS DE RANGOS E INTERVALOS" << endl;
    cout << "Construyendo estructuras..." << endl;

    vector<Punto> puntos = generarPuntos();
    vector<Intervalo> intervalos = generarIntervalos();

    chrono::high_resolution_clock::time_point t0 = chrono::high_resolution_clock::now();
    RangeTree2D rt(puntos);
    cout << "Range Tree 2D construido en " << fixed << setprecision(1) << msDesde(t0) << " ms" << endl;

    t0 = chrono::high_resolution_clock::now();
    LayeredRangeTree lrt(puntos);
    cout << "Layered Range Tree construido en " << msDesde(t0) << " ms" << endl;

    t0 = chrono::high_resolution_clock::now();
    IntervalTree it(intervalos);
    cout << "Interval Tree construido en " << msDesde(t0) << " ms" << endl;

    int opcion = 0;
    while(opcion != 3){
        cout << "\n========== MENU ==========" << endl;
        cout << "1. Comparar Range Tree 2D vs Layered Range Tree" << endl;
        cout << "2. Interval Tree (query point)" << endl;
        cout << "3. Salir" << endl;
        opcion = leerEntero("Opcion: ");

        if(opcion == 1) opcionRangeTrees(rt, lrt);
        else if(opcion == 2) opcionIntervalTree(it, intervalos);
        else if(opcion == 3) cout << "Hasta luego." << endl;
        else cout << "Opcion invalida." << endl;
    }
    return 0;
}
