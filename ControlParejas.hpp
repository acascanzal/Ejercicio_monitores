#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;

// ----------------------------------------------------
const int N_EST = 60; //# de estudiantes
const int N_FIL = N_EST /2; //# de filas en la matriz
const int N_COL = 1000; //# de columnas
const int N_SILL =2; //Numero de sillas libres
// ----------------------------------------------------

struct vExamen{
    bool permiso=false;
    int resultado=0;
};
class monitor{
    public:
        monitor(); //constructor
        void sentarseEnSilla(int nip);
        void asignacionParejasAlumno(int nip,int& miPareja,int& filaExam);
        void calculoMax(int max,int filaExam);
        void permisoAlumno(int filaExam,int nip,int miPareja,int suma);
        void asignacionParejasProfesor(int i);
        void reincioParejasProfesor();
        void parejasTerProfesor();
        

    private:
        
        
        int fila,sillasOc,contNipSabenPareja,parejasTer;
        int silla[N_SILL];
        bool asignacionParejas;
        vExamen finMaxFil[N_FIL];
        int pareja[N_EST];
        
        mutex mtxMonitor; //FUNDAMENTAL: mutex usarán las funcs

        condition_variable sillasMenor2;
        condition_variable asignacionParejasTrue;
        condition_variable finMaxFilaPermisoTrue;
        condition_variable sillasOcIgual2;
        condition_variable contNipSabenParejaIgual2;
        condition_variable parejasTerIgual30;
};