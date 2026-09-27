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
