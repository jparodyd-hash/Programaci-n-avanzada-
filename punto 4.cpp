#include <iostream>
#include <cmath>
#include <initializer_list>
using namespace std;

const double PI = 3.1415;

// -----------------------------------------------------------------------------
// Clase para números complejos en forma binomial: a + ib
// -----------------------------------------------------------------------------
class polar; // Declaración adelantada

class complex {
private:
    double re; // Parte real
    double im; // Parte imaginaria

public:
    // Constructores
    complex(double r = 0, double i = 0) : re(r), im(i) {}
    complex(const polar& p); // Conversión desde polar

    // Métodos de acceso
    double real() const { return re; }
    double imag() const { return im; }

    // Operadores aritméticos
    complex operator+(const complex& c) const {
        return complex(re + c.re, im + c.im);
    }

    complex operator-(const complex& c) const {
        return complex(re - c.re, im - c.im);
    }

    complex operator*(const complex& c) const {
        return complex(re * c.re - im * c.im, re * c.im + im * c.re);
    }

    complex operator/(const complex& c) const {
        double denom = c.re * c.re + c.im * c.im;
        return complex((re * c.re + im * c.im) / denom,
                       (im * c.re - re * c.im) / denom);
    }

    // Conjugado
    complex conj() const {
        return complex(re, -im);
    }

    // Magnitud y ángulo
    double magnitud() const {
        return sqrt(re * re + im * im);
    }

    double argumento() const {
        return atan2(im, re);
    }

    // Imprimir
    void imprimir() const {
        cout << re << (im >= 0 ? " + " : " - ") << fabs(im) << "i";
    }

    // Conversión a polar
    polar toPolar() const;
};

// -----------------------------------------------------------------------------
// Clase para números complejos en forma polar: r * e^(iθ)
// -----------------------------------------------------------------------------
class polar {
private:
    double r;     // Módulo
    double theta; // Argumento (en radianes)

public:
    // Constructores
    polar(double mod = 0, double ang = 0) : r(mod), theta(ang) {}
    polar(const complex& c) {
        r = c.magnitu