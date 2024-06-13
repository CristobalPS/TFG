/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de definiciones asociado a las funciones necesarias para la entrada de datos
/***************************************************************************/

#include "z_io.h" 
#include <iostream>

using namespace std;

/*************************************************************************/
/*************************************************************************/
// Inicializa los pares del struct Datos_CSV
//
// Recibe:
//      linea: Línea a leer donde se encuentran todas las etiquetas
//      fichero: Fichero de datos
//      datos: Referencia al struct de datos que se va a inicializar
//      etiqueda: Booleano que indica si se trata de una etiqueta o un valor
//      leidas: Número de líneas con datos

void InicializaStruct(string linea, ifstream & fichero, Datos_CSV & datos, bool etiqueta, int & leidas){

    if(etiqueta){ //Inicializamos las etiquetas
        getline(fichero, linea);

        //Divide la línea en campos utilizando la coma como delimitador
        stringstream ss(linea);
        string celda;

        getline(ss, celda, ',');
        datos.COMENTARIO.first = celda;

        getline(ss, celda, ',');
        datos.ALGORITMO.first = celda;

        getline(ss, celda, ',');
        datos.PATHENTRADA.first = celda;

        getline(ss, celda, ',');
        datos.PATHSALIDA.first = celda;

        getline(ss, celda, ',');
        datos.OUTPUT_FILE.first = celda;

        getline(ss, celda, ',');
        datos.POR_SEGURIDAD.first = celda;

        getline(ss, celda, ',');
        datos.POR_ESTACIONES.first = celda;

        getline(ss, celda, ',');
        datos.POR_CANT.first = celda;

        getline(ss, celda, ',');
        datos.MOD_K.first = celda;

        getline(ss, celda, ',');
        datos.VALOR_KM.first = celda;

        getline(ss, celda, ',');
        datos.MAX_EPOCAS_BL.first = celda;

        getline(ss, celda, ',');
        datos.FACTOR_SOBRANTE.first = celda;

        getline(ss, celda, ',');
        datos.AUMENTO_TRAFICO.first = celda;

        getline(ss, celda, ',');
        datos.HIBRIDO.first = celda;

        getline(ss, celda, ',');
        datos.MAX_ITERACIONES.first = celda;

        getline(ss, celda, ',');
        datos.SEMILLA.first = celda;

        getline(ss, celda, ',');
        datos.PASO_VNS.first = celda;

        getline(ss, celda, ',');
        datos.NUM_VECINOS.first = celda;

        getline(ss, celda, ',');
        datos.TAM_POBLACION.first = celda;

        getline(ss, celda, ',');
        datos.TAM_ELITE.first = celda;

        getline(ss, celda, ',');
        datos.PROB_CRUCE.first = celda;

        getline(ss, celda, ',');
        datos.PROB_MUT.first = celda;

        getline(ss, celda, ',');
        datos.TAM_TORNEO.first = celda;

        getline(ss, celda, ',');
        datos.GEN_BUSQUEDALOCAL.first = celda;

        getline(ss, celda, ',');
        datos.BL_FIJA.first = celda;

        getline(ss, celda, ',');
        datos.MAX_EPOCAS_EVO.first = celda;

        leidas++;
    }
    else{   // Inicializamos los valores 

        //Divide la línea en campos utilizando la coma como delimitador
        stringstream ss(linea);
        string celda;

        // Asociamos todos los valores del struct a los campos del archivo
        getline(ss, celda, ',');
        datos.COMENTARIO.second = celda;

        getline(ss, celda, ',');
        datos.ALGORITMO.second = celda;

        getline(ss, celda, ',');
        datos.PATHENTRADA.second = celda;

        getline(ss, celda, ',');
        datos.PATHSALIDA.second = celda;

        getline(ss, celda, ',');
        datos.OUTPUT_FILE.second = celda;

        getline(ss, celda, ',');
        datos.POR_SEGURIDAD.second = (celda);

        getline(ss, celda, ',');
        datos.POR_ESTACIONES.second = celda;

        getline(ss, celda, ',');
        datos.POR_CANT.second = celda;

        getline(ss, celda, ',');
        datos.MOD_K.second = celda;

        getline(ss, celda, ',');
        datos.VALOR_KM.second = celda;

        getline(ss, celda, ',');
        datos.MAX_EPOCAS_BL.second = celda;

        getline(ss, celda, ',');
        datos.FACTOR_SOBRANTE.second = celda;

        getline(ss, celda, ',');
        datos.AUMENTO_TRAFICO.second = celda;

        getline(ss, celda, ',');
        datos.HIBRIDO.second = celda;

        getline(ss, celda, ',');
        datos.MAX_ITERACIONES.second = celda;

        getline(ss, celda, ',');
        datos.SEMILLA.second = celda;

        getline(ss, celda, ',');
        datos.PASO_VNS.second = celda;

        getline(ss, celda, ',');
        datos.NUM_VECINOS.second = celda;

        getline(ss, celda, ',');
        datos.TAM_POBLACION.second = celda;

        getline(ss, celda, ',');
        datos.TAM_ELITE.second = celda;

        getline(ss, celda, ',');
        datos.PROB_CRUCE.second = celda;

        getline(ss, celda, ',');
        datos.PROB_MUT.second = celda;

        getline(ss, celda, ',');
        datos.TAM_TORNEO.second = celda;

        getline(ss, celda, ',');
        datos.GEN_BUSQUEDALOCAL.second = celda;

        getline(ss, celda, ',');
        datos.BL_FIJA.second = celda;

        getline(ss, celda, ',');
        datos.MAX_EPOCAS_EVO.second = celda;
    
        leidas++;
    }
}

/*************************************************************************/
/*************************************************************************/
// Rutina para inicializar los valores de la entrada
//
// Recibe:
//      entrada: Entrada procedente de la línea de comandos
//      CONFIG: Estructura con los valores de configuración
//      BASE: Vector que almacenará als capacidades de las estaciones

void lectura(Datos_CSV & entrada, Datos_EVAL & CONFIG, vector<int> & BASE){
    
    // Fecha actual
    time_t now = time(NULL);
    tm * tm = localtime(&now);
    CONFIG.setTiempoInicial(tm);
    CONFIG.setComentario(entrada.COMENTARIO.second);
    CONFIG.setAlgoritmo(entrada.ALGORITMO.second);
    CONFIG.setPathEntrada(entrada.PATHENTRADA.second);
    CONFIG.setPathSalida(entrada.PATHSALIDA.second);
    CONFIG.setOutputFile(entrada.OUTPUT_FILE.second);

    if(entrada.POR_SEGURIDAD.first != "") CONFIG.setPorSeguridad(lectura_float(entrada.POR_SEGURIDAD.second));
    else CONFIG.setPorSeguridad(0);
    
    if(entrada.VALOR_KM.first != "") CONFIG.setValorKM(lectura_float(entrada.VALOR_KM.second));
    else CONFIG.setValorKM(0);

    CONFIG.setMaxEpocasBL(lectura_float(entrada.MAX_EPOCAS_BL.second));
    CONFIG.setFactorSobrante(lectura_float(entrada.FACTOR_SOBRANTE.second));
    CONFIG.setPorEstaciones(lectura_float(entrada.POR_ESTACIONES.second));
    CONFIG.setPorCantidad(lectura_float(entrada.POR_CANT.second));

    if(entrada.MOD_K.first != "") CONFIG.setModK(lectura_float(entrada.MOD_K.second));
    else CONFIG.setModK(10);

    if(entrada.AUMENTO_TRAFICO.first != "") CONFIG.setFactorStress(lectura_float(entrada.AUMENTO_TRAFICO.second));
    else CONFIG.setFactorStress(1);

    if(entrada.HIBRIDO.first != "") CONFIG.setHibrido(lectura_int(entrada.HIBRIDO.second));
    else CONFIG.setHibrido(0);

    CONFIG.setMaxIteraciones(lectura_int(entrada.MAX_ITERACIONES.second));
    CONFIG.setSemilla(lectura_int(entrada.SEMILLA.second));

    if(entrada.PASO_VNS.first != "") CONFIG.setPasoVNS(lectura_float(entrada.PASO_VNS.second));
    else CONFIG.setPasoVNS(0);

    CONFIG.setNumVecinos(lectura_int(entrada.NUM_VECINOS.second));
    CONFIG.setTamPoblacion(lectura_int(entrada.TAM_POBLACION.second));
    CONFIG.setTamElite(lectura_int(entrada.TAM_ELITE.second));
    CONFIG.setProbCruce(lectura_float(entrada.PROB_CRUCE.second));
    CONFIG.setProbMut(lectura_float(entrada.PROB_MUT.second));
    CONFIG.setTamTorneo(lectura_int(entrada.TAM_TORNEO.second));
    CONFIG.setGenBusquedaLocal(lectura_int(entrada.GEN_BUSQUEDALOCAL.second));
    CONFIG.setBLFija(lectura_int(entrada.BL_FIJA.second));
    CONFIG.setMaxEpocasEvo(lectura_float(entrada.MAX_EPOCAS_EVO.second));
    CONFIG.setEvaluaciones(0);
    CONFIG.setNLLamadasEvaluacion(0);
    CONFIG.setPathResultado(CONFIG.getPathSalida());
    vector<int> hijos;
    CONFIG.setHijosMejorPadre(hijos);

    // ====  CARGA DE DATOS DE FICHEROS  ================================
    BASE = carga_vector_int_parejaEstaciones(CONFIG.getPathEntrada()+"capacidades.csv", false);
    CONFIG.setCerca(cargamatriz_int(CONFIG.getPathEntrada()+"cercanas_indices.csv", false));
    CONFIG.setCercaKM(cargamatriz_float(CONFIG.getPathEntrada()+"cercanas_kms.csv", false));
    CONFIG.setTendencia(cargamatriz_int(CONFIG.getPathEntrada()+"tendencia_media.csv", true));
    CONFIG.setDeltas(cargamatriz_int(CONFIG.getPathEntrada()+"deltas.csv", true));
    CONFIG.setTotalBicis(MaxSumaAcumulada(CONFIG.getDeltas()));

    cout << "Tamaños de los datos leidos: " << endl;
    cout << "Individuo Base: " << BASE.size() << endl;
    cout << "Matriz Cerca: {" << getShape(CONFIG.getCerca())[0] << ", " <<getShape(CONFIG.getCerca())[1] << "}" << endl;
    cout << "Matriz Cerca km: {" << getShape(CONFIG.getCercaKM())[0] << ", " <<getShape(CONFIG.getCercaKM())[1] << "}" << endl;
    cout << "Matriz Deltas: {" << getShape(CONFIG.getDeltas())[0] << ", " <<getShape(CONFIG.getDeltas())[1] << "}" << endl;;
    cout << "Matriz Tendencias: {" << getShape(CONFIG.getTendencia())[0] << ", " <<getShape(CONFIG.getTendencia())[1] << "}" << endl;
}   

