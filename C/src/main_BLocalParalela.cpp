#include "FuncionesBLocalParalela.h"
#include <iostream>
using namespace std;

int main(int argc, char * argv[]){
    
    if(argc != 3){
        cerr << "Error en el número de argumentos" << endl;
        cerr << "Uso: " << argv[0] << " <fichero_configuracion.csv>" << " modo (CUDA/OMP)";
        cerr << endl;
        exit(-1);
    }

    Datos_EVAL cc;
    vector<int> bb;
    int lineas_leidas = 0;
    vector<Datos_EVAL> EXPERIMENTOS;
    EXPERIMENTOS = extraer_ejecucion(argv[1], cc, bb, lineas_leidas);
    
    for(int i = 0; i < EXPERIMENTOS.size(); i++){
        // cout << "Linea " << lineas_leidas << ": " << cc.ToString() << " - [";
        // for(int i = 0; i < bb.size(); i++){
        //     cout << bb[i] << " ";
        // }
        // cout << "]" << endl;
        try{
            lanzador(EXPERIMENTOS[i], bb, argv[2]);
        }
        catch(const exception& e)
        {
            cout << "Ha habido algún error" << endl;
            cout << e.what() << endl;
            guardar_error(cc);
        }
    }
}