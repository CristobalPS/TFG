/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de cabecera asociado a la estructura Datos_EVAL
/***************************************************************************/

#ifndef Z_IO
#define Z_IO

#include "z_output.h"
#include "z_Individuo.h"
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <valarray>
#include <cctype>
#include <algorithm>
#include <numeric>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Datos referentes a BLocalParalela
///////////////////////////////////////////////////////////////////////////
struct Datos_CSV{
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

///////////////////////////////////////////////////////////////////////////
// Métodos asociados al fichero de entrada de BLocalParalela
///////////////////////////////////////////////////////////////////////////

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

void InicializaStruct(string linea, ifstream & fichero, Datos_CSV & datos, bool etiqueda, int & leidas);

/*************************************************************************/
/*************************************************************************/
// Rutina para inicializar los valores de la entrada
//
// Recibe:
//      entrada: Entrada procedente de la línea de comandos
//      CONFIG: Estructura con los valores de configuración
//      BASE: Vector que almacenará als capacidades de las estaciones

class Datos_EVAL; // Declaración anticipada
void lectura(Datos_CSV & entrada, Datos_EVAL & CONFIG, vector<int> & BASE);


/*************************************************************************/
/*************************************************************************/
// Extrae de un fichero CSV todos los campos a utilizar
// Recibe:
//      fichero: FIchero de datos csv
//      Datos_EVAL: Objeto de la clase que se va a inicializar 
//      bb: Vector que almacena las capacidades de las estaciones
//      leidas: Número de líneas con datos
class Datos_EVAL; // Declaración anticipada
vector<Datos_EVAL> extraer_ejecucion(string fichero, Datos_EVAL & Datos_EVAL, vector<int> & bb, int & leidas);

/*************************************************************************/
/*************************************************************************/
// Paso de string a float

float lectura_float(string cadena);

/*************************************************************************/
/*************************************************************************/
// Paso de string a int

int lectura_int(string cadena);

/*************************************************************************/
/*************************************************************************/
// Método ToString del struct

string Serializa(Datos_CSV & datos);


///////////////////////////////////////////////////////////////////////////
// Métodos asociados al fichero de salida de BLocalParalela
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
class Individuo_POO; // Declaración anticipada
class Datos_EVAL{
private:
    string TIEMPOINI;
    string TIEMPOFIN;
    string COMENTARIO;
    string ALGORITMO;
    string PATHENTRADA;
    string PATHSALIDA;
    string OUTPUT_FILE;
    string PATHRESULTADO;
    float POR_SEGURIDAD;
    float MAX_EPOCAS_BL;
    float POR_ESTACIONES;
    float POR_CANTIDAD;
    float MOD_K;
    float VALOR_KM;
    float FACTOR_SOBRANTE;
    float FACTOR_STRESS;
    bool HIBRIDO;
    int MAX_ITERACIONES;
    float SEMILLA;
    float PASO_VNS;
    int NUM_VECINOS;
    int TAM_POBLACION;
    int TAM_ELITE;
    float PROB_CRUCE;
    float PROB_MUT;
    int TAM_TORNEO;
    int GEN_BUSQUEDA_LOCAL;
    int BL_FIJA;
    float MAX_EPOCAS_EVO;
    int EVALUACIONES;
    int N_LLAMADAS_EVALUACION;
    vector<int> HIJOS_MEJOR_PADRE;
    int LINEAXLS;
    
    // Datos de los ficheros
    vector<vector<int>> CERCA;
    vector<vector<float>> CERCA_KM;
    vector<vector<int>> TENDENCIA;
    vector<vector<int>> DELTAS;
    int TOTALBICIS;

    //Resultados
    valarray<int> RESTA;
    vector<int> CAPACIDADES_BASE;
    
public:

    /*************************************************************************/
    /*************************************************************************/
    // Constructor sin argumentos
    //
    // PRE: Ninguna
    // POST: Crea un objeto de la clase Datos_EVAL vacío

    Datos_EVAL() = default;

    /*************************************************************************/
    /*************************************************************************/
    // Constructor de copia
    //
    // Recibe:
    //      otro: Referencia a un objeto de la misma clase del que copiar 
    //      los datos
    //
    // PRE: otro debe haber sido inicializado 

    Datos_EVAL(const Datos_EVAL & otro);

    /*************************************************************************/
    /*************************************************************************/
    // Métodos SET para los atributos de la clase