/*************************************************************************/
/*************************************************************************/
// Extrae de un fichero CSV todos los campos a utilizar
// Recibe:
//      fichero: FIchero de datos csv
//      Datos_EVAL: Objeto de la clase que se va a inicializar 
//      bb: Vector que almacena las capacidades de las estaciones
//      leidas: Número de líneas con datos

vector<Datos_EVAL> extraer_ejecucion(string fichero, Datos_EVAL & datos_eval, vector<int> & bb, int & leidas){

    cout << "Leyendo fichero: " << fichero << endl;
    ifstream fich(fichero);
    Datos_CSV datos;
    string linea;
    vector<Datos_EVAL> resultados;

    if (!fich.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        exit(1);
    }

    // Primera pasada al archivo (lectura de las etiquetas)
    InicializaStruct(linea, fich, datos, true, leidas);

    while(getline(fich, linea)){
        // Segunda pasada al archivo (lectura de los valores)
        // Leemos hasta que no haya más líneas de datos
        InicializaStruct(linea, fich, datos, false, leidas);

        // Tenemos el struct de los datos del csv relleno
        datos.COMENTARIO.second.erase(remove_if(datos.COMENTARIO.second.begin(), datos.COMENTARIO.second.end(), ::isspace), datos.COMENTARIO.second.end());
        string sin_espacios = datos.COMENTARIO.second;

        if(sin_espacios.length() == 0){
            cout << "Linea " << leidas << ": " << Serializa(datos);
            try{
                lectura(datos, datos_eval, bb);
                datos_eval.setLineaXLS(leidas);
            }
            catch(exception & e){
                guardar_error(datos_eval);
                throw e;
            }

            resultados.push_back(datos_eval);
        }
    }

    return resultados;
}

