#include <iostream>
#include <iomanip>   // Para formato de impresión
#include <cstdlib>   // Para rand() y srand()
#include <ctime>     // Para inicializar la semilla aleatoria
using namespace std;

class Matriz {
private:
    int n;          // Tamaño de la matriz (n x n)
    double** datos; // Puntero doble para almacenar la matriz dinámicamente

public:
    // Constructor: inicializa la matriz con valores aleatorios
    Matriz(int tam) : n(tam) {
        srand(time(nullptr)); // Inicializa la semilla aleatoria una vez
        datos = new double*[n];
        for (int i = 0; i < n; ++i) {
            datos[i] = new double[n];
            for (int j = 0; j < n; ++j) {
                datos[i][j] = rand() % 10; // Valores aleatorios entre 0 y 9
            }
        }
    }

    // Constructor de copia (para evitar problemas con memoria dinámica)
    Matriz(const Matriz& otra) : n(otra.n) {
        datos = new double*[n];
        for (int i = 0; i < n; ++i) {
            datos[i] = new double[n];
            for (int j = 0; j < n; ++j) {
                datos[i][j] = otra.datos[i][j];
            }
        }
    }

    // Método para calcular la transpuesta de la matriz
    Matriz transpuesta() const {
        Matriz T(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                T.datos[i][j] = datos[j][i];
            }
        }
        return T;
    }

    // Método para multiplicar la matriz por otra matriz del mismo tamaño
    Matriz multiplicar(const Matriz& B) const {
        Matriz resultado(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                resultado.datos[i][j] = 0;
                for (int k = 0; k < n; ++k) {
                    resultado.datos[i][j] += datos[i][k] * B.datos[k][j];
                }
            }
        }
        return resultado;
    }

    // Sobrecarga del operador () para acceso a elementos (lectura y escritura)
    double& operator()(int i, int j) {
        return datos[i][j];
    }

    // Versión const del operador de acceso
    const double& operator()(int i, int j) const {
        return datos[i][j];
    }

    // Método para imprimir la matriz
    void imprimir() const {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << setw(5) << datos[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Destructor: libera la memoria asignada dinámicamente
    ~Matriz() {
        for (int i = 0; i < n; ++i)
            delete[] datos[i];
        delete[] datos;
    }
};
