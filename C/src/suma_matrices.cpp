#include <iostream>
#include <vector>
#include <valarray>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

struct Datos_BL{
    pair<string,string> COMENTARIO;
    pair<string,string> ALGORITMO;
    pair<string,string> PATHENTRADA;
    pair<string,string> PATHSALIDA;
    pair<string,string> OUTPUT_FILE;
    pair<string,string> POR_SEGURIDAD;
    pair<string,string> VALOR_KM;
    pair<string,string> MAX_EPOCAS_BL;
    pair<string,string> FACTOR_SOBRANTE;
    pair<string,string> POR_ESTACIONES;
    pair<string,string> POR_CANT;
    pair<string,string> MOD_K;
    pair<string,string> AUMENTO_TRAFICO;
    pair<string,string> HIBRIDO;
    pair<string,string> MAX_ITERACIONES;
    pair<string,string> SEMILLA;
    pair<string,string> PASO_VNS;
    pair<string,string> NUM_VECINOS;
    pair<string,string> TAM_POBLACION;
    pair<string,string> TAM_ELITE;
    pair<string,string> PROB_CRUCE;
    pair<string,string> PROB_MUT;
    pair<string,string> TAM_TORNEO;
    pair<string,string> GEN_BUSQUEDALOCAL;
    pair<string,string> BL_FIJA;
    pair<string,string> MAX_EPOCAS_EVO;
};

void InicializaEtiquetas(string linea, ifstream & fichero, Datos_BL & datos){
    
    getline(fichero, linea);

    //Divide la línea en campos utilizando la coma como delimitador
    stringstream ss(linea);
    string cell;

    // Asociamos todos las etiquetas del struct a los campos del archivo
    getline(ss, cell, '*');
    datos.COMENTARIO.first = cell;

    getline(ss, cell, '*');
    datos.ALGORITMO.first = cell;

    getline(ss, cell, '*');
    datos.PATHENTRADA.first = cell;

    getline(ss, cell, '*');
    datos.PATHSALIDA.first = cell;

    getline(ss, cell, '*');
    datos.OUTPUT_FILE.first = cell;

    getline(ss, cell, '*');
    datos.POR_SEGURIDAD.first = cell;

    getline(ss, cell, '*');
    datos.POR_ESTACIONES.first = cell;

    getline(ss, cell, '*');
    datos.POR_CANT.first = cell;

    getline(ss, cell, '*');
    datos.MOD_K.first = cell;

    getline(ss, cell, '*');
    datos.VALOR_KM.first = cell;

    getline(ss, cell, '*');
    datos.MAX_EPOCAS_BL.first = cell;

    getline(ss, cell, '*');
    datos.FACTOR_SOBRANTE.first = cell;

    getline(ss, cell, '*');
    datos.AUMENTO_TRAFICO.first = cell;

    getline(ss, cell, '*');
    datos.HIBRIDO.first = cell;

    getline(ss, cell, '*');
    datos.MAX_ITERACIONES.first = cell;

    getline(ss, cell, '*');
    datos.SEMILLA.first = cell;

    getline(ss, cell, '*');
    datos.PASO_VNS.first = cell;

    getline(ss, cell, '*');
    datos.NUM_VECINOS.first = cell;

    getline(ss, cell, '*');
    datos.TAM_POBLACION.first = cell;

    getline(ss, cell, '*');
    datos.TAM_ELITE.first = cell;

    getline(ss, cell, '*');
    datos.PROB_CRUCE.first = cell;

    getline(ss, cell, '*');
    datos.PROB_MUT.first = cell;

    getline(ss, cell, '*');
    datos.TAM_TORNEO.first = cell;

    getline(ss, cell, '*');
    datos.GEN_BUSQUEDALOCAL.first = cell;

    getline(ss, cell, '*');
    datos.BL_FIJA.first = cell;

    getline(ss, cell, '*');
    datos.MAX_EPOCAS_EVO.first = cell;
}

/*************************************************************************/
/*************************************************************************/
// Inicializa el segundo componente de los pares del struct Datos_BL
// Recibe:
//      linea: Línea a leer donde se encuentran todos los valores de las etiquetas
//      fichero: Fichero de datos
//      datos: Referencia al struct de datos que se va a inicializar
void InicializaValores(string linea, ifstream & fichero, Datos_BL & datos, int & leidas){
    
    while(getline(fichero, linea)){
        //getline(fichero, linea);
        //Divide la línea en campos utilizando la coma como delimitador
        stringstream ss(linea);
        string cell;

        // Asociamos todos los valores del struct a los campos del archivo
        getline(ss, cell, '*');
        datos.COMENTARIO.second = cell;

        getline(ss, cell, '*');
        datos.ALGORITMO.second = cell;

        getline(ss, cell, '*');
        datos.PATHENTRADA.second = cell;

        getline(ss, cell, '*');
        datos.PATHSALIDA.second = cell;

        getline(ss, cell, '*');
        datos.OUTPUT_FILE.second = cell;

        getline(ss, cell, '*');
        datos.POR_SEGURIDAD.second = (cell);

        getline(ss, cell, '*');
        datos.POR_ESTACIONES.second = cell;

        getline(ss, cell, '*');
        datos.POR_CANT.second = cell;

        getline(ss, cell, '*');
        datos.MOD_K.second = cell;

        getline(ss, cell, '*');
        datos.VALOR_KM.second = cell;

        getline(ss, cell, '*');
        datos.MAX_EPOCAS_BL.second = cell;

        getline(ss, cell, '*');
        datos.FACTOR_SOBRANTE.second = cell;

        getline(ss, cell, '*');
        datos.AUMENTO_TRAFICO.second = cell;

        getline(ss, cell, '*');
        datos.HIBRIDO.second = cell;

        getline(ss, cell, '*');
        datos.MAX_ITERACIONES.second = cell;

        getline(ss, cell, '*');
        datos.SEMILLA.second = cell;

        getline(ss, cell, '*');
        datos.PASO_VNS.second = cell;

        getline(ss, cell, '*');
        datos.NUM_VECINOS.second = cell;

        getline(ss, cell, '*');
        datos.TAM_POBLACION.second = cell;

        getline(ss, cell, '*');
        datos.TAM_ELITE.second = cell;

        getline(ss, cell, '*');
        datos.PROB_CRUCE.second = cell;

        getline(ss, cell, '*');
        datos.PROB_MUT.second = cell;

        getline(ss, cell, '*');
        datos.TAM_TORNEO.second = cell;

        getline(ss, cell, '*');
        datos.GEN_BUSQUEDALOCAL.second = cell;

        getline(ss, cell, '*');
        datos.BL_FIJA.second = cell;

        getline(ss, cell, '*');
        datos.MAX_EPOCAS_EVO.second = cell;

        leidas++;
    }
}

string Serializa(Datos_BL & datos){
    return "{'" + datos.COMENTARIO.first + "': " + "'" + datos.COMENTARIO.second + "', " +
            "'" + datos.ALGORITMO.first + "': " + "'" + datos.ALGORITMO.second + "', " +
            "'" + datos.PATHENTRADA.first + "': " + "'" + datos.PATHENTRADA.second + "', " +
            "'" + datos.PATHSALIDA.first + "': " + "'" + datos.PATHSALIDA.second + "', " +
            "'" + datos.OUTPUT_FILE.first + "': " + "'" + datos.OUTPUT_FILE.second + "', " +
            "'" + datos.POR_SEGURIDAD.first + "': " + "'" + datos.POR_SEGURIDAD.second + "', " +
            "'" + datos.POR_ESTACIONES.first + "': " + "'" + datos.POR_ESTACIONES.second + "', " +
            "'" + datos.POR_CANT.first + "': " + "'" + datos.POR_CANT.second + "', " +
            "'" + datos.MOD_K.first + "': " + "'" + datos.MOD_K.second + "', " +
            "'" + datos.VALOR_KM.first + "': " + "'" + datos.VALOR_KM.second + "', " +
            "'" + datos.MAX_EPOCAS_BL.first + "': " + "'" + datos.MAX_EPOCAS_BL.second + "', " +
            "'" + datos.FACTOR_SOBRANTE.first + "': " + "'" + datos.FACTOR_SOBRANTE.second + "', " +
            "'" + datos.AUMENTO_TRAFICO.first + "': " + "'" + datos.AUMENTO_TRAFICO.second + "', " +
            "'" + datos.HIBRIDO.first + "': " + "'" + datos.HIBRIDO.second + "', " +
            "'" + datos.MAX_ITERACIONES.first + "': " + "'" + datos.MAX_ITERACIONES.second + "', " +
            "'" + datos.SEMILLA.first + "': " + "'" + datos.SEMILLA.second + "', " +
            "'" + datos.PASO_VNS.first + "': " + "'" + datos.PASO_VNS.second + "', " +
            "'" + datos.NUM_VECINOS.first + "': " + "'" + datos.NUM_VECINOS.second + "', " +
            "'" + datos.TAM_POBLACION.first + "': " + "'" + datos.TAM_POBLACION.second + "', " +
            "'" + datos.TAM_ELITE.first + "': " + "'" + datos.TAM_ELITE.second + "', " +
            "'" + datos.PROB_CRUCE.first + "': " + "'" + datos.PROB_CRUCE.second + "', " +
            "'" + datos.PROB_MUT.first + "': " + "'" + datos.PROB_MUT.second + "', " +
            "'" + datos.TAM_TORNEO.first + "': " + "'" + datos.TAM_TORNEO.second + "', " +
            "'" + datos.GEN_BUSQUEDALOCAL.first + "': " + "'" + datos.GEN_BUSQUEDALOCAL.second + "', " +
            "'" + datos.BL_FIJA.first + "': " + "'" + datos.BL_FIJA.second + "', " +
            "'" + datos.MAX_EPOCAS_EVO.first + "': " + "'" + datos.MAX_EPOCAS_EVO.second + "'}";           
}

vector<vector<int>> cargamatriz_int(string fichero, bool cabecera){
    ifstream f(fichero);
    vector<vector<int>> contenido;
    string linea;

    if (!f.is_open()) {
        cerr << "Error al abrir el archivo." << std::endl;
        return contenido;
    }

    if (cabecera)
        getline(f, linea); // Se descarta la primera línea

    while(getline(f, linea)){
        istringstream iss(linea);
        int valor;
        vector<int> fila;

        while(iss >> valor){
            fila.push_back(valor);
            if(iss.peek() == ',')
                iss.ignore(1);
        }

        contenido.push_back(fila);
    }
    
    f.close();
    return contenido;   
}

vector<vector<double>> cargamatriz_double(string fichero, bool cabecera){
    ifstream f(fichero);
    vector<vector<double>> contenido;
    string linea;

    if (!f.is_open()) {
        cerr << "Error al abrir el archivo." << std::endl;
        return contenido;
    }

    if (cabecera)
        getline(f, linea); // Se descarta la primera línea

    while(getline(f, linea)){
        istringstream iss(linea);
        double valor;
        vector<double> fila;

        while(iss >> valor){
            fila.push_back(valor);
            if(iss.peek() == ',')
                iss.ignore(1);
        }

        contenido.push_back(fila);
    }
    
    f.close();
    return contenido; 
}

template <typename T>
vector<int> getShape(const vector<vector<T>> & matriz){
    int filas = matriz.size();
    int columnas = (filas > 0) ? matriz[0].size() : 0;
    vector<int> dimensiones;
    
    dimensiones.push_back(filas);
    dimensiones.push_back(columnas);

    return dimensiones;
}

int MaxSumaAcumulada(vector<vector<int>> matriz){
    
    // Realiza la suma acumulativa a lo largo del eje 0
    vector<vector<int>> cumsum = matriz;
    
    for (size_t j = 0; j < matriz[0].size(); ++j) {
        for (size_t i = 1; i < matriz.size(); ++i) {
            cumsum[i][j] += cumsum[i-1][j];
        }
    }

    vector<int> sum(cumsum.size());
    for (size_t i = 0; i < cumsum.size(); ++i) {
        sum[i] = accumulate(cumsum[i].begin(), cumsum[i].end(), 0);
    }

    // Encuentra el valor máximo
    int max_value = *max_element(sum.begin(), sum.end());

    //int *algo;
    //cudaMalloc(&algo, sizeof(int));

    return max_value;
}

int main(){
    Datos_BL datos;
    string linea;
    int leidas = 0;
    string ruta = "../C/BL_Sevilla6_evaluacionesBase.csv";
    ifstream fichero(ruta);

    InicializaEtiquetas(linea, fichero, datos);
    InicializaValores(linea, fichero, datos, leidas);

    for(int i = 0; i < leidas; i++){
        cout << Serializa(datos) << endl;
    }
    return 0;
}





config.setEvaluaciones(config.getEvaluaciones() + 1);
    LLENAS = 0;
    VACIAS = 0;
    float movido, aaaa;
    vector<int> ocupacion(CAPACIDAD.size(), 0);
    int idestaciones  = config.getDeltas()[0].size();

    // Acumuladores de costes
    int coste_kms_extra = 0;
    int coste_kms_tendencia = 0;
    TENDENCIA_ENTRADA = 0;
    TENDENCIA_SALIDA = 0;
    bool mov_virtual, entrada_virt;

    vector<vector<int>> DELTAS = config.getDeltas();   
    vector<vector<int>> CERCA = config.getCerca();
    vector<vector<int>> TENDENCIAS = config.getTendencia();
    vector<vector<float>> CERCAKM = config.getCercaKM();

    for(int linea_delta = 0; linea_delta < DELTAS.size(); linea_delta++){
        for(int est_Origen = 0; est_Origen < idestaciones; est_Origen++){
            float pendiente = DELTAS[linea_delta][est_Origen] * config.getFactorStress();
            int nuevatendencia = TENDENCIAS[linea_delta][est_Origen];
            int it_estacion = 0;
            int idest = CERCA[est_Origen][it_estacion];
            if(pendiente == 0){
                if(ocupacion[est_Origen] == CAPACIDAD[est_Origen] || ocupacion[est_Origen] == 0){
                    mov_virtual = true;
                    pendiente = nuevatendencia;
                }
                else
                    mov_virtual = false;
            }
            else
                mov_virtual = false;

            if(pendiente > (CAPACIDAD[est_Origen] - ocupacion[est_Origen]))
                USUARIOSPERDIDOS += pendiente - (CAPACIDAD[est_Origen] - ocupacion[est_Origen]);
            else if((ocupacion[est_Origen] + pendiente) < 0)
                USUARIOSPERDIDOS -= pendiente + ocupacion[est_Origen];

            while(pendiente != 0){
                if(pendiente > 0){  // Vamos a meter est_Origen
                    if((ocupacion[idest] + pendiente) <= CAPACIDAD[idest]){
                        movido = pendiente;
                        pendiente = 0;
                    }
                    else{   // No hay bastantes huecos
                        movido = CAPACIDAD[idest] - ocupacion[idest];
                        pendiente -= movido;
                    }
                    if(!mov_virtual)
                        ocupacion[idest] += movido;
                    else{
                        entrada_virt = true;
                        if(ocupacion[idest] == CAPACIDAD[idest])
                            LLENAS++;
                    }
                }
                else{   // Vamos a sacar est_Origen
                    if((ocupacion[idest] + pendiente) >= 0){ // Hay suficientes
                        movido = -pendiente;
                        pendiente = 0;
                    }
                    else{   // Faltan bicis
                        movido = ocupacion[idest];
                        pendiente += movido;
                    }
                    if(!mov_virtual)
                        ocupacion[idest] -= movido;
                    else{
                        entrada_virt = false;
                        if(ocupacion[idest] == 0)
                            VACIAS++;
                    }
                }
                aaaa = movido * CERCAKM[est_Origen][it_estacion];
                if(mov_virtual){
                    coste_kms_tendencia += aaaa;
                    if(entrada_virt)
                        TENDENCIA_ENTRADA += aaaa;
                    else
                        TENDENCIA_SALIDA += aaaa;
                }
                else
                    coste_kms_extra += aaaa;

                it_estacion++;

                if(it_estacion == NUM_ESTACIONES){
                    cout << "SOCORRO... QUE NO ME CABE!! " << endl;
                    cout << "La línea delta es " << linea_delta << "-->" << DELTAS[linea_delta][0] << endl;
                    exit(-1);
                    break;
                }
                idest = CERCA[est_Origen][it_estacion];
            }
        }
    }

    // CÁLCULO DE FITNESS
    float bicis_total = (config.getTotalBicis() * (1 + config.getPorSeguridad()) * 1.0);
    SOBRANTE = (CAPACIDAD_TOTAL - bicis_total) / bicis_total;
    FITNESS = ((coste_kms_extra + coste_kms_tendencia) * config.getValorKM() + SOBRANTE * config.getFactorSobrante());
    KM_EXTRA = coste_kms_extra;
    KM_TENDENCIA = coste_kms_tendencia;
    cout << "INDIVIDUO: " << ToString() << endl;  