# Tarea-1_SO
# Valentina Mella-Paulette Saavedra 
# Seccion 2
1)-plan.txt: plan de actividades diciocheras 
parser o parseo: que actividades existen y de que depende. para esto usamos el plan.txt en que este se encuentre todo en una linea,pero queremos clasificarlo en un plan de actividades como sale en el enunciado.

Algo asi:

  -ID_Actividad:Identificador unico alfanunmerico del nodo 
  -Nombre_Actividad:Etiqueta descirptiva de la accion 
  -Tiempo(tiempo_ms):El tiempo estimado de la actividad en milisegundos
  -Depenendencias:Lista de IDs separados por comas.Estas actividaes deben haber finalizado anted de que el nodo actual pueda ejecutarse

  Como una especie de lista u separarllo en vez de hacerlo en una line 
  Si el tiempo viene vacio,asignar un valor aleatoria entre 100 y 5000 ms 
  
2)-planificador.cpp:

Para aquello tengo que ir leyendo el archivo por id,nombre,tiempo_ms y dependencias por lo qeu se separe cada uno con n:para guardar todo como en una estructura,como una especie de lista o actividad.
Luego de hacer eso el id y el nombre se guardan automaticamente,mientras que el tiempo se convertia en texto a numero con stoi y se ponia como mencione enates que si tenia vacio se generaba uno aleatoriamente entre 100 y 5000. Las dependencias como vienen juntas,simplemente guardamos cada una poniendole entre medio una coma para asi guardarlas cada una por separada en un vector,lo de los numero aleatorios se hace con srand que hace que permita asignarle un tiempo en este caso le agregamos tiempo nulo para que sea mas aleatorio en vex de que salga la misma actividad  a cada rato.
Tambien en el codigo abrimos el plan.txt para poder leerlo y seguir el formato. Guardamos todo en una misma linea luego va a tener una estructura como de lista. Para el tiempo simplemente se hacen ambos casos como el que es vacio y el que viene con el tiempo incluido.


3)-dag.cpp:

el archivo se va a encargar de leer plan.txt y construir el dag con las actividades. Para esto se abre el archivo y se va leyendo linea por linea,separando cada dato : para obtener ID,nombre,tiempo y dependencias. La funcion quitarEspacios() sirve para eliminar los espacios que quedan al leer datos en el archivo. Si el tiempo viene vacio se elige un valor aleatoriamente entre 100 y 5000 ms,si viene con un numero o definido,entonces se convierte usando stoi. Las dependencias las separando utilizando una coma y se iban guardando por separado en un vector. Cada actividad se iba guardando dentro de DAG utilizando la ID. Ademas se calcula la cantidad de dependencias pendientes y se crea una relacion entre las actividades mediante vectores. Finalmente, se retorna el DAG con toda la información de las actividades para continuar con la ejecución del plan. 


4)-generado.py:

Creamos el generador para poder usalro como archivo de datos.osea un planificador de hasta 10,000 actividades.
El programa va a generar un archivo llamado plan_grande.txt con las actividades 


*Compilacion y ejecucion:

Para compilar el programa utilizamos g++ , g++ -Wall -Wextra -std=c++17 -lpthread main.cpp dag.cpp executor.cpp -o planificador con esto. 
Una vex compilado el programa, este se ejecuta indicando el archivo que tiene el plan de actividades y limita k 
Por ejmplo cuanfo ejecuto ./planificador plan.txt 2, lo que hace el progrma es utilizar plan.txt como archivo y establece maximo 2 procesos ejecutandose al mismo tiempo. 
Para realizar la prueba de estres,se puede ejecutar previamente generador.py,que genera una archivo con una hartas actividades para poder utilizarlas luego en el planificador 

------------------------------------------------------------------------------------------------------------------------

*Modulo Ejecutor y Gestion de Procesos

1)-Manejo de procesos con fork: 

para ejecutar cada actividad del plan independientemente de la otra usamos fork(),o sea, cada vez que una tarea queda lista porque ya no tiene dependencias pendientes, el proceso padre crea un hijo que se encarga de simular la ejecucion de la actividad con usleep() segn los milisegundos indicados, y así cada tarea corre en su propio espacio de memoria sin molestar al resto.

2)-Control de concurrencia K: 

para no saturar el sistema y cumpir el limite de concurrencia K llevamos un contador de procesos en_ejecucion y una cola de tareas pendientes, entonces mientras haya cupos disponibles y tareas en la cola el padre va lanzando hijos, y cuando ya se llega al limite K el padre se queda esperando a que al menos un hijo termine con waitpid(-1, &estado, 0), lo que nos deja liberar el cupo al tiro y lanzar la siguiente tarea sin andar con busy-waiting, o sea espera activa, para no gastar CPU de más.

3)-Comunicación con tuberías: 

para la comunicacion entre hijo y padre usamos tuberías anonimas con pipe(), y la idea es que antes del fork() el padre crea la tubería, luego el hijo al terminar su trabajo escribe un mensaje de confirmacion en la tubería y cierra su extremo de escritura con write,el padre lee esa notificación con read, confirma que el insumo o la tarea está lista y cierra sus descriptores para no dejar recursos colgando.

4)-aislamiento de errores: 

al usar procesos separados nos da aislamiento de memoria natural, así que si un hijo falla o termina de forma rara no rompe ni al padre ni a los demás hijos, y el padre revisa el estado de retorno de cada hijo con WIFEXITED y WEXITSTATUS, y solo si el hijo terminó bien con código 0 el padre descuenta las dependencias de las tareas hijas y las mete en la cola de listos para ejecutarlas después.

5)-SIGINT:

para simlar el corte por inspeccion se configuro la captura de SIGINT con sigaction, y se mantuvo un registro dinamico d los PIDs de todos los hijos activos en procesos_activos, de esa forma se activa la funcion manejadora que recorre ese arreglo de PIDs y le manda un kill(pid, SIGKILL) a cada hijo activo para detner todo antes de que termine el programa.

-------------------------------------------------------------------------------------------------------------------------

*Decisiones de Diseño (Módulo Ejecutor)

-Cambio de nombre a leer_plan: en el módulo del DAG/Parser, se decidio renombrar la función de lectura (inicialmente pensada como parseo_plan) a leer_plan().Ya que con esto el nombre refleja de manera más directa su verdadero proposito: abrir el archivo .txt, interpretar las líneas y armar el mapa del DAG.

-Función principal procesar_plan: nombramos la función del ejecutor como procesar_plan() para mantener una lectura mas natural en el código.

-Simplificación de variables: usar nombres simples para las variables internas (como procesos_activos, tuberia, pendientes y en_ejecucion), logrando un código limpio y más legible.

