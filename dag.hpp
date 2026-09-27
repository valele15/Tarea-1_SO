#ifndef DAG_HPP
#define DAG_HPP
#include <vector>
#include <string>
#include <unordered_map>

struct Actividad {
std::string id;
std::string nombre;
int tiempo_ms;
std::vector<std::string> dependencias;
std::vector<std::string> hijas;
int deps_pendientes=0;
bool cancelado=false; };

using DAG= std::unordered_map<std::string, Actividad>;
DAG leer_plan(const std::string & ruta_archivo);
#endif
