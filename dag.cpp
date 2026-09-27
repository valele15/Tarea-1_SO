#include "dag.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

string quitarEspacios(string texto) {

    while (!texto.empty() && texto[0] == ' ') {
        texto.erase(0, 1);
    }

    while (!texto.empty() && texto[texto.size() - 1] == ' ') {
        texto.erase(texto.size() - 1, 1);
    }

    return texto;
}

DAG leer_plan(const string& ruta_archivo) {

    srand(time(NULL));

    ifstream archivo(ruta_archivo);

    DAG dag;

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << endl;
        return dag;
    }

    string linea;

    
    while (getline(archivo, linea)) {

        if (linea.empty()) {
            continue;
        }

        
        vector<string> campos;

        size_t inicio = 0;

        while (true) {

            size_t posicion = linea.find(':', inicio);

            if (posicion == string::npos) {

                campos.push_back(
                    linea.substr(inicio)
                );

                break;
            }

            campos.push_back(
                linea.substr(
                    inicio,
                    posicion - inicio
                )
            );

            inicio = posicion + 1;
        }

    
        Actividad actividad;

        actividad.id = quitarEspacios(campos[0]);

        
        actividad.nombre = quitarEspacios(campos[1]);

        
        string tiempo = quitarEspacios(campos[2]);

        if (tiempo.empty()) {

            actividad.tiempo_ms =
                100 + rand() % 4901;

        } else {

            actividad.tiempo_ms =
                stoi(tiempo);
        }

        string dependencias =
            quitarEspacios(campos[3]);

        if (!dependencias.empty()) {

            size_t inicioDependencia = 0;

            while (true) {

                size_t posicion =
                    dependencias.find(
                        ',',
                        inicioDependencia
                    );

                if (posicion == string::npos) {

                    string dependencia =
                        dependencias.substr(
                            inicioDependencia
                        );

                    actividad.dependencias.push_back(
                        quitarEspacios(dependencia)
                    );

                    break;
                }

                string dependencia =
                    dependencias.substr(
                        inicioDependencia,
                        posicion - inicioDependencia
                    );

                actividad.dependencias.push_back(
                    quitarEspacios(dependencia)
                );

                inicioDependencia = posicion + 1;
            }
        }

        
        actividad.deps_pendientes =
            actividad.dependencias.size();

        
        dag[actividad.id] = actividad;
    }

    archivo.close();

    
    for (auto& [id, actividad] : dag) {

        for (string dependencia : actividad.dependencias) {

            if (dag.count(dependencia)) {

                dag[dependencia].hijas.push_back(id);
            }
        }
    }

    return dag;
}