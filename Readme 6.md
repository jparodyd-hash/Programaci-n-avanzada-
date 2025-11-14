
Juego de Dominó en C++

Este proyecto es una implementación del clásico juego de dominó, desarrollado en C++ y diseñado para jugarse en la consola. El juego soporta de 2 a 4 jugadores y sigue las reglas estándar del dominó, incluyendo el reparto de fichas, el inicio con el doble más alto, el robo del "pozo" y la detección de juegos ganados o "trancados".

Características

  * **Juego Clásico:** Implementa las reglas del dominó tradicional.
  * **Multijugador (Local):** Soporta de 2 a 4 jugadores en la misma consola.
  * **Interfaz de Consola:** Toda la interacción se realiza a través de texto en la terminal.
  * **Lógica de Juego Completa:**
      * Creación y barajado aleatorio del set de 28 fichas.
      * Reparto automático de 7 fichas por jugador.
      * Determinación automática del primer jugador (basado en el doble más alto).
      * Lógica para robar del "pozo" cuando un jugador no tiene jugadas válidas.
      * Manejo de "pases" de turno si el pozo está vacío y no hay jugadas.
  * **Detección de Fin de Juego:** El juego termina cuando:
      * Un jugador se queda sin fichas (¡Dominó\!).
      * El juego se "tranca" (todos los jugadores pasan consecutivamente).
  * **Diseño Orientado a Objetos (OOP):** El código está estructurado en clases claras y cohesivas: `Ficha`, `Tablero`, `Jugador` y `Juego`.

-----

Cómo Compilar y Ejecutar

Para compilar y ejecutar este proyecto, necesitarás un compilador de C++ (como g++ o Clang) que soporte el estándar C++11 o superior.

### 1\. Guardar el Código

Guarda el código fuente en un archivo llamado, por ejemplo, `domino.cpp`.

### 2\. Compilar

Abre una terminal o línea de comandos, navega hasta el directorio donde guardaste el archivo y ejecuta el siguiente comando:

```bash
# Se recomienda usar -std=c++11 (o superior) por el uso de <random> y <chrono>
g++ -std=c++11 domino.cpp -o domino
```

Esto creará un archivo ejecutable llamado `domino` (o `domino.exe` en Windows).

### 3\. Ejecutar

Una vez compilado, puedes iniciar el juego ejecutando:

```bash
./domino
```

O en Windows:

```bash
.\domino.exe
```

-----

Cómo Jugar

1.  Inicia el programa.
2.  Introduce el **número de jugadores** (de 2 a 4).
3.  Introduce el **nombre** de cada jugador.
4.  El juego comenzará automáticamente. El jugador con el doble más alto (ej. [6|6]) pondrá la primera ficha en el tablero.
5.  En tu turno, verás el estado actual del tablero y tu mano de fichas (cada una con un número de **índice**).
6.  Si puedes jugar, el programa te pedirá:
      * El **índice** de la ficha que quieres jugar (ej. `0`, `1`, `2`, ...).
      * El **lado** donde quieres jugarla: `I` (para el extremo izquierdo) o `D` (para el extremo derecho).
7.  Si la jugada es válida, la ficha se colocará. Si intentas jugar una ficha que no coincide con el extremo, el juego te lo notificará para que lo intentes de nuevo.
8.  Si no tienes fichas válidas en tu mano, el juego te hará "robar" del pozo automáticamente, una por una, hasta que obtengas una ficha jugable.
9.  Si el pozo se vacía y sigues sin tener una ficha jugable, "pasarás" tu turno.
10. El juego termina cuando un jugador se queda sin fichas o cuando todos los jugadores pasan su turno consecutivamente (juego trancado).

-----

Estructura del Código

El proyecto está dividido en cuatro clases principales para una clara separación de responsabilidades:

  * ### `Ficha`

    Representa una única ficha de dominó (ej. `[5|3]`). Contiene métodos para:

      * Obtener sus valores (`getLadoA`, `getLadoB`).
      * Saber si es un doble (`esDoble`).
      * Invertir sus lados (`invertir`).
      * Convertirse a `string` (`toString`).

  * ### `Tablero`

    Modela la mesa de juego. Utiliza un `std::deque` (cola de doble extremo) que permite añadir fichas eficientemente por ambos lados.

      * Maneja los extremos izquierdo y derecho (`getExtremoIzquierdo`, `getExtremoDerecho`).
      * Permite jugar por la izquierda o la derecha (`jugarPorIzquierda`, `jugarPorDerecha`), encargándose de invertir la ficha si es necesario.

  * ### `Jugador`

    Almacena el nombre del jugador y su "mano" (un `std::vector` de `Ficha`). Tiene lógica para:

      * Robar fichas (`robarFicha`).
      * Verificar si tiene movimientos válidos (`tieneFichaValida`).
      * Jugar una ficha (`sacarFicha`).
      * Buscar el doble más alto para iniciar (`buscarDoble`).

  * ### `Juego`

    Es la clase principal que orquesta toda la partida. Contiene:

      * El "pozo" (un `std::vector` de `Ficha`).
      * La lista de jugadores.
      * Una instancia del `Tablero`.
      * La lógica del bucle principal (`buclePrincipal`), manejo de turnos (`manejarTurno`) y condiciones de victoria (`verificarEstadoJuego`).