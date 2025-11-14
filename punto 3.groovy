#include <iostream>
#include <cmath>
using namespace std;

class Polinomio {
private:
    int grado;          // Grado del polinomio
    double* coef;       // Coeficientes del polinomio (coef[i] corresponde a x^i)

public:
    // Constructor que inicializa los coeficientes
    Polinomio(int n, const double* coeficientes) : grado(n) {
        coef = new double[grado + 1];
        for (int i = 0; i <= grado; ++i)
            coef[i] = coeficientes[i];
    }

    // Constructor de copia
    Polinomio(const Polinomio& otro) : grado(otro.grado) {
        coef = new double[grado + 1];
        for (int i = 0; i <= grado; ++i)
            coef[i] = otro.coef[i];
    }

    // Método para evaluar el polinomio en un valor dado de x
    double evaluar(double x) const {
        double resultado = 0.0;
        for (int i = grado; i >= 0; --i)
            resultado = resultado * x + coef[i];  // Regla de Horner
        return resultado;
    }

    // Sobrecarga del operador suma
    Polinomio operator+(const Polinomio& p) const {
        int maxGrado = (grado > p.grado) ? grado : p.grado;
        double* nuevoCoef = new double[maxGrado + 1];

        for (int i = 0; i <= maxGrado; ++i) {
            double a = (i <= grado) ? coef[i] : 0.0;
            double b = (i <= p.grado) ? p.coef[i] : 0.0;
            nuevoCoef[i] = a + b;
        }

        Polinomio resultado(maxGrado, nuevoCoef);
        delete[] nuevoCoef;
        return resultado;
    }

    // Sobrecarga del operador multiplicación
    Polinomio operator*(const Polinomio& p) const {
        int nuevoGrado = grado + p.grado;
        double* nuevoCoef = new double[nuevoGrado + 1];

        for (int i = 0; i <= nuevoGrado; ++i)
            nuevoCoef[i] = 0.0;

        for (int i = 0; i <= grado; ++i)
            for (int j = 0; j <= p.grado; ++j)
                nuevoCoef[i + j] += coef[i] * p.coef[j];

        Polinomio resultado(nuevoGrado, nuevoCoef);
        delete[] nuevoCoef;
        return resultado;
    }

    // Método para derivar el polinomio
    Polinomio derivar() const {
        if (grado == 0) {
            double c[1] = {0.0};
            return Polinomio(0, c);
        }

        double* nuevoCoef = new double[grado];
        for (int i = 1; i <= grado; ++i)
            nuevoCoef[i - 1] = i * coef[i];

        Polinomio derivada(grado - 1, nuevoCoef);
        delete[] nuevoCoef;
        return derivada;
    }

    // Método para imprimir el polinomio en forma estándar
    void imprimir() const {
        for (int i = grado; i >= 0; --i) {
            cout << coef[i];
            if (i > 0)
                cout << "x^" << i << " ";
            if (i > 0 && coef[i - 1] >= 0)
                cout << "+ ";
        }
        cout << endl;
    }

    // Destructor
    ~Polinomio() {
        delete[] coef;
    }
};
