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
  
