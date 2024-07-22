#include <iostream>
#include "ControlParejas.hpp"
#include <iomanip>
#include <stdlib.h>
monitor::monitor(){        //constructor de la clase monitor que deja las variables compartidas criticas en el estado apropiado
    sillasOc=0;
    asignacionParejas=false;
    contNipSabenPareja=0;
    parejasTer=0;
    for (int i = 0; i < N_FIL; i++){
        finMaxFil[i].permiso=false;
    }
} 

//Procedimiento que permite sentarse a un alumno en las sillas de forma atómica
void monitor::sentarseEnSilla(int nip){
    unique_lock<mutex> lck(mtxMonitor);
    while(!(sillasOc<2)){
        sillasMenor2.wait(lck);
    }
    silla[sillasOc]=nip;
    sillasOc++;

    sillasMenor2.notify_all();
    sillasOcIgual2.notify_one();
};

//Procediento que espera a que el profesor de el permiso para que se proceda con la asignacion de parejas,
//una vez dado el permiso, se actualizan las varibales "miPareja" y "filaExam" con la correspondiente 
//información del compañero asignado y fila asignada
void  monitor::asignacionParejasAlumno(int nip,int& miPareja,int& filaExam){
    unique_lock<mutex> lck(mtxMonitor);
    while(!asignacionParejas){
            asignacionParejasTrue.wait(lck);
    }
    miPareja=pareja[nip];
    contNipSabenPareja++;
    filaExam=fila;

    contNipSabenParejaIgual2.notify_one();
    
};

//Procedimiento que se encarga de realizar el asignado de permiso para que el compañero de NIP mayor
//muestre los resultados del examen por pantalla una vez acabe su parte del examen y asigna el resultado 
//de calcular el máximo de su fila
void  monitor::calculoMax(int max,int filaExam){
    unique_lock<mutex> lck(mtxMonitor);
    finMaxFil[filaExam].resultado=max;
    finMaxFil[filaExam].permiso=true;

    //maxFila.notify_all();
    finMaxFilaPermisoTrue.notify_all();
};

//Procedimiento que espera a que el compañero de NIP menor termine de calcular el máximo de la fila,
//una vez acabe y este nos de permiso, el procedimiento se encargará de mostrar los resultados calculados
//por la pareja en pantalla
void  monitor::permisoAlumno(int filaExam,int nip,int miPareja,int suma){
    unique_lock<mutex> lck(mtxMonitor);
    while(!(finMaxFil[filaExam].permiso==true)){
        finMaxFilaPermisoTrue.wait(lck);
    }
    cout<<setfill(' ')<<setw(2)<<filaExam<<setw(2)<<"|"<<setw(6)<<miPareja<<"-"<<nip<<setw(6)<<"|"<<setw(6)<<finMaxFil[filaExam].resultado<<setw(6)<<"|"<<setw(12)<<suma<<endl;
    parejasTer++;

    parejasTerIgual30.notify_one();

};

//procedimiento encargado de esperar a que 2 alumnos se encuentren sentados esperando a que se les asigne una fila,
//una vez sentados, el profesor les asigna una fila, comunica la respectiva pareja a los 2 alumnos y permite que se asignen
void  monitor::asignacionParejasProfesor(int i){
    unique_lock<mutex> lck(mtxMonitor);
    while(!(sillasOc==2)){
        sillasOcIgual2.wait(lck);
    }
    pareja[silla[0]]=silla[1];
    pareja[silla[1]]=silla[0];
    fila=i;
    asignacionParejas=true;
    
    asignacionParejasTrue.notify_all();
    
    
};

//procedimineto que espera a que los 2 alumnos sepan quien es su compañero, una vez se realiza esta acción,
//se reinician las varibales de conteo de alumnos sentados, el número de alumnos que sabe su otra pareja y
//se les quita el permiso para que se asigne otra pareja al alumno
void  monitor::reincioParejasProfesor(){
    unique_lock<mutex> lck(mtxMonitor);
    while(!(contNipSabenPareja==2)){
        contNipSabenParejaIgual2.wait(lck);
    }
    sillasOc=0;
    asignacionParejas=false;
    contNipSabenPareja=0;


    sillasMenor2.notify_all();
};

//Procedimiento que espera a que todos los alumnos terminen de realizar el examen, una vez acaben, el proceso
//profesor puede terminar
void  monitor::parejasTerProfesor(){
    unique_lock<mutex> lck(mtxMonitor);
    while(!(parejasTer==(N_EST/2))){
        parejasTerIgual30.wait(lck);
    }
};
