#include <iostream>
#include <cstdlib>
#include "dag.hpp"
#include "executor.hpp"

using namespace std;
int main(int argc, char* argv[]){
 if (argc < 3){
  cerr << "Error: Parametros insuficientes." << endl;
  cerr << "Uso correcto: " << argv[0] << " <archivo_plan.txt> <K>" << endl;
return 1;}

string ruta_archivo = argv[1];
int K = atoi(argv[2]);

if (K <= 0){
cerr << "Error: El limite K debe ser mayor a 0." << endl;
return 1;}

DAG dag = leer_plan(ruta_archivo);
 if (dag.empty()){
  cerr << "Error: No se pudieron cargar actividades o el archivo está vacio." << endl;
return 1;}

procesar_plan(dag, K);
return 0;}
