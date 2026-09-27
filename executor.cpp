#include "executor.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <cstring>

using namespace std;
static vector<pid_t> procesos_activos;

void manejador_sigint(int senial){
  (void)senial;
  cout<<"\n[ALERTA] Inspección detectada (SIGINT). Cancelando tareas..." << endl;
    for (pid_t pid:procesos_activos){
    if (pid > 0){kill(pid, SIGKILL);}}
    exit(1);}

void procesar_plan(DAG& dag, int K){ 
 struct sigaction sa_sigint;
 sa_sigint.sa_handler=manejador_sigint;
 sigemptyset(&sa_sigint.sa_mask);
 sa_sigint.sa_flags=0;
 sigaction(SIGINT,&sa_sigint,NULL);
   for(auto& [id, tarea]:dag){
     for(const auto& dep_id:tarea.dependencias){
       if(dag.count(dep_id)){
         dag[dep_id].hijas.push_back(id);}}}

queue<string> pendientes;
 for (auto& [id, tarea]:dag){
 if (tarea.deps_pendientes==0){pendientes.push(id);}}

int en_ejecucion=0;
while (!pendientes.empty() || en_ejecucion>0){
 while (en_ejecucion < K && !pendientes.empty()){
 string id_actual=pendientes.front();
 pendientes.pop();

Actividad& tarea=dag[id_actual];
if (tarea.cancelado) continue;
int tuberia[2];
if (pipe(tuberia)== -1){
perror("Fallo al crear la tuberia");
exit(1);}

pid_t pid=fork();
if (pid < 0) {
perror("Fallo al crear proceso hijo");
exit(1);} 
        
else if (pid==0){
close(tuberia[0]);
cout << "[PID" << getpid() << "] Inicio tarea: " << tarea.nombre 
<< " (" << tarea.tiempo_ms << " ms)" <<endl;
 usleep(tarea.tiempo_ms * 1000);
 string msg = "Completada actividad: " + tarea.nombre;
 write(tuberia[1], msg.c_str(), msg.length() + 1);
 close(tuberia[1]);
 _exit(0);} 
          
 else{
close(tuberia[1]);
en_ejecucion++;
procesos_activos.push_back(pid);
 char buffer[256];
 ssize_t n=read(tuberia[0], buffer, sizeof(buffer));
 if (n > 0){
 cout << "[PIPE] Notificacion recibida en el padre: \"" << buffer << "\"" << endl;
 }
 close(tuberia[0]);}}

if (en_ejecucion>0){
int estado;
pid_t pid_finalizado=waitpid(-1, &estado, 0);
if (pid_finalizado > 0){ en_ejecucion--;
 for (auto it = procesos_activos.begin(); it != procesos_activos.end(); ++it) {
 if (*it == pid_finalizado){
 procesos_activos.erase(it);
 break;}}

if (WIFEXITED(estado) && WEXITSTATUS(estado) == 0){
  for (auto& [id, tarea] : dag){
  for (const string& id_hija : tarea.hijas){
   dag[id_hija].deps_pendientes--;
  if (dag[id_hija].deps_pendientes == 0 && !dag[id_hija].cancelado){
  pendientes.push(id_hija);}}}}}}

cout << "\n[FIN] Todas las actividades del plan finalizaron con exito."<< endl;
}
}
