/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de cabecera asociado a la estructura Individuo_POO
/***************************************************************************/

#ifndef Z_INDIVIDUO
#define Z_INDIVIDUO

#include <vector>
#include <string>
#include <random>
#include "z_io.h"

using namespace std;

//////////////////////////////////////////////////////////////
class Datos_EVAL; // Declaración anticipada
class Individuo_POO{
private:
    vector<int> CAPACIDAD;
    int NUM_ESTACIONES;
    float FITNESS;

    float KM_EXTRA;

    float KM_TENDENCIA;
    float SOBRANTE;

    int CAPACIDAD_TOTAL;
    int LLENAS;
    int VACIAS;
    int USUARIOSPERDIDOS;

public:
    /*************************************************************************/
    /*************************************************************************/
    // Constructor sin argumentos (crea un Individuo vacío)

    Individuo_POO();

    /*************************************************************************/
    /*************************************************************************/
    // Constructor de copia
    //
    // Recibe:
    //      otro: Referencia a un objeto de la misma clase del que copiar 
    //      los datos
    //
    // PRE: otro debe haber sido inicializado 

    Individuo_POO (const Individuo_POO & otro);

    /*************************************************************************/
    /*************************************************************************/
    // Sobrecarga del operador de asignación
    //
    // Recibe
    //  otro: Referencia al objeto de la misma clase del que se van a copiar 
    //  los datos
    //
    //  PRE: El objeto otro debe estar inicializado
  
    Individuo_POO & operator = (const Individuo_POO & otro);

    /*************************************************************************/
    /*************************************************************************/
    // Sobrecarga del operador de igualdad
    // Comprueba si dos individuos son iguales (tienen el mismo fitness)
    //
    // Recibe:
    //      otro: Referencia al objeto de la misma clase con el que comparar
    //
    // PRE: El objeto otro debe estar inicializado

    bool operator == (const Individuo_POO & otro) const;


    /*************************************************************************/
    /*************************************************************************/
    // Métodos SET para los atributos de la clase

    void setCapacidad(const vector<int> & capacidad);

    void setNumEstaciones(const int num_estaciones);

    void setFitness(const float fitness);

    void setCosteKmExtra(const float km_extra);

    void setCosteKmsTendencia(const float coste_kms_tendencia);

    void setSobrante(const float sobrante);

    void setCapacidadTotal(const int capacidad_total);

    void setUsuariosPerdidos(const int usuarios_perdidos);

    void setEstacionesLlenas(const int estaciones_llenas);

    void setEstacionesVacias(const int estaciones_vacias);

    /*************************************************************************/
    /*************************************************************************/
    // Métodos GET para los atributos de la clase

    vector<int> getCapacidad() const;

    int getNumEstaciones() const;

    float getFitness() const;

    float getKmExtra() const;

    float getKmTendencia() const;

    float getSobrante() const;

    int getCapacidadTotal() const;

    int getUsuariosPerdidos() const;

    int getLlenas() const;

    int getVacias() const;

    /*************************************************************************/
    /*************************************************************************/
    // Método para serializar un individuo

    string ToString();

    /*************************************************************************/
    /*************************************************************************/
    // Generacion de un nuevo individuo a partir del base, con un número de cambios
    // relativo al número de estaciones y con una cantidad relativa de bicis movidas.
    // Recibe:
    //      tope_inferior: tope inferior de la cantidad de bicis a mover
    //      config: estructura con los datos de la simulación
    // Devuelve
    //      Un nuevo individuo mutado
    
    Individuo_POO mutacion(int tope_inferior, Datos_EVAL & config);

    /*************************************************************************/
    /*************************************************************************/
    // Mutación extrema de un individuo
    // Recibe:
    //      tope_inferior: tope inferior de la cantidad de bicis a mover
    //      config: estructura con los datos de la simulación
    //      n_modificaciones: número de modificaciones a realizar
    // Devuelve
    //      Un nuevo individuo mutado
   
    Individuo_POO mutacion_extrema(int tope_inferior, Datos_EVAL & config, int n_modificaciones);

    /*************************************************************************/
    /*************************************************************************/
    // Mutación variable de un individuo
    // Recibe:
    //      tope_inferior: tope inferior de la cantidad de bicis a mover
    //      config: estructura con los datos de la simulación
    //      n_modificaciones: número de modificaciones a realizar
    // Devuelve
    //      Un nuevo individuo mutado
    
    Individuo_POO mutacion_variable(int tope_inferior, Datos_EVAL & config, int n_modificaciones);

    /*************************************************************************/
    /*************************************************************************/
    // Mutación conservativa de un individuo
    // Recibe:
    //      intercambios: COMPLETAR
    //      maxcantidad: COMPLETAR (por defecto a 5)
    // Devuelve
    //      Un nuevo individuo mutado

    Individuo_POO mutacion_conservativa(int intercambios, int maxcantidad=5);

    /*************************************************************************/
    /*************************************************************************/
    // Cruce de dos individuos
    // Recibe:
    //      otroIndividuo: individuo con el que cruzarse
    //      cap_min: capacidad mínima de las estaciones
    // Devuelve
    //      Un nuevo individuo cruzado

    Individuo_POO cruce(Individuo_POO & otroIndividuo, int cap_min);

    /*************************************************************************/
    /*************************************************************************/
    // Cruce 5% de dos individuos
    // Recibe:
    //      uno: primer individuo con el que cruzarse
    //      otro: segundo individuo con el que cruzarse
    //      cap_min: capacidad mínima de las estaciones

    void cruceCincoPorCiento(Individuo_POO & uno, Individuo_POO & otro, int cap_min);

    /*************************************************************************/
    /*************************************************************************/
    // Evaluación del individuo
    // Recibe:
    //      config: estructura con los datos de la simulación

    void evaluacion_individuo(Datos_EVAL & config);

private:

    /*************************************************************************/
    /*************************************************************************/
    // Copia los datos desde otro objeto de la clase
    // PRE: Se ha reservado memoria para los datos
    
    void CopiarDatos (const Individuo_POO & otro);
};

/*************************************************************************/
/*************************************************************************/
// Sumatorio de la capacidad de las estaciones

int sum(const vector<int> & capacidad); 
#endif