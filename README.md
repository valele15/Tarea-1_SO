# Tarea-1_SO

-plan.txt: plan de actividades diciocheras 
parser o parseo: que actividades existen y de que depende. para esto usamos el plan.txt en que este se encuentre todo en una linea,pero queremos clasificarlo en un plan de actividades como sale en el enunciado.

Algo asi:

  -ID_Actividad:Identificador unico alfanunmerico del nodo 
  -Nombre_Actividad:Etiqueta descirptiva de la accion 
  -Tiempo(tiempo_ms):El tiempo estimado de la actividad en milisegundos
  -Depenendencias:Lista de IDs separados por comas.Estas actividaes deben haber finalizado anted de que el nodo actual pueda ejecutarse

  Como una especie de lista u separarllo en vez de hacerlo en una line 
  Si el tiempo viene vacio,asignar un valor aleatoria entre 100 y 5000 ms 
  
-planificador.cpp:

Para aquello tengo que ir leyendo el archivo por id,nombre,tiempo_ms y dependencias por lo qeu se separe cada uno con n:para guardar todo como en una estructura,como una especie de lista o actividad.
Luego de hacer eso el id y el nombre se guardan automaticamente,mientras que el tiempo se convertia en texto a numero con stoi y se ponia como mencione enates que si tenia vacio se generaba uno aleatoriamente entre 100 y 5000. Las dependencias como vienen juntas,simplemente guardamos cada una poniendole entre medio una coma para asi guardarlas cada una por separada en un vector,lo de los numero aleatorios se hace con srand que hace que permita asignarle un tiempo en este caso le agregamos tiempo nulo para que sea mas aleatorio en vex de que salga la misma actividad  a cada rato.
Tambien en el codigo abrimos el plan.txt para poder leerlo y seguir el formato. Guardamos todo en una misma linea luego va a tener una estructura como de lista. Para el tiempo simplemente se hacen ambos casos como el que es vacio y el que viene con el tiempo incluido.

-generado.py:

Creamos el generador para poder usalro como archivo de datos.osea un planificador de hasta 10,000 actividades.
El programa va a generar un archivo llamado plan_grande.txt con las actividades 


-Compilacion y ejecucion:

Para compilar el programa utilizamos g++ , g++ -Wall -Wextra -std=c++17 -lpthread main.cpp dag.cpp executor.cpp -o planificador con esto. 
Una vex compilado el programa, este se ejecuta indicando el archivo que tiene el plan de actividades y limita k 
Por ejmplo cuanfo ejecuto ./planificador plan.txt 2, lo que hace el progrma es utilizar plan.txt como archivo y establece maximo 2 procesos ejecutandose al mismo tiempo. 
Para realizar la prueba de estres,se puede ejecutar previamente generador.py,que genera una archivo con una hartas actividades para poder utilizarlas luego en el planificador 

------------------------------------------------------------------------------------------------------------------------

*Módulo Ejecutor y Gestión de Procesos

1) Manejo de Procesos (`fork`)
Para ejecutar cada actividad del plan de manera independiente, utilizamos la llamada al sistema `fork()`. Cada vez que una tarea queda lista (es decir, cuando ya no tiene dependencias pendientes), el proceso padre crea un proceso hijo. Este hijo es el encargado de simular la ejecución de la actividad usando `usleep()` según el tiempo en milisegundos indicado. De esta forma, cada tarea se ejecuta en su propio espacio de memoria sin interferir con el resto.

2) Control de Concurrencia ($K$)
Para no saturar el sistema y cumplir con el límite de concurrencia $K$, llevamos el control con un contador de procesos `en_ejecucion` y una cola de tareas `pendientes`. 
- Mientras haya cupos disponibles (`en_ejecucion < K`) y tareas en la cola, el padre lanza nuevos procesos hijos.
- Cuando se alcanza el límite $K$, el proceso padre se queda esperando a que al menos un hijo termine mediante `waitpid(-1, &estado, 0)`. Esto nos permite liberar el cupo inmediatamente y lanzar la siguiente tarea sin hacer uso de *busy-waiting* (espera activa), cuidando el consumo de CPU.

3) Comunicación mediante Tuberías (`pipe`)
Utilizamos tuberías anónimas (`pipe()`) para la comunicación entre el proceso hijo y el proceso padre:
- Antes de hacer el `fork()`, el padre crea una tubería.
- Al terminar su trabajo, el proceso hijo escribe un mensaje de confirmación en la tubería y cierra su extremo de escritura (`write`).
- El proceso padre lee esta notificación (`read`), confirma que el insumo/tarea está listo y cierra sus descriptores correspondientes para evitar fugas de recursos.

4) Aislamiento de Errores
El uso de procesos separados nos otorga aislamiento de memoria natural. Si un proceso hijo falla o termina de forma inesperada, no rompe la ejecución del proceso padre ni de los demás hijos.
El padre analiza el estado de retorno de cada hijo con `WIFEXITED` y `WEXITSTATUS`. Solo si el hijo finalizó exitosamente (código `0`), el padre descuenta las dependencias de las tareas hijas vinculadas y las agrega a la cola de listos para su posterior ejecución.

5) Manejo de la señal `SIGINT` (Inspección / Ctrl+C)
Para simular el corte por inspección, configuramos la captura de la señal `SIGINT` mediante `sigaction`:
- Mantenemos un registro dinámico de los PIDs de todos los hijos en ejecución (`procesos_activos`).
- Si se presiona `Ctrl+C`, se activa nuestra función manejadora, la cual recorre el arreglo de PIDs y envía un `kill(pid, SIGKILL)` a cada hijo activo para detener de inmediato todos los procesos antes de finalizar el programa.

-------------------------------------------------------------------------------------------------------------------------

*Decisiones de Diseño (Módulo Ejecutor)

-Cambio de nombre a `leer_plan`: En el módulo del DAG/Parser, se decidio renombrar la función de lectura (inicialmente pensada como `parseo_plan`) a `leer_plan()`.Ya que con esto el nombre refleja de manera más directa su verdadero proposito: abrir el archivo `.txt`, interpretar las líneas y armar el mapa del DAG.

-Función principal `procesar_plan`: Nombramos la función del ejecutor como `procesar_plan()` para mantener una lectura mas natural en el código.

-Simplificación de variables: Usar nombres simples para las variables internas (como `procesos_activos`, `tuberia`, `pendientes` y `en_ejecucion`), logrando un código limpio y más legible.

