#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

class CTermino {
private:
    double coeficiente;
    int exponente;

public:
    // Constructor que inicia un término (por omisión a 0)
    CTermino(double coef = 0, int exp = 0) : coeficiente(coef), exponente(exp) {}
    
    // Método para acceder al coeficiente
    double obtenerCoeficiente() const {
        return coeficiente;
    }
    
    // Método para acceder al exponente
    int obtenerExponente() const {
        return exponente;
    }
    
    // Método para modificar el coeficiente
    void setCoeficiente(double coef) {
        coeficiente = coef;
    }
    
    // Método para obtener la cadena de caracteres equivalente al término
    string toString() const {
        if (coeficiente == 0) {
            return "0";
        }
        
        stringstream ss;
        string signo = (coeficiente > 0) ? "+" : "-";
        double coefAbs = abs(coeficiente);
        
        // Caso: exponente 0 (término constante)
        if (exponente == 0) {
            ss << signo << coefAbs;
        }
        // Caso: exponente 1
        else if (exponente == 1) {
            if (coefAbs == 1) {
                ss << signo << "x";
            } else {
                ss << signo << coefAbs << "x";
            }
        }
        // Caso: exponente mayor a 1
        else {
            if (coefAbs == 1) {
                ss << signo << "x^" << exponente;
            } else {
                ss << signo << coefAbs << "x^" << exponente;
            }
        }
        
        return ss.str();
    }
};


class CPolinomio {
private:
    vector<CTermino> polinomio;

public:
    // Constructor que inicia un polinomio con cero términos
    CPolinomio() {}
    
    // Método para obtener el número de términos
    int obtenerNumeroTerminos() const {
        return polinomio.size();
    }
    
    // Método para asignar un término al polinomio
    void asignarTermino(const CTermino& termino) {
        double coef = termino.obtenerCoeficiente();
        int exp = termino.obtenerExponente();
        
        // Si el coeficiente es 0, no hacer nada
        if (coef == 0) {
            return;
        }
        
        // Buscar si ya existe un término con el mismo exponente
        for (size_t i = 0; i < polinomio.size(); i++) {
            if (polinomio[i].obtenerExponente() == exp) {
                // Sumar coeficientes
                double nuevoCoef = polinomio[i].obtenerCoeficiente() + coef;
                if (nuevoCoef == 0) {
                    // Si el resultado es 0, eliminar el término
                    polinomio.erase(polinomio.begin() + i);
                } else {
                    // Reemplazar con el nuevo coeficiente
                    polinomio[i].setCoeficiente(nuevoCoef);
                }
                return;
            }
        }
        
        // Si no existe, agregarlo y ordenar
        polinomio.push_back(termino);
        
        // Ordenar en orden ascendente del exponente
        sort(polinomio.begin(), polinomio.end(), 
             [](const CTermino& a, const CTermino& b) {
                 return a.obtenerExponente() < b.obtenerExponente();
             });
    }
    
    // Método para sumar dos polinomios
    CPolinomio sumar(const CPolinomio& otro) const {
        CPolinomio resultado;
        
        // Agregar todos los términos del primer polinomio
        for (const auto& termino : polinomio) {
            resultado.asignarTermino(termino);
        }
        
        // Agregar todos los términos del segundo polinomio
        for (const auto& termino : otro.polinomio) {
            resultado.asignarTermino(termino);
        }
        
        return resultado;
    }
    
    // Método para multiplicar dos polinomios
    CPolinomio multiplicar(const CPolinomio& otro) const {
        CPolinomio resultado;
        
        // Multiplicar cada término del primer polinomio con cada término del segundo
        for (const auto& t1 : polinomio) {
            for (const auto& t2 : otro.polinomio) {
                double nuevoCoef = t1.obtenerCoeficiente() * t2.obtenerCoeficiente();
                int nuevoExp = t1.obtenerExponente() + t2.obtenerExponente();
                resultado.asignarTermino(CTermino(nuevoCoef, nuevoExp));
            }
        }
        
        return resultado;
    }
    
    // Método para obtener la cadena de caracteres equivalente al polinomio
    string toString() const {
        if (polinomio.empty()) {
            return "0";
        }
        
        string cadena = "";
        for (size_t i = 0; i < polinomio.size(); i++) {
            string terminoStr = polinomio[i].toString();
            
            if (i == 0) {
                // Primer término: si empieza con +, no mostrarlo
                if (terminoStr[0] == '+') {
                    cadena += terminoStr.substr(1);
                } else {
                    cadena += terminoStr;
                }
            } else {
                // Términos subsecuentes: agregar espacio antes del signo
                cadena += " " + terminoStr;
            }
        }
        
        return cadena;
    }
};


// Programa principal
int main() {
    cout << "=== Sistema de Manipulacion de Polinomios ===" << endl << endl;
    
    // Crear P(x) = 4x^3 + 3x^2 - 2x + 7
    CPolinomio P;
    P.asignarTermino(CTermino(4, 3));
    P.asignarTermino(CTermino(3, 2));
    P.asignarTermino(CTermino(-2, 1));
    P.asignarTermino(CTermino(7, 0));
    
    cout << "P(x) = " << P.toString() << endl;
    cout << "Numero de terminos de P(x): " << P.obtenerNumeroTerminos() << endl << endl;
    
    // Crear Q(x) = 5x^2 - 6x + 10
    CPolinomio Q;
    Q.asignarTermino(CTermino(5, 2));
    Q.asignarTermino(CTermino(-6, 1));
    Q.asignarTermino(CTermino(10, 0));
    
    cout << "Q(x) = " << Q.toString() << endl;
    cout << "Numero de terminos de Q(x): " << Q.obtenerNumeroTerminos() << endl << endl;
    
    // Sumar P(x) + Q(x)
    CPolinomio R = P.sumar(Q);
    cout << "R(x) = P(x) + Q(x) = " << R.toString() << endl;
    cout << "Numero de terminos de R(x): " << R.obtenerNumeroTerminos() << endl << endl;
    
    // Multiplicar P(x) * Q(x)
    CPolinomio M = P.multiplicar(Q);
    cout << "M(x) = P(x) * Q(x) = " << M.toString() << endl;
    cout << "Numero de terminos de M(x): " << M.obtenerNumeroTerminos() << endl << endl;
    
    // Pruebas adicionales
    cout << "=== Pruebas Adicionales ===" << endl << endl;
    
    // Polinomio simple: S(x) = 2x + 3
    CPolinomio S;
    S.asignarTermino(CTermino(2, 1));
    S.asignarTermino(CTermino(3, 0));
    cout << "S(x) = " << S.toString() << endl;
    
    // Multiplicar S por sí mismo: (2x + 3)^2
    CPolinomio S2 = S.multiplicar(S);
    cout << "S(x)^2 = " << S2.toString() << endl;
    
    return 0;
}