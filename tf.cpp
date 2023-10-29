#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

//Representa el valor más grande que puede ser almacenado en una variable de tipo int
const int INF = numeric_limits<int>::max();

// Estructura para representar el grafo ponderado
struct Grafo {
    int numNodos;
    int** pesos;

    Grafo(int n) : numNodos(n) {
        pesos = new int*[n];
        for (int i = 0; i < n; ++i) {
            pesos[i] = new int[n];
        }
    }

    ~Grafo() {
        for (int i = 0; i < numNodos; ++i) {
            delete[] pesos[i];
        }
        delete[] pesos;
    }
};

// Función para verificar si un nodo se ha visitado
bool fueVisitado(int nodo, const int* ruta, int longitud) {
    for (int i = 0; i < longitud; ++i) {
        if (ruta[i] == nodo) {
            return true;
        }
    }
    return false;
}

// Función para calcular la distancia de la ruta
int calcularDistanciaParcial(const int* ruta, int longitud, const Grafo& grafo) {
    int costo = 0;
    for (int i = 0; i < longitud - 1; ++i) {
        costo += grafo.pesos[ruta[i]][ruta[i + 1]];
    }
    return costo;
}

// Función para imprimir la matriz con etiquetas
void imprimirMatrizConEtiquetas(const Grafo& grafo) {
    cout << "  ";
    for (int i = 0; i < grafo.numNodos; ++i) {
        cout << "  " << char(97 + i);
    }
    cout << endl;

    for (int i = 0; i < grafo.numNodos; ++i) {
        cout << char(97 + i) << "   ";
        for (int j = 0; j < grafo.numNodos; ++j) {
            cout << grafo.pesos[i][j] << "  ";
        }
        cout << endl;
    }
}

// Función para realizar la búsqueda exhaustiva (Branch and Bound)
void busquedaExhaustivaTSP(int nodoActual, int* ruta, int longitudRuta, int& costoMinimo, int* rutaMinima, const Grafo& grafo) {
    if (longitudRuta == grafo.numNodos) {
        // Si se ha visitado todos los nodos, verifica si el costo es menor que el mínimo actual
        int costoActual = calcularDistanciaParcial(ruta, longitudRuta, grafo) + grafo.pesos[nodoActual][ruta[0]];
        if (costoActual < costoMinimo) {
            costoMinimo = costoActual;
            for (int i = 0; i < longitudRuta; ++i) {
                rutaMinima[i] = ruta[i];
            }
        }
        return;
    }

    for (int nodoSiguiente = 0; nodoSiguiente < grafo.numNodos; ++nodoSiguiente) {
        if (!fueVisitado(nodoSiguiente, ruta, longitudRuta)) {
            ruta[longitudRuta] = nodoSiguiente;
            busquedaExhaustivaTSP(nodoSiguiente, ruta, longitudRuta + 1, costoMinimo, rutaMinima, grafo);
        }
    }
}
void solicitarDatos(int& N) {
    do {
        cout << "Ingrese el valor de N (entre 5 y 15): ";
        cin >> N;
    } while (N < 5 || N > 15);
}

void elegirFormaDeEntrada(Grafo& grafo, int N) {
    char opc;
    cout << "¿Cómo desea ingresar los pesos de las aristas?" << endl;
    cout << "A. Forma automática (valores aleatorios entre 0 y 9)" << endl;
    cout << "B. Forma manual (ingresar valores uno por uno)" << endl;
    cout << "Elija una opción (A/B): ";
    cin >> opc;
    
    opc = toupper(opc);

    if (opc == 'A') {
        // Forma automática
        for (int i = 0; i < grafo.numNodos; ++i) {
            for (int j = i + 1; j < grafo.numNodos; ++j) {
                int valorAleatorio = rand() % 10; // Valor aleatorio entre 0 y 9
                grafo.pesos[i][j] = valorAleatorio;
                grafo.pesos[j][i] = valorAleatorio; // Establecer el mismo valor en la posición [j][i]
            }
        }
    } else if (opc == 'B') {
        // Forma manual
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) { 
                cin >> grafo.pesos[i][j]; 
            }
        }
    } else {
        cout << "Opción no válida! >:v" << endl;
    }
}

int main() {
    int N; //Número de nodos (a, b, c, d...)
    solicitarDatos(N);

    Grafo grafo(N);

    elegirFormaDeEntrada(grafo, N);

    cout << "Matriz de pesos:\n";
    imprimirMatrizConEtiquetas(grafo);

    int* ruta = new int[N];
    ruta[0] = 0; // Comienza desde el nodo 0
    int costoMinimo = INF;
    int* rutaMinima = new int[N];

    busquedaExhaustivaTSP(0, ruta, 1, costoMinimo, rutaMinima, grafo);

    cout << "Ciclo Hamiltoniano de peso mínimo:\n";
    for (int i = 0; i < N; ++i) {
        cout << char(97 + rutaMinima[i]) << " "; // El ciclo hamiltoniano siempre empieza por el nodo "a"
    }
    cout << "a\n"; // Regresa al nodo "a" para completar el ciclo"
    cout << "Peso mínimo: " << costoMinimo << endl;

    delete[] ruta;
    delete[] rutaMinima;

return 0;
}