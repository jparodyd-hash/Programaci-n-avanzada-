
#include <iostream>
#include <vector>
#include <string>
#include <deque>       // Ideal para el tablero, permite añadir por ambos extremos
#include <algorithm>   // Para std::shuffle (barajar)
#include <random>      // Para el motor de números aleatorios
#include <chrono>      // Para la semilla (seed) de tiempo
#include <sstream>     // Para construir la representación de la ficha
#include <limits>      // Para limpiar el buffer de entrada (cin)

//****************************************************************************
// 1. CLASE FICHA
// Representa una sola ficha de dominó.
//****************************************************************************
class Ficha {
private:
    int ladoA;
    int ladoB;

public:
    // Constructor
    Ficha(int a, int b) : ladoA(a), ladoB(b) {}

    // Getters
    int getLadoA() const { return ladoA; }
    int getLadoB() const { return ladoB; }

    // Comprueba si es un doble (ej. [6|6])
    bool esDoble() const {
        return ladoA == ladoB;
    }

    // Invierte la ficha (ej. [3|5] -> [5|3])
    // Necesario para la lógica de juego
    void invertir() {
        std::swap(ladoA, ladoB);
    }

    // Devuelve una representación en string (ej. "[3|5]")
    std::string toString() const {
        std::stringstream ss;
        ss << "[" << ladoA << "|" << ladoB << "]";
        return ss.str();
    }
};


//****************************************************************************
// 2. CLASE TABLERO
// Representa la mesa de juego.
//****************************************************************************
class Tablero {
private:
    // Un deque (cola de doble fin) es perfecto.
    // Podemos añadir fichas al frente (izquierda) o al final (derecha).
    std::deque<Ficha> fichasJugadas;

public:
    Tablero() {} // Constructor vacío

    bool estaVacio() const {
        return fichasJugadas.empty();
    }

    // Devuelve el valor libre en el extremo izquierdo
    int getExtremoIzquierdo() const {
        if (estaVacio()) return -1; // No hay extremos si está vacío
        return fichasJugadas.front().getLadoA();
    }

    // Devuelve el valor libre en el extremo derecho
    int getExtremoDerecho() const {
        if (estaVacio()) return -1;
        return fichasJugadas.back().getLadoB();
    }

    // Lógica para jugar la primera ficha
    bool jugarPrimeraFicha(Ficha f) {
        if (estaVacio()) {
            fichasJugadas.push_back(f);
            return true;
        }
        return false;
    }

    // Intenta jugar una ficha por la izquierda.
    // Se encarga de invertirla si es necesario.
    bool jugarPorIzquierda(Ficha& f) {
        if (estaVacio()) return false; // Usar jugarPrimeraFicha

        int extremo = getExtremoIzquierdo();
        if (f.getLadoB() == extremo) {
            fichasJugadas.push_front(f);
            return true;
        } else if (f.getLadoA() == extremo) {
            f.invertir();
            fichasJugadas.push_front(f);
            return true;
        }
        return false; // No se pudo jugar
    }

    // Intenta jugar una ficha por la derecha.
    bool jugarPorDerecha(Ficha& f) {
        if (estaVacio()) return false;

        int extremo = getExtremoDerecho();
        if (f.getLadoA() == extremo) {
            fichasJugadas.push_back(f);
            return true;
        } else if (f.getLadoB() == extremo) {
            f.invertir();
            fichasJugadas.push_back(f);
            return true;
        }
        return false; // No se pudo jugar
    }

    // Muestra todas las fichas en la mesa
    void mostrar() const {
        std::cout << "Tablero: ";
        if (estaVacio()) {
            std::cout << "[ Vacío ]" << std::endl;
            return;
        }
        for (const auto& ficha : fichasJugadas) {
            std::cout << ficha.toString() << " ";
        }
        std::cout << std::endl;
    }
};


//****************************************************************************
// 3. CLASE JUGADOR
// Representa a un jugador con su mano de fichas.
//****************************************************************************
class Jugador {
private:
    std::string nombre;
    std::vector<Ficha> mano;

public:
    Jugador(std::string n) : nombre(n) {}

    std::string getNombre() const {
        return nombre;
    }

    void robarFicha(Ficha f) {
        mano.push_back(f);
    }

    // Comprueba si el jugador tiene *alguna* ficha que se pueda jugar
    bool tieneFichaValida(int extremoA, int extremoB) const {
        for (const auto& f : mano) {
            if (f.getLadoA() == extremoA || f.getLadoB() == extremoA ||
                f.getLadoA() == extremoB || f.getLadoB() == extremoB) {
                return true;
            }
        }
        return false;
    }

    // Saca una ficha de la mano por su índice y la devuelve
    Ficha sacarFicha(int indice) {
        Ficha f = mano[indice];
        mano.erase(mano.begin() + indice);
        return f;
    }

    // Devuelve una *referencia* a una ficha, para no copiarla
    Ficha& getFicha(int indice) {
        return mano[indice];
    }

    int getTamanoMano() const {
        return mano.size();
    }

    bool tieneManoVacia() const {
        return mano.empty();
    }

    void mostrarMano() const {
        std::cout << "Mano de " << nombre << ":" << std::endl;
        for (size_t i = 0; i < mano.size(); ++i) {
            std::cout << "  " << i << ": " << mano[i].toString() << std::endl;
        }
    }

    // Busca si el jugador tiene un doble específico (para el inicio)
    int buscarDoble(int valor) const {
        for (size_t i = 0; i < mano.size(); ++i) {
            if (mano[i].esDoble() && mano[i].getLadoA() == valor) {
                return i; // Devuelve el índice de la ficha
            }
        }
        return -1; // No encontrado
    }
};


//****************************************************************************
// 4. CLASE JUEGO
// Orquesta todo: el pozo, los turnos, la lógica principal.
//****************************************************************************
class Juego {
private:
    std::vector<Jugador> jugadores;
    std::vector<Ficha> pozo;
    Tablero tablero;
    int turnoActual; // Índice del jugador actual
    bool juegoTerminado;
    int pasesConsecutivos; // Para detectar un juego trancado

public:
    Juego(const std::vector<std::string>& nombres) {
        for (const auto& n : nombres) {
            jugadores.emplace_back(n); // Crea los jugadores
        }
        turnoActual = 0;
        juegoTerminado = false;
        pasesConsecutivos = 0;
    }

    // Crea las 28 fichas, las baraja y las reparte
    void inicializar() {
        std::cout << "Creando y barajando fichas..." << std::endl;
        
        // 1. Crear las 28 fichas
        for (int i = 0; i <= 6; ++i) {
            for (int j = i; j <= 6; ++j) {
                pozo.emplace_back(i, j);
            }
        }

        // 2. Barajar el pozo
        // Usamos un motor de aleatoriedad moderno
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(pozo.begin(), pozo.end(), std::default_random_engine(seed));

        // 3. Repartir 7 fichas a cada jugador
        int numJugadores = jugadores.size();
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < numJugadores; ++j) {
                jugadores[j].robarFicha(pozo.back());
                pozo.pop_back();
            }
        }

        std::cout << "¡Fichas repartidas! Quedan " << pozo.size() << " en el pozo." << std::endl;
    }

    // Lógica para determinar quién empieza
    // (El que tenga el doble 6, o 5, etc.)
    void determinarPrimerTurno() {
        std::cout << "Determinando quién empieza (doble más alto)..." << std::endl;
        for (int doble = 6; doble >= 0; --doble) {
            for (size_t i = 0; i < jugadores.size(); ++i) {
                int indiceFicha = jugadores[i].buscarDoble(doble);
                if (indiceFicha != -1) {
                    // ¡Encontrado!
                    turnoActual = i;
                    std::cout << jugadores[i].getNombre() << " empieza con el [" << doble << "|" << doble << "]." << std::endl;
                    
                    // Juega la primera ficha
                    Ficha f = jugadores[i].sacarFicha(indiceFicha);
                    tablero.jugarPrimeraFicha(f);
                    
                    // Pasa al siguiente jugador
                    siguienteTurno();
                    return;
                }
            }
        }
        
        // Caso muy raro (ej. 2 jugadores, uno tiene todos los dobles)
        // Por si acaso, empieza el jugador 0.
        std::cout << "No se encontraron dobles. Empezando por Jugador 1 (arbitrario)." << std::endl;
        turnoActual = 0; 
        // En este caso, el primer jugador jugará lo que quiera en un tablero vacío.
    }


    // El bucle principal del juego
    void buclePrincipal() {
        determinarPrimerTurno(); // Decide y juega la primera ficha

        while (!juegoTerminado) {
            mostrarEstadoJuego();
            manejarTurno();
            verificarEstadoJuego();
            if (!juegoTerminado) {
                siguienteTurno();
            }
        }
        
        anunciarGanador();
    }

private:
    void manejarTurno() {
        Jugador& jugadorActual = jugadores[turnoActual];
        std::cout << "\n--- Turno de " << jugadorActual.getNombre() << " ---" << std::endl;
        
        int extA = tablero.getExtremoIzquierdo();
        int extB = tablero.getExtremoDerecho();

        // 1. Revisar si el jugador puede jugar
        bool puedeJugar = jugadorActual.tieneFichaValida(extA, extB);

        // 2. Si no puede, roba hasta que pueda o el pozo se vacíe
        while (!puedeJugar && !pozo.empty()) {
            std::cout << "No tienes fichas para jugar. Robando del pozo..." << std::endl;
            jugadorActual.robarFicha(pozo.back());
            pozo.pop_back();
            jugadorActual.mostrarMano();
            puedeJugar = jugadorActual.tieneFichaValida(extA, extB);
        }

        // 3. Si sigue sin poder jugar (y el pozo está vacío), pasa.
        if (!puedeJugar) {
            std::cout << "El pozo está vacío y no puedes jugar. ¡Pasas el turno!" << std::endl;
            pasesConsecutivos++;
            return;
        }

        // 4. Si puede jugar, resetea los pases y pide la jugada
        pasesConsecutivos = 0;
        jugadorActual.mostrarMano();

        // Bucle para pedir una jugada válida
        bool jugadaRealizada = false;
        while (!jugadaRealizada) {
            int indiceFicha = -1;
            char ladoJugar = ' ';

            // Pedir índice
            while (true) {
                std::cout << "Elige el índice de la ficha a jugar: ";
                if (!(std::cin >> indiceFicha) || indiceFicha < 0 || indiceFicha >= jugadorActual.getTamanoMano()) {
                    std::cout << "Índice inválido. Intenta de nuevo." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    break;
                }
            }

            // Pedir lado
            while (true) {
                std::cout << "¿Jugar por Izquierda (I) o Derecha (D)? ";
                std::cin >> ladoJugar;
                ladoJugar = std::toupper(ladoJugar);
                if (ladoJugar == 'I' || ladoJugar == 'D') {
                    break;
                }
                std::cout << "Opción inválida." << std::endl;
            }

            // Intenta hacer la jugada
            Ficha fichaParaJugar = jugadorActual.getFicha(indiceFicha); // Obtiene copia
            bool exito = false;
            
            if (ladoJugar == 'I') {
                exito = tablero.jugarPorIzquierda(fichaParaJugar);
            } else {
                exito = tablero.jugarPorDerecha(fichaParaJugar);
            }

            if (exito) {
                std::cout << "¡Jugada exitosa!" << std::endl;
                jugadorActual.sacarFicha(indiceFicha); // Saca la ficha de la mano
                jugadaRealizada = true;
            } else {
                std::cout << "¡Jugada inválida! Esa ficha no coincide con ese extremo." << std::endl;
                // El bucle while(!jugadaRealizada) se repetirá
            }
        }
    }

    void siguienteTurno() {
        turnoActual = (turnoActual + 1) % jugadores.size();
    }

    void verificarEstadoJuego() {
        // Opción 1: Un jugador se quedó sin fichas
        if (jugadores[turnoActual].tieneManoVacia()) {
            juegoTerminado = true;
            return;
        }

        // Opción 2: El juego está "trancado"
        if (pasesConsecutivos == jugadores.size()) {
            std::cout << "\n¡JUEGO TRANCADO!" << std::endl;
            std::cout << "Todos los jugadores han pasado. Contando puntos..." << std::endl;
            juegoTerminado = true;
            return;
        }
    }

    void anunciarGanador() {
        std::cout << "\n=========================================" << std::endl;
        std::cout << "           ¡JUEGO TERMINADO!           " << std::endl;
        std::cout << "=========================================" << std::endl;

        // Si se trancó, hay que contar puntos (implementación simplificada)
        if (pasesConsecutivos == jugadores.size()) {
            // Aquí iría la lógica de contar puntos.
            // Por simplicidad, declaramos un empate o el último que jugó.
            std::cout << "El juego se ha trancado. El ganador es quien tenga menos puntos (lógica no implementada)." << std::endl;
            // En este código, el "ganador" será el último que jugó si no se tranca.
        } else {
            // El ganador es el último que jugó (turnoActual) porque se quedó sin fichas
            std::cout << "¡¡¡ FELICIDADES, " << jugadores[turnoActual].getNombre() << " !!!" << std::endl;
            std::cout << "¡Has ganado la partida!" << std::endl;
        }
    }

    void mostrarEstadoJuego() {
        std::cout << "\n-----------------------------------------" << std::endl;
        tablero.mostrar();
        std::cout << "Fichas en el pozo: " << pozo.size() << std::endl;
        for(const auto& j : jugadores) {
            std::cout << "  " << j.getNombre() << " tiene " << j.getTamanoMano() << " fichas." << std::endl;
        }
        std::cout << "-----------------------------------------" << std::endl;
    }
};


//****************************************************************************
// 5. FUNCIÓN MAIN
// Punto de entrada del programa.
//****************************************************************************
int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "      BIENVENIDO AL JUEGO DE DOMINÓ      " << std::endl;
    std::cout << "            Versión C++                  " << std::endl;
    std::cout << "=========================================" << std::endl;

    int numJugadores = 0;
    while(numJugadores < 2 || numJugadores > 4) {
        std::cout << "¿Cuántos jugadores (2-4)? ";
        std::cin >> numJugadores;
        if(std::cin.fail() || numJugadores < 2 || numJugadores > 4) {
            std::cout << "Entrada inválida." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            numJugadores = 0;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar buffer

    std::vector<std::string> nombres;
    for (int i = 0; i < numJugadores; ++i) {
        std::string nombre;
        std::cout << "Nombre del Jugador " << i + 1 << ": ";
        std::getline(std::cin, nombre);
        nombres.push_back(nombre);
    }

    // Crear y empezar el juego
    Juego miJuego(nombres);
    miJuego.inicializar();
    miJuego.buclePrincipal();

    return 0;
}