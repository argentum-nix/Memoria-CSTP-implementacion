# Memoria-CSTP-implementacion
## Sobre la estructura del programa

### Archivos
- `casualty.cpp` - contiene definiciones de la clase `Casualty` que representa una victima en el sistema
- `vehicle.cpp` - contiene definiciones de la clase `Vehicle` que representa un vehículo en el sistema
- `hospital.cpp` - contiene definiciones de la clase `Hospital` que representa un hospital en el sistema
- `instance.cpp` - contiene definiciones de la clase `Instance`, que permite cargar y guardar datos de la instancia, como también contiene abstracciones de acceso a los métodos de las clases anteriores
-  `solver.cpp` - contiene los algoritmos que se encargan de la solución del problema

### Directorios
-  `/gantt util` - directorio con un jupyter notebook que se utiliza para graficar los resultados mediante análisis del archivo de resultados 
-  `/instances` - directorio que contiene las instancias y el archivo único de la red de transporte

El programa depende de la existencia de dos archivos: `<nombreinstancia>.txt` y `<nombrered>.txt`. Estos archivos deben estar en el mismo directorio, el cual se debe indicar durante la ejecución vía parámetro. Se detallan los pasos de compilación en la siguiente sección.

## Compilación y Ejecución

#### Compilación
Ejecutar el siguiente comando en el directorio principal:
`make clean;make`

#### Ejecución: 
El programa recibe varios parámetros. Un ejemplo de comando de ejecución es:
`./CSTPSolver ./instances/ 1 network 1 1 10 123 0.8 0.2 H > output.txt`

Donde:
- `./CSTPSolver` es el archivo ejecutable creado en la fase de compilación por el makefile
- `./instances/` es la ruta relativa a la de makefile, que indica la ubicación de archivo de instancia
- `1`  es el nombre, sin extensión, de la instancia - en este caso, se refiere a la instancia `1.txt`
- `network` es el nombre,sin extensión, del archivo del grafo de transporte - en este caso, se refiere al archivo `network.txt`
- `1` - flag de heurística. Valor 1 indica que se debe utilizar la heurística para mejorar soluciones. 0 eoc.
- `1` - flag de GRASP. Valor 1 indica que se debe usar procedimiento de Greedy Aleatorizado en la fase de construcción. 0 eoc.
- `10` - tamaño de la ventana a utilizarse en el procedimiento de GRASP. Se toma en cuenta solo si el flag de GRASP no es nulo.
- `123` - valor de la semilla a utilizar en el procedimiento de GRASP. Se toma en cuenta solo si el flag de GRASP no es nulo.
- `0.8` - es el valor de la importancia de cercanía de un vehículo a la hora de calcular su "prioridad de selección". Se toma en cuenta solo si flag de heurística no es nulo.
- `0.2` - es el valor de la importancia de disponibilidad de un vehículo a la hora de calcular su "prioridad de selección". Se toma en cuenta solo si flag de heurística no es nulo.
- `H` - indica que se debe minimizar el trayecto total, desde aparición de la víctima hasta el momento de su hospitalización. Posibles valores:
    - `H` (minimizar trayecto total)
    - `S` (minimizar tiempo entre aparición y estabilización lograda)
    - `C` (minimizar tiempo entre asignación de vehículo y estabilización lograda)
##### Ejemplos de comandos de ejecución:

Nota: en los ejemplos, `output.txt` es un archivo a cual se redirigen los cout con soluciones obtenidas.
  
*  `./CSTPSolver ./instances/ 1 network 0 0 10 123 0.8 0.2 H > output.txt` - se utilizará Greedy no aleatorizado sin heurística de mejora
* `./CSTPSolver ./instances/ 1 network 0 1 10 123 0.8 0.2 C > output.txt` - utlizará solo GRASP con ventana 10 y semilla 123, sin heurística de mejora

## Sobre el cálculo de la calidad de solución
El programa utiliza dos formas de cálculo (sin depender de si se usa heurística, grasp, greedy no aleatorizado, etc). 
- Por cada periodo, se hace un cálculo de las rutas propias de este periodo. Por ejemplo, el algoritmo está analizando Periodo 2 con víctimas 36-109, solo se aplicará el cálculo a estas. Es decir, no se sumarán las víctimas ya atendidas en periodo anterior (1-38). Este es el valor que intentará minimizar la heurística (valor local de periodo).
- Una vez que se termine el periodo, adicionalmente se calcula el valor "final de la solución hasta el momento". Esta solución usa para el cálculo todas las rutas aceptadas (no descartadas ni reiniciadas) para periodo anterior, y las rutas creadas para periodo actual. En el ejemplo anterior, esto sería equivalente a calcular calidad para rutas de víctimas 1-109. Este valor no afecta la ejecución.

El calculo de prioridad se hace como una sumatoria de prioridad*X, donde X es el tiempo específico que describe a la victima. El tiempo se define por el último parámetro de ejecución (de valores H, S o C)

## Sobre la selección de víctima en heurística de re-enrutamiento forzado 
En la heurística se seleccionan las víctimas para el re-enrutamiento forzado bajo dos condiciones (definidas en función `checkIfHighST`):
- La víctima debe haber sido enrutada con helicoptero por el greedy constructor (aleatorizado o no)
- La víctima debe tener un alto tiempo de estabilización - es decir, debe ser de gravedad 3 o de gravedad 2 y edad 1 o 2.

Si la víctima cumple con estas condiciones, se re-enruta con una ambulancia. La selección de esta se hace, minimizando el valor de "prioridad" calculado como:

`X * cercanía + Y * tiempo_de_siguiente_disponibilidad`
Donde X e Y son parámetros pasados al ejecutable. 

- cercanía - cantidad de minutos de viaje desde la posición de una ambulancia hasta la víctima
- tiempo de siguiente disponibilidad - cantidad de minutos, calculada como `hora_en_que_ambulancia_puede_hacer_nueva_ruta - hora_actual`

Se selecciona una ambulancia cuyo valor total de cálculo anterior es mínimo y esta se ocupa con la víctima seleccionada.En base a esta asignacion, se reasignan todas las víctimas posteriores a la analizada para el periodo actual.
