#include <iostream>
#include <iomanip>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "ControlParejas.hpp"
using namespace std;



const string nombreFich="datos.txt";
// Pre : <fila > es un índice de fila de <D>
// Post : devuelve el máximo de la fila <fila >
int maxFila (int D[ N_FIL ][ N_COL ] , int fila ) {
    int max=D[fila][0];
    for(int i=1;i<N_COL;i++){
        if(D[fila][i]>max){
            max=D[fila][i];
        }
    }
    return max ;
}
// Pre : <fila > es un índice de fila de <D>
// Post : devuelve la suma de los els . de la fila <fila >
int sumaFila ( int D[ N_FIL ][ N_COL ] , int fila ) {
    int sum=D[fila][0];
    for(int i=1;i<N_COL;i++){
        sum+=D[fila][i];
        
    }
    return sum;
}


//Pre: Le pasamos un nip, un monitor creado e inicializado y el vector sobre el que tienen que realizar los calculos listo para realizar los calculos pertinentes
//Post: si NIP > miPareja a imprimido la suma de la fila otorgada y el máximo encontrado por el miPareja pero, si NIP < miPareja da permiso a su compañero y devuelve el maximo de la fila
void Estudiante (int nip,monitor& monitor,int D[ N_FIL ][ N_COL ]) {
// <await sillaOc < 2
//        silla[sillasOc]=nip;
//        sillasOc++>
//  
    monitor.sentarseEnSilla(nip);
//  <await asignacionParejas
//      miPareja=pareja[nip]
//      contNipSabenPareja++;
//      filaExam=fila;>
//  >
    int miPareja,filaExam;
    monitor.asignacionParejasAlumno(nip,miPareja,filaExam);
    if (nip < miPareja) { 
        int max=maxFila(D,filaExam);
        // <finMaxFil[fila].resultado=max
        // finMaxFil[fila].permiso=true>
        monitor.calculoMax(max,filaExam);
    }
    else {
        int suma=0;
        suma=sumaFila(D,filaExam);

    // <await finMaxFil[fila]permiso.==true
    //      mostrar resultados  
    //      parejasTer++>
        monitor.permisoAlumno(filaExam,nip,miPareja,suma);
    }
}
//Pre: Monitor creado e inicializado
//Post: Ha asignado todas las parejas y comunicado a las distintas parejas la fila correspondiente a tratar
void Profesor (monitor& monitor) {

    for( int i=0; i< N_FIL ; i++) {
    //<await sillasOc==2
    // pareja[silla[0]]=silla[1]
    // pareja[silla[1]]=silla[0]
    // fila=i;
    // asignacionParejas=true>
    monitor.asignacionParejasProfesor(i);
    

    //  <await contNipSabenPareja==2
    //   sillasOc=0;
    //   asignacionParejas=false
    //   contNipSabenPareja=0>
    monitor.reincioParejasProfesor();
    }

//<await parejasTer==30>
monitor.parejasTerProfesor();

}



// ----------------------------------------------------
int main () {
    
    int D[ N_FIL ][ N_COL ]; // para almacenar los datos
    thread P[N_EST];
    monitor monitor;
    //Abrimos el flujo de datos
    
    ifstream f;
    f.open(nombreFich);
    if(f.is_open()){
        int datoLeido;
        int fila=0;
        int col=0;
        while(f >> datoLeido && fila<N_FIL){
            if(col==N_COL){
                col=0;
                fila++;
            }
            D[fila][col]=datoLeido;
            col++;
        }
        f.close();
    }else{
        cerr << "No se ha conseguido abrir correctamente el fichero : "  << nombreFich << endl;
    }
    // Creamos los hilos correspondientes al profesor y estudiantes
    thread c(&Profesor,ref(monitor));
    
    for(int i = 0; i < N_EST; i++){
         P[i]=thread(&Estudiante,i,ref(monitor),D);
    }
    

    //Hacemos los join para que nuestra función Main no avance y acabe antes que nuestros hilos de cálculo
    //debido a que si no hacemos esto, nos encontrariamos en una situación de error
    c.join();
    for(int i = 0; i < N_EST; i++){
        P[i].join();
    }
    cout << " Prueba finalizada \n";
    cout<<"----------------------------------------------------------------------------------------------"<<endl;
    return 0;
}