/*************************************************************************/
/*************************************************************************/
// Paso de string a float

float lectura_float(string cadena){
    if(cadena == "") return 0;
    return stof(cadena);   
}

/*************************************************************************/
/*************************************************************************/
// Paso de string a int
int lectura_int(string cadena){
    if(cadena == "") return 0;
    return stoi(cadena);
}

/*************************************************************************/
/*************************************************************************/
// Método ToString del struct

string Serializa(Datos_CSV & datos){
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
            "'" + datos.MAX_EPOCAS_EVO.first + "': " + "'" + datos.MAX_EPOCAS_EVO.second + "'}\n";           
}


/////////////////////////////////////////////////////////////////////////
// MÉTODOS DE LA CLASE Datos_EVAL
/////////////////////////////////////////////////////////////////////////

/*************************************************************************/
/*************************************************************************/
// Constructor de copia
//
// Recibe:
//      otro: Referencia a un objeto de la misma clase del que copiar 
//      los datos
//
// PRE: otro debe haber sido inicializado 

Datos_EVAL::Datos_EVAL(const Datos_EVAL & otro){
    TIEMPOINI = otro.TIEMPOINI;
    TIEMPOFIN = otro.TIEMPOFIN;
    COMENTARIO = otro.COMENTARIO;
    ALGORITMO = otro.ALGORITMO;
    PATHENTRADA = otro.PATHENTRADA;
    PATHSALIDA = otro.PATHSALIDA;
    OUTPUT_FILE = otro.OUTPUT_FILE;
    PATHRESULTADO = otro.PATHRESULTADO;
    POR_SEGURIDAD = otro.POR_SEGURIDAD;
    MAX_EPOCAS_BL = otro.MAX_EPOCAS_BL;
    POR_ESTACIONES = otro.POR_ESTACIONES;
    POR_CANTIDAD = otro.POR_CANTIDAD;
    MOD_K = otro.MOD_K;
    VALOR_KM = otro.VALOR_KM;
    FACTOR_SOBRANTE = otro.FACTOR_SOBRANTE;
    FACTOR_STRESS = otro.FACTOR_STRESS;
    HIBRIDO = otro.HIBRIDO;
    MAX_ITERACIONES = otro.MAX_ITERACIONES;
    SEMILLA = otro.SEMILLA;
    PASO_VNS = otro.PASO_VNS;
    NUM_VECINOS = otro.NUM_VECINOS;
    TAM_POBLACION = otro.TAM_POBLACION;
    TAM_ELITE = otro.TAM_ELITE;
    PROB_CRUCE = otro.PROB_CRUCE;
    PROB_MUT = otro.PROB_MUT;
    TAM_TORNEO = otro.TAM_TORNEO;
    GEN_BUSQUEDA_LOCAL = otro.GEN_BUSQUEDA_LOCAL;
    BL_FIJA = otro.BL_FIJA;
    MAX_EPOCAS_EVO = otro.MAX_EPOCAS_EVO;
    EVALUACIONES = otro.EVALUACIONES;
    N_LLAMADAS_EVALUACION = otro.N_LLAMADAS_EVALUACION;
    HIJOS_MEJOR_PADRE = otro.HIJOS_MEJOR_PADRE;
    LINEAXLS = otro.LINEAXLS;

    CERCA = otro.CERCA;
    CERCA_KM = otro.CERCA_KM;
    TENDENCIA = otro.TENDENCIA;
    DELTAS = otro.DELTAS;
    TOTALBICIS = otro.TOTALBICIS;
    RESTA = otro.RESTA;
    CAPACIDADES_BASE = otro.CAPACIDADES_BASE;;
}

/*************************************************************************/
/*************************************************************************/
// Métodos SET para los atributos de la clase

