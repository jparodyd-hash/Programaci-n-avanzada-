Clases complex (binomial) y polar (polar) + acumula
Descripción

Implementación dual de números complejos en forma binomial y polar, con operadores aritméticos y conversión cruzada entre ambos formatos.
Incluye una función acumula que permite sumar un número indefinido de complejos o polares.

Características
Clase complex (binomial a + ib):

Representación estándar de números complejos.

Operadores: +, -, *, /, conj.

Conversión automática desde un objeto polar.

Métodos para magnitud y argumento.

Conversión explícita a polar.

Clase polar (r·e^{iθ}):

Representa complejos mediante módulo y argumento.

Operadores: +, -, *, /, conj.

Suma y resta convertidas internamente a forma binomial.

Conversión explícita a complex.

Función acumula

Recibe cualquier número de argumentos complex o polar
Usa funciones variádicas en C++ (pack expansion).

Devuelve la suma total en forma binomial.

Uso esperado

Sistemas eléctricos, señales, física cuántica, telecomunicaciones, matemáticas aplicadas y cualquier dominio que use números complejos.