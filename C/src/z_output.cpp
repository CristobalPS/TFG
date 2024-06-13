/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de definiciones asociado a las funciones necesarias para la salida de datos
/***************************************************************************/

#include "z_output.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

/*************************************************************************/
/*************************************************************************/
// Genera los resultados de la ejecución y los guarda en un fichero  
// Recibe:
//      config: Datos de configuración del algoritmo
//      resultados: Estructura con los resultados de la ejecución

void generarResultados(Datos_EVAL & config, Resultado & resultados, Individuo_POO & base){
    cout << string(70, '*') << endl;
    cout << string(70, '*') << endl;
    cout << "" << endl;
    cout << "\t\tGENERANDO RESULTADOS" << endl;
    cout << "" << endl;
    cout << string(70, '*') << endl;
    cout << string(70, '*') << endl;

    Datos_EVAL cc(config); // No modificamos el original

    if(cc.getOutputFile().empty()){
        cc.setOutputFile("historico.csv");
    }

    // Creamos el directorio si no existe
    string command = "mkdir -p " + cc.getPathSalida();
    if(system(command.c_str()) == -1) {
        cerr << "Error al crear el directorio de salida: " << strerror(errno) << endl;
        cerr << "Saliendo del programa" << endl;
        exit(1);
    }

    // Construye el path completo al archivo de salida
    string mifichero = cc.getPathSalida() + "/" + cc.getOutputFile();

    // Comprobamos si el archivo ya existe
    ifstream test(mifichero);
    bool existe = test.good();

    // Abrimos el archivo CSV
    ofstream archivo(mifichero, ios::app);

    if(!archivo.is_open()){
        cerr << "Error al abrir el archivo de salida: " << mifichero << endl;
        cerr << "Saliendo del programa" << endl;
        exit(1);
    }


    if(!existe){
        // Escribimos los datos del archivo de configuración con los resultados
        // del mejor individuo
        archivo << "TIEMPO_INICIO,TIEMPO_FIN,COMENTARIO,ALGORITMO,ENTRADA,SALIDA,OUTPUT_FILE,POR_SEGURIDAD,VALOR_KM,MAX_EPOCAS_BL,";
        archivo << "FACTOR_SOBRANTE,POR_ESTACIONES,POR_CANT,MOD_K,FACTOR_STRESS,HIBRIDO,MAX_ITERACIONES,SEMILLA,PASO_VNS,NUM_VECINOS,";
        archivo << "TAM_POBLACION,TAM_ELITE,PROB_CRUCE,PROB_MUT,TAM_TORNEO,GEN_BUSQUEDA_LOCAL,BL_FIJA,MAX_EPOCAS_EVO,EVALUACIONES,";
        archivo << "NUM_LLAMADAS_EVALUACION,PATH_RESULTADO,TOTAL_BICIS,LINEA_XLS,HIJOS_MEJOR_PADRE,CAPACIDADES_BASE,CAPACIDADES_IBASE_CAPACIDADES_MEJOR,";
        archivo << "MEJOR_INDIV_CAPACIDAD,MEJOR_INDIV_NUM_ESTACIONES,MEJOR_INDIV_FITNESS,MEJOR_INDIV_KM_EXTRA,MEJOR_INDIV_KM_TENDENCIA,MEJOR_INDIV_SOBRANTE,";
        archivo << "MEJOR_INDIV_VACIAS,MEJOR_INDIV_LLENAS,MEJOR_INDIV_CAPACIDAD_TOTAL,MEJOR_INDIV_USUARIOS_PERDIDOS" << endl;
    }

    archivo << config.getTiempoInicial() << "," << config.getTiempoFinal() << ",";
    archivo << config.getComentario() << "," << config.getAlgoritmo() << ",";
    archivo << config.getPathEntrada() << "," << config.getPathSalida() << ",";
    archivo << config.getOutputFile() << "," << config.getPorSeguridad() << ",";
    archivo << config.getValorKM() << "," << config.getMaxEpocasBL() << ",";
    archivo << config.getFactorSobrante() << "," << config.getPorEstaciones() << ",";
    archivo << config.getPorCantidad() << "," << config.getModK() << ",";
    archivo << config.getFactorStress() << "," << config.getHibrido() << ",";
    archivo << config.getMaxIteraciones() << "," << config.getSemilla() << ",";
    archivo << config.getPasoVNS() << "," << config.getNumVecinos() << ",";
    archivo << config.getTamPoblacion() << "," << config.getTamElite() << ",";
    archivo << config.getProbCruce() << "," << config.getProbMut() << ",";
    archivo << config.getTamTorneo() << "," << config.getGenBusquedaLocal() << ",";
    archivo << config.getBLFija() << "," << config.getMaxEpocasEvo() << ",";
    archivo << config.getEvaluaciones() << "," << config.getNLLamadasEvaluacion() << ",";
    archivo << config.getPathResultado() << "," << config.getTotalBicis() << ",";
    archivo << config.getLineaXLS() << ",";

    string hijos = "[";
    for(int i = 0; i < config.getHijosMejorPadre().size(); i++)
        hijos += to_string(config.getHijosMejorPadre()[i]) + " ";
    hijos += "]";
    archivo << hijos << ",";

    string capacidades_base = "[";
    for(int i = 0; i < config.getCapacidadesBase().size(); i++)
        capacidades_base += to_string(config.getCapacidadesBase()[i]) + " ";
    capacidades_base += "]";
    archivo << capacidades_base << ",";

    string capacidades_IBase = "[";
    for(int i = 0; i < base.getCapacidad().size(); i++)
        capacidades_IBase += to_string(base.getCapacidad()[i]) + " ";
    capacidades_IBase += "]";
    archivo << capacidades_IBase << ",";

    string capacidades_mejor = "[";
    for(int i = 0; i < resultados.m_individuo->getCapacidad().size(); i++)
        capacidades_mejor += to_string(resultados.m_individuo->getCapacidad()[i]) + " ";
    capacidades_mejor += "]";
    archivo << capacidades_mejor << ",";

    archivo << resultados.m_individuo->getNumEstaciones() << ",";
    archivo << resultados.m_individuo->getFitness() << ",";
    archivo << resultados.m_individuo->getKmExtra() << ",";
    archivo << resultados.m_individuo->getKmTendencia() << ",";
    archivo << resultados.m_individuo->getSobrante() << ",";
    archivo << resultados.m_individuo->getVacias() << ",";
    archivo << resultados.m_individuo->getLlenas() << ",";
    archivo << resultados.m_individuo->getCapacidadTotal() << ",";
    archivo << resultados.m_individuo->getUsuariosPerdidos() << endl;

    string path = crear_dir_detalles(cc);
    grabar_todos(resultados.lista_individuos, cc, path);
    //generar_graficas(resultados.lista_individuos, "img", path);
}

/*************************************************************************/
/*************************************************************************/
// Guarda los errores de la ejecución

void guardar_error(Datos_EVAL & datos){
    cerr << "Guardando errores de línea: " << datos.getLineaXLS() << endl;
}

/*************************************************************************/
/*************************************************************************/
// Crea los detalles del directorio
//
// Recibe:
//      config: Archivo de configuración
// Devuelve:
//      Ruta del directorio creado

string crear_dir_detalles(Datos_EVAL & config){
    string cadena = config.getPathEntrada();
    string intermedio = "";

    if (cadena.find("enero") != string::npos)
        intermedio = "enero";
    if (cadena.find("ferbrero") != string::npos)
        intermedio = "ferbrero";
    if (cadena.find("marzo_estres") != string::npos)
        intermedio = "marzo_estres";
    if (cadena.find("marzo_estres_v2") != string::npos)
        intermedio = "marzo_estres_v2";
    if (cadena.find("marzo_reales") != string::npos)
        intermedio = "marzo_reales";
    if (cadena.find("marzo") != string::npos)
        intermedio = "marzo";
    if (cadena.find("abril") != string::npos)
        intermedio = "abril";
    if (cadena.find("mayo") != string::npos)
        intermedio = "mayo";
    if (cadena.find("junio") != string::npos)
        intermedio = "junio";

    stringstream ss;
    ss << intermedio << "_" << config.getAlgoritmo() << "_" << config.getPorEstaciones()
       << "_" << config.getPorCantidad() << "_" << config.getFactorSobrante()
       << "_" << config.getSemilla();
    intermedio = ss.str();

    // Construyendo la ruta del directorio;
    string dir = config.getPathSalida() + "/" + intermedio;

    // Creando el directorio
    string command = "mkdir -p " + dir;
    if(system(command.c_str()) == -1) {
        cerr << "Error al crear el directorio: " << strerror(errno) << endl;
        exit(1);
    }
    return dir;
}

/*************************************************************************/
/*************************************************************************/
// Guarda todos los detalles en un arhivo csv
// Recibe:
//      lista: Estructura con los individuos más relevantes
//      cc: Datos de configuración
//      rutaArchivo: Ruta del archivo de salida

void grabar_todos(vector<Individuo_POO> & lista, Datos_EVAL & cc, string & rutaArchivo){

    string nombreArchivo = to_string(cc.getSemilla()) + "_allbest.csv";
    string rutaCompleta = rutaArchivo + "/" + nombreArchivo;
    
    // Abrimos el archivo CSV
    ofstream archivo(rutaCompleta);

    if(!archivo.is_open()){
        cerr << "Error al abrir el archivo de salida: " << rutaCompleta << endl;
        cerr << "Saliendo del programa" << endl;
        exit(1);
    }

    // Escribimos la cabecera (coma como separador)
    archivo << "MES,FITNESS,CAPACIDAD_TOTAL,KM_EXTRA,KM_TENDENCIA,SOBRANTE,VACIAS,LLENAS,BICIS TOTAL,PERDIDOS,CAPACIDAD,IBASE-CAPACIDADES_MEJOR" << endl;

    // Escribimos los datos
    for(int i = 0; i < lista.size(); i++){
       
        string cadena2 = "[";
        for(int k = 0; k < cc.getResta().size(); k++)
            cadena2 += to_string(cc.getResta()[k]) + " ";
        cadena2 += "]";

        string cadena = "["; 
        for(int k = 0; k < lista[i].getCapacidad().size(); k++)
            cadena += to_string(lista[i].getCapacidad()[k]) + " ";
        cadena += "]";

        float bicis = round(cc.getTotalBicis() * (1 + cc.getPorSeguridad()) * 1);
        string cadena3 = cc.getPathEntrada().substr(50, 7);

        archivo << cadena3 << "," << lista[i].getFitness() << "," << lista[i].getCapacidadTotal();
        archivo << "," << lista[i].getKmExtra() << "," << lista[i].getKmTendencia();
        archivo << "," << lista[i].getSobrante() << "," << lista[i].getVacias();
        archivo << "," << lista[i].getLlenas() << "," << bicis;
        archivo << "," << lista[i].getUsuariosPerdidos() << "," << cadena;
        archivo << "," << cadena2 << endl;
    }

    archivo.close();
}