void Datos_EVAL::setTiempoInicial(const tm * tm){
    string tiempo = to_string(tm->tm_hour) + ":" + to_string(tm->tm_min) + ":" + to_string(tm->tm_sec);
    TIEMPOINI = tiempo;
}

void Datos_EVAL::setTiempoFinal(const tm * tm){
    string tiempo = to_string(tm->tm_hour) + ":" + to_string(tm->tm_min) + ":" + to_string(tm->tm_sec);
    TIEMPOFIN = tiempo;
}

void Datos_EVAL::setComentario(const string comentario){
    COMENTARIO = comentario;
}

void Datos_EVAL::setAlgoritmo(const string algoritmo){
    ALGORITMO = algoritmo;
}

void Datos_EVAL::setPathEntrada(const string pathentrada){
    PATHENTRADA = pathentrada;
}

void Datos_EVAL::setPathSalida(const string pathsalida){
    PATHSALIDA = pathsalida;
}
void Datos_EVAL::setOutputFile(const string output_file){
    OUTPUT_FILE = output_file;
}
void Datos_EVAL::setPathResultado(const string path_resultado){
    PATHRESULTADO = path_resultado;
}
void Datos_EVAL::setPorSeguridad(const float por_seguridad){
    POR_SEGURIDAD = por_seguridad;
}

void Datos_EVAL::setMaxEpocasBL(const float max_epocas_bl){
    MAX_EPOCAS_BL = max_epocas_bl;
}
void Datos_EVAL::setPorEstaciones(const float por_estaciones){
    POR_ESTACIONES = por_estaciones;
}
void Datos_EVAL::setPorCantidad(const float por_cantidad){
    POR_CANTIDAD = por_cantidad;
}

void Datos_EVAL::setModK(const float mod_k){
    MOD_K = mod_k;
}
void Datos_EVAL::setValorKM(const float valor_km){
    VALOR_KM = valor_km;
}

void Datos_EVAL::setFactorSobrante(const float factor_sobrante){
    FACTOR_SOBRANTE = factor_sobrante;
}

void Datos_EVAL::setFactorStress(const float factor_stress){
    FACTOR_STRESS = factor_stress;
}

void Datos_EVAL::setHibrido(const bool hibrido){
    HIBRIDO = hibrido;
}

void Datos_EVAL::setMaxIteraciones(const int max_iteraciones){
    MAX_ITERACIONES = max_iteraciones;
}

void Datos_EVAL::setSemilla(const float semilla){
    SEMILLA = semilla;
}

void Datos_EVAL::setPasoVNS(const float paso_vns){
    PASO_VNS = paso_vns;
}

void Datos_EVAL::setNumVecinos(const int num_vecinos){
    NUM_VECINOS = num_vecinos;
}

void Datos_EVAL::setTamPoblacion(const int tam_poblacion){
    TAM_POBLACION = tam_poblacion;
}

void Datos_EVAL::setTamElite(const int tam_elite){
    TAM_ELITE = tam_elite;
}

void Datos_EVAL::setProbCruce(const float prob_cruce){
    PROB_CRUCE = prob_cruce;
}

void Datos_EVAL::setProbMut(const float prob_mut){
    PROB_MUT = prob_mut;
}

void Datos_EVAL::setTamTorneo(const int tam_torneo){
    TAM_TORNEO = tam_torneo;
}

void Datos_EVAL::setGenBusquedaLocal(const int gen_busqueda_local){
    GEN_BUSQUEDA_LOCAL = gen_busqueda_local;
}

void Datos_EVAL::setBLFija(const int bl_fija){
    BL_FIJA = bl_fija;
}

void Datos_EVAL::setMaxEpocasEvo(const float max_epocas_evo){
    MAX_EPOCAS_EVO = max_epocas_evo;
}

void Datos_EVAL::setEvaluaciones(const int evaluaciones){
    EVALUACIONES = evaluaciones;
}

void Datos_EVAL::setNLLamadasEvaluacion(const int n_llamadas_evaluacion){
    N_LLAMADAS_EVALUACION = n_llamadas_evaluacion;
}

void Datos_EVAL::setHijosMejorPadre(const vector<int> hijos_mejor_padre){
    HIJOS_MEJOR_PADRE = hijos_mejor_padre;
}

void Datos_EVAL::setCerca(const vector<vector<int>> & cerca){
    CERCA = cerca;
}

void Datos_EVAL::setCercaKM(const vector<vector<float>> & cerca_km){
    CERCA_KM = cerca_km;
}

void Datos_EVAL::setTendencia(const vector<vector<int>> & tendencia){
    TENDENCIA = tendencia;
}

void Datos_EVAL::setDeltas(const vector<vector<int>> & deltas){
    DELTAS = deltas;
}

void Datos_EVAL::setTotalBicis(int totalbicis){
    TOTALBICIS = totalbicis;
}

void Datos_EVAL::setResta(const valarray<int> & resta){
    RESTA = resta;
}

void Datos_EVAL::setCapacidadesBase(const vector<int> & capacidades_base){
    CAPACIDADES_BASE = capacidades_base;
}

void Datos_EVAL::setLineaXLS(const int linea){
    LINEAXLS = linea;
}

/*************************************************************************/
/*************************************************************************/
// Métodos GET para los atributos de la clase

string Datos_EVAL::getTiempoInicial() const{
    return TIEMPOINI;
}

string Datos_EVAL::getTiempoFinal() const{
    return TIEMPOFIN;
}

string Datos_EVAL::getComentario() const{
    return COMENTARIO;
}

string Datos_EVAL::getAlgoritmo() const{
    return ALGORITMO;
}

string Datos_EVAL::getPathEntrada() const{
    return PATHENTRADA;
}

string Datos_EVAL::getPathSalida() const{
    return PATHSALIDA;
}

string Datos_EVAL::getPathResultado() const{
    return PATHRESULTADO;
}

string Datos_EVAL::getOutputFile() const{
    return OUTPUT_FILE;
}

float Datos_EVAL::getPorSeguridad() const{
    return POR_SEGURIDAD;
}

float Datos_EVAL::getMaxEpocasBL() const{
    return MAX_EPOCAS_BL;
}

float Datos_EVAL::getPorEstaciones() const{
    return POR_ESTACIONES;
}

float Datos_EVAL::getPorCantidad() const{
    return POR_CANTIDAD;
}

float Datos_EVAL::getModK() const{
    return MOD_K;
}

float Datos_EVAL::getValorKM() const{
    return VALOR_KM;
}

float Datos_EVAL::getFactorSobrante() const{
    return FACTOR_SOBRANTE;
}

float Datos_EVAL::getFactorStress() const{
    return FACTOR_STRESS;
}

bool Datos_EVAL::getHibrido() const{
    return HIBRIDO;
}

int Datos_EVAL::getMaxIteraciones() const{
    return MAX_ITERACIONES;
}

float Datos_EVAL::getSemilla() const{
    return SEMILLA;
}

float Datos_EVAL::getPasoVNS() const{
    return PASO_VNS;
}

int Datos_EVAL::getNumVecinos() const{
    return NUM_VECINOS;
}

int Datos_EVAL::getTamPoblacion() const{
    return TAM_POBLACION;
}

int Datos_EVAL::getTamElite() const{
    return TAM_ELITE;
}

float Datos_EVAL::getProbCruce() const{
    return PROB_CRUCE;
}

float Datos_EVAL::getProbMut() const{
    return PROB_MUT;
}

int Datos_EVAL::getTamTorneo() const{
    return TAM_TORNEO;
}

int Datos_EVAL::getGenBusquedaLocal() const{
    return GEN_BUSQUEDA_LOCAL;
}

int Datos_EVAL::getBLFija() const{
    return BL_FIJA;
}

float Datos_EVAL::getMaxEpocasEvo() const{
    return MAX_EPOCAS_EVO;
}

int Datos_EVAL::getEvaluaciones() const{
    return EVALUACIONES;
}

int Datos_EVAL::getNLLamadasEvaluacion() const{
    return N_LLAMADAS_EVALUACION;
}

vector<int> Datos_EVAL::getHijosMejorPadre() const{
    return HIJOS_MEJOR_PADRE;
}

vector<vector<int>> Datos_EVAL::getCerca() const{
    return CERCA;
}

vector<vector<float>> Datos_EVAL::getCercaKM() const{
    return CERCA_KM;
}

vector<vector<int>> Datos_EVAL::getTendencia() const{
    return TENDENCIA;
}

vector<vector<int>> Datos_EVAL::getDeltas() const{
    return DELTAS;
}

int Datos_EVAL::getTotalBicis() const{
    return TOTALBICIS;
}

valarray<int> Datos_EVAL::getResta() const{
    return RESTA;
}

vector<int> Datos_EVAL::getCapacidadesBase() const{
    return CAPACIDADES_BASE;
}

int Datos_EVAL::getLineaXLS() const{
    return LINEAXLS;
}


/*************************************************************************/
/*************************************************************************/
// Método ToString de la clase

string Datos_EVAL::ToString(){
    string cad = "{'TIEMPO': '" + TIEMPOINI + "', 'COMENTARIO': '" + COMENTARIO + "', 'ALGORITMO': '" + ALGORITMO + 
    "', 'PATHENTRADA': '" + PATHENTRADA + "', 'PATHSALIDA': '" + PATHSALIDA + "', 'OUTPUT_FILE': '" + OUTPUT_FILE + 
    "', 'POR_SEGURIDAD': " + to_string(POR_SEGURIDAD) + ", 'VALOR_KM': " + to_string(VALOR_KM) + ", 'MAX_EPOCAS_BL': " + 
    to_string(MAX_EPOCAS_BL) + ", 'FACTOR_SOBRANTE': " + to_string(FACTOR_SOBRANTE) + ", 'POR_ESTACIONES': " + 
    to_string(POR_ESTACIONES) + ", 'POR_CANT': " + to_string(POR_CANTIDAD) + + ", 'MOD_K': " + to_string(MOD_K) +
    ", 'FACTOR_STRESS': " + to_string(FACTOR_STRESS) + ", 'HIBRIDO': " + to_string(HIBRIDO) + ", 'MAX_ITERACIONES': " + 
    to_string(MAX_ITERACIONES) + ", 'SEMILLA': " + to_string(SEMILLA) + ", 'PASO_VNS': " + to_string(PASO_VNS) + ", 'NUM_VECINOS': " + 
    to_string(NUM_VECINOS) + ", 'TAM_POBLACION': " + to_string(TAM_POBLACION) + ", 'TAM_ELITE': " + to_string(TAM_ELITE) + 
    ", 'PROB_CRUCE': " + to_string(PROB_CRUCE) + ", 'PROB_MUT': " + to_string(PROB_MUT) + 
    ", 'TAM_TORNEO': " + to_string(TAM_TORNEO) + ", 'GEN_BUSQUEDA_LOCAL': " + to_string(GEN_BUSQUEDA_LOCAL) +
    ", 'BL_FIJA': " + to_string(BL_FIJA) + ", 'MAX_EPOCAS_EVO': " + to_string(MAX_EPOCAS_EVO) +
    ", 'EVALUACIONES': " + to_string(EVALUACIONES) + ", 'N_LLAMADAS_EVALUACION': " + to_string(N_LLAMADAS_EVALUACION) +
    ", 'PATH_RESULTADO': " + PATHRESULTADO + ", 'HIJOS_MEJOR_PADRE': [" + to_string(HIJOS_MEJOR_PADRE.size()) + 
    "], 'CERCA: array(";

    for(int i = 0; i < CERCA.size(); i++){
        for(int j = 0; j < CERCA[i].size(); j++)
            cad += to_string(CERCA[i][j]) + ", ";
    }

    cad += "), 'CERCA_KM': array(";

    for(int i = 0; i < CERCA_KM.size(); i++){
        for(int j = 0; j < CERCA_KM[i].size(); j++)
            cad += to_string(CERCA_KM[i][j]) + ", ";
    }

    cad += "), 'TENDENCIA': array(";

    for(int i = 0; i < TENDENCIA.size(); i++){
        for(int j = 0; j < TENDENCIA[i].size(); j++)
            cad += to_string(TENDENCIA[i][j]) + ", ";
    }

    cad += "), 'DELTAS': array(";

    for(int i = 0; i < DELTAS.size(); i++){
        for(int j = 0; j < DELTAS[i].size(); j++)
            cad += to_string(DELTAS[i][j]) + ", ";
    }

    cad += "), 'TOTALBICIS': " + to_string(TOTALBICIS) + ", 'LINEAXLS': " + to_string(LINEAXLS) + "}";

    return cad;
}

/////////////////////////////////////////////////////////
// Funciones auxiliares
/////////////////////////////////////////////////////////

/*************************************************************************/
/*************************************************************************/
// Devuelve el número de filas y columnas de cualquier matriz

template <typename T>
vector<int> getShape(const vector<vector<T>> & matriz){
    int filas = matriz.size();
    int columnas = (filas > 0) ? matriz[0].size() : 0;
    vector<int> dimensiones;
    
    dimensiones.push_back(filas);
    dimensiones.push_back(columnas);

    return dimensiones;
}

/*************************************************************************/
/*************************************************************************/
// Devuelve el máximo entre la suma acumulada de cada fila y de cada columna

int MaxSumaAcumulada(const vector<vector<int>> & matriz){
    
    // Realiza la suma acumulativa a lo largo del eje 0
    vector<vector<int>> cumsum = matriz;
    
    for (size_t i = 1; i < matriz.size(); i++) {
        for (size_t j = 0; j < matriz[i].size(); j++) {
            cumsum[i][j] += cumsum[i-1][j];
        }
    }

    // Realiza la suma a lo largo del eje 1
    vector<int> sum(cumsum.size(), 0);
    for (size_t i = 0; i < cumsum.size(); ++i){
        for(size_t j = 0; j < cumsum[i].size(); j++)
            sum[i] += cumsum[i][j];
    }

    // Encuentra el valor máximo
    int max_value = *max_element(sum.begin(), sum.end());

    return max_value;
}

/*************************************************************************/
/*************************************************************************/
// Funciones de carga de vectores y matrices

vector<int> carga_vector_int(string fichero, bool cabecera){
    ifstream f(fichero);
    vector<int> contenido;
    
    string linea;
    
    if (!f.is_open()) {
        cerr << "Error al abrir el archivo." << std::endl;
        return contenido;
    }

    if(cabecera)
        getline(f, linea); // Se descarta la primera línea
    
    while(getline(f, linea)){
        istringstream iss(linea);
        int valor;
        
        iss >> valor; // Extrae el primer entero de la línea
        contenido.push_back(valor);
    }

    f.close();

    return contenido;
}

vector<int> carga_vector_int_parejaEstaciones(string fichero, bool cabecera){
    ifstream f(fichero);
    vector<int> contenido;
    string linea;
    
    if (!f.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return contenido;
    }

    if (cabecera)
        getline(f, linea); // Se descarta la primera línea

    while(getline(f, linea)){
        stringstream iss(linea);
        
        // Ignoramos el primer valor
        string valor1;
        getline(iss, valor1, ',');

        // Nos quedamos con el segundo valor
        string valor2;
        getline(iss, valor2, ',');
        
        int v2 = stoi(valor2);
        contenido.push_back(v2);
    }

    f.close();

    return contenido;
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
        stringstream iss(linea);
        vector<int> fila;
        string valor;

        while(getline(iss, valor, ','))
            fila.push_back(stoi(valor));
        
        contenido.push_back(fila);
    }
    
    f.close();
    return contenido;   
}

vector<vector<float>> cargamatriz_float(string fichero, bool cabecera){
    ifstream f(fichero);
    vector<vector<float>> contenido;
    string linea;

    if (!f.is_open()) {
        cerr << "Error al abrir el archivo." << std::endl;
        return contenido;
    }

    if (cabecera)
        getline(f, linea); // Se descarta la primera línea

    while(getline(f, linea)){
        stringstream iss(linea);
        vector<float> fila;
        string valor;

        while(getline(iss, valor, ',')){
            fila.push_back(stof(valor));
            // if(iss.peek() == ',')
            //     iss.ignore(1);
        }
        contenido.push_back(fila);
    }

    f.close();
    return contenido; 
}