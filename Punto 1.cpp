#include <iostream>
#include <cmath>
using namespace std;

class Vector3D {
private:
    double x, y, z; // Coordenadas del vector

public:
    // Constructor que inicializa las coordenadas
    Vector3D(double x_=0, double y_=0, double z_=0)
        : x(x_), y(y_), z(z_) {}

    // Método para calcular la magnitud del vector
    double magnitud() const {
        return sqrt(x*x + y*y + z*z);
    }

    // Método para normalizar el vector
    void normalizar() {
        double mag = magnitud();
        if (mag != 0) {
            x /= mag;
            y /= mag;
            z /= mag;
        } else {
            cout << "No se puede normalizar un vector nulo." << endl;
        }
    }

    // Sobrecarga del operador suma (+)
    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    // Sobrecarga del operador resta (-)
    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    // Sobrecarga del operador * para producto escalar
    double operator*(const Vector3D& v) const {
        return x*v.x + y*v.y + z*v.z;
    }

    // Método para imprimir las coordenadas
    void imprimir() const {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