    void setTiempoInicial(const tm * tm);
    void setTiempoFinal(const tm * tm);
    void setComentario(const string comentario);
    void setAlgoritmo(const string algoritmo);
    void setPathEntrada(const string pathentrada);
    void setPathSalida(const string pathsalida);
    void setOutputFile(const string output_file);
    void setPathResultado(const string path_resultado);
    void setPorSeguridad(const float por_seguridad);
    void setMaxEpocasBL(const float max_epocas_bl);
    void setPorEstaciones(const float por_estaciones);
    void setPorCantidad(const float por_cantidad);
    void setModK(const float mod_k);
    void setValorKM(const float valor_km);
    void setFactorSobrante(const float factor_sobrante);
    void setFactorStress(const float factor_stress);
    void setHibrido(const bool hibrido);
    void setMaxIteraciones(const int max_iteraciones);
    void setSemilla(const float semilla);
    void setPasoVNS(const float paso_vns);
    void setNumVecinos(const int num_vecinos);
    void setTamPoblacion(const int tam_poblacion);
    void setTamElite(const int tam_elite);
    void setProbCruce(const float prob_cruce);
    void setProbMut(const float prob_mut);
    void setTamTorneo(const int tam_torneo);
    void setGenBusquedaLocal(const int gen_busqueda_local);
    void setBLFija(const int bl_fija);
    void setMaxEpocasEvo(const float max_epocas_evo);
    void setEvaluaciones(const int evaluaciones);
    void setNLLamadasEvaluacion(const int n_llamadas_evaluacion);
    void setHijosMejorPadre(const vector<int> hijos_mejor_padre);
    void setCerca(const vector<vector<int>> & cerca);
    void setCercaKM(const vector<vector<float>> & cerca_km);
    void setTendencia(const vector<vector<int>> & tendencia);
    void setDeltas(const vector<vector<int>> & deltas);
    void setTotalBicis(int totalbicis);
    void setResta(const valarray<int> & resta);
    void setCapacidadesBase(const vector<int> & capacidades_base);
    void setLineaXLS(const int linea);

    /*************************************************************************/
    /*************************************************************************/
    // Métodos GET para los atributos de la clase

    string getTiempoInicial() const;
    string getTiempoFinal() const;
    string getComentario() const;
    string getAlgoritmo() const;
    string getPathEntrada() const;
    string getPathSalida() const;
    string getPathResultado() const;
    string getOutputFile() const;
    float getPorSeguridad() const;
    float getMaxEpocasBL() const;
    float getPorEstaciones() const;
    float getPorCantidad() const;
    float getModK() const;
    float getValorKM() const;
    float getFactorSobrante() const;
    float getFactorStress() const;
    bool getHibrido() const;
    int getMaxIteraciones() const;
    float getSemilla() const;
    float getPasoVNS() const;
    int getNumVecinos() const;
    int getTamPoblacion() const;
    int getTamElite() const;
    float getProbCruce() const;
    float getProbMut() const;
    int getTamTorneo() const;
    int getGenBusquedaLocal() const;
    int getBLFija() const;
    float getMaxEpocasEvo() const;
    int getEvaluaciones() const;
    int getNLLamadasEvaluacion() const;
    vector<int> getHijosMejorPadre() const;
    vector<vector<int>> getCerca() const;
    vector<vector<float>> getCercaKM() const;
    vector<vector<int>> getTendencia() const;
    vector<vector<int>> getDeltas() const;
    int getTotalBicis() const;
    valarray<int> getResta() const;
    vector<int> getCapacidadesBase() const;
    int getLineaXLS() const;

    /*************************************************************************/
    /*************************************************************************/
    // Método ToString de la clase

    string ToString();
};


/////////////////////////////////////////////////////////
// Funciones auxiliares
/////////////////////////////////////////////////////////

/*************************************************************************/
/*************************************************************************/
// Devuelve el número de filas y columnas de cualquier matriz
template <typename T>
vector<int> getShape(const vector<vector<T>> & matriz);

/*************************************************************************/
/*************************************************************************/
// Devuelve el máximo entre la suma acumulada de cada fila y de cada columna
int MaxSumaAcumulada(const vector<vector<int>> & matriz);

/*************************************************************************/
/*************************************************************************/
// Funciones de carga de vectores y matrices

vector<int> carga_vector_int(string fichero, bool cabecera = true);

vector<int> carga_vector_int_parejaEstaciones(string fichero, bool cabecera = true);

vector<vector<int>> cargamatriz_int(string fichero, bool cabecera = true);

vector<vector<float>> cargamatriz_float(string fichero, bool cabecera = true);


#endif