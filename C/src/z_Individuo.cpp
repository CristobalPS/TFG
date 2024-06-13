/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero con la definición de los métodos asociadas a la clase Individuo_POO
/***************************************************************************/

#include "z_Individuo.h"
#include <cmath>
#include <iostream>

using namespace std;

/*************************************************************************/
/*************************************************************************/
// Constructor sin argumentos (crea un Individuo vacío)

Individuo_POO::Individuo_POO(){
    CAPACIDAD = vector<int>();
    NUM_ESTACIONES = 0;
    FITNESS = 0;

    KM_EXTRA = 0;
    KM_TENDENCIA = 0;
    SOBRANTE = 0;
    
    CAPACIDAD_TOTAL = 0;
    VACIAS = 0;
    LLENAS = 0;
    
    USUARIOSPERDIDOS = 0;
}

/*************************************************************************/
/*************************************************************************/
// Constructor de copia
//
// Recibe:
//      otro: Referencia a un objeto de la misma clase del que copiar 
//      los datos
//
// PRE: otro debe haber sido inicializado y ser distinto al objeto

Individuo_POO::Individuo_POO(const Individuo_POO & otro){
    if(this != &otro)
        CopiarDatos(otro);
}

/*************************************************************************/
/*************************************************************************/
// Sobrecarga del operador de asignación
//
// Recibe
//  otro: Referencia al objeto de la misma clase del que se van a copiar 
// los datos
//
//  PRE: El objeto otro debe estar inicializado y ser distinto al objeto

Individuo_POO & Individuo_POO::operator = (const Individuo_POO & otro){
    if(this != &otro){
        CopiarDatos(otro);
    }

    return (*this);
}

/*************************************************************************/
/*************************************************************************/
// Sobrecarga del operador de igualdad
// Comprueba si dos individuos son iguales (tienen el mismo fitness)
//
// Recibe:
//      otro: Referencia al objeto de la misma clase con el que comparar
//
// PRE: El objeto otro debe estar inicializado

bool Individuo_POO::operator == (const Individuo_POO & otro) const{
    return this->FITNESS == otro.FITNESS;
}

/*************************************************************************/
/*************************************************************************/
// Métodos SET para los atributos de la clase

void Individuo_POO::setCapacidad(const vector<int> & capacidad){
    CAPACIDAD = capacidad;
    NUM_ESTACIONES = capacidad.size();
    CAPACIDAD_TOTAL = sum(capacidad);
    FITNESS = 0;
    KM_EXTRA = 0;
    KM_TENDENCIA = 0;
    SOBRANTE = 0;
    VACIAS = 0;
    LLENAS = 0;
    USUARIOSPERDIDOS = 0;
}

void Individuo_POO::setNumEstaciones(const int num_estaciones){
    NUM_ESTACIONES = num_estaciones;
}

void Individuo_POO::setFitness(const float fitness){
    FITNESS = fitness;
}

void Individuo_POO::setCosteKmExtra(const float km_extra){
    KM_EXTRA = km_extra;
}

void Individuo_POO::setCosteKmsTendencia(const float km_tendencia){
    KM_TENDENCIA = km_tendencia;
}

void Individuo_POO::setSobrante(const float sobrante){
    SOBRANTE = sobrante;
}

void Individuo_POO::setCapacidadTotal(const int capacidad_total){
    CAPACIDAD_TOTAL = capacidad_total;
}

void Individuo_POO::setUsuariosPerdidos(const int usuarios_perdidos){
    USUARIOSPERDIDOS = usuarios_perdidos;
}

void Individuo_POO::setEstacionesLlenas(const int estaciones_llenas){
    LLENAS = estaciones_llenas;
}

void Individuo_POO::setEstacionesVacias(const int estaciones_vacias){
    VACIAS = estaciones_vacias;
}

/*************************************************************************/
/*************************************************************************/
// Métodos GET para los atributos de la clase

vector<int> Individuo_POO::getCapacidad() const{
    return CAPACIDAD;
}

int Individuo_POO::getNumEstaciones() const{
    return NUM_ESTACIONES;
}

int Individuo_POO::getFitness() const{
    return FITNESS;
}

int Individuo_POO::getKmExtra() const{
    return KM_EXTRA;
}

int Individuo_POO::getKmTendencia() const{
    return KM_TENDENCIA;
}

int Individuo_POO::getSobrante() const{
    return SOBRANTE;
}

int Individuo_POO::getCapacidadTotal() const{
    return CAPACIDAD_TOTAL;
}

int Individuo_POO::getUsuariosPerdidos() const{
    return USUARIOSPERDIDOS;
}

int Individuo_POO::getLlenas() const{
    return LLENAS;
}

int Individuo_POO::getVacias() const{
    return VACIAS;
}

/*************************************************************************/
/*************************************************************************/
// Método para serializar un individuo

string Individuo_POO::ToString(){
    string cadena;
    cadena += "FITNESS: " + to_string(FITNESS) + "\n";
    cadena += "KM_TENDENCIA: " + to_string(KM_TENDENCIA) + "\n";
    cadena += "KM_EXTRA: " + to_string(KM_EXTRA) + "\n";
    cadena += "SOBRANTES: " + to_string(SOBRANTE) + "\n";

    return cadena;
}

/*************************************************************************/
/*************************************************************************/
// Generacion de un nuevo individuo a partir del base, con un número de cambios
// relativo al número de estaciones y con una cantidad relativa de bicis movidas.
// Recibe:
//      tope_inferior: tope inferior de la cantidad de bicis a mover
//      config: estructura con los datos de la simulación
// Devuelve
//      Un nuevo individuo mutado

Individuo_POO Individuo_POO::mutacion(int tope_inferior, Datos_EVAL & config){

    int n_modificaciones = round(NUM_ESTACIONES * config.getPorEstaciones());
    if(n_modificaciones == 0) n_modificaciones = 1;

    Individuo_POO individuo;
    individuo.setCapacidad(CAPACIDAD);

    for(int i = 0; i < n_modificaciones; i++){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> randint(0, NUM_ESTACIONES-1);
        int estacion = randint(gen);
        float maxcantidad = round(individuo.CAPACIDAD[estacion] * config.getPorCantidad()+1);

        uniform_int_distribution<int> randint2(-maxcantidad, maxcantidad);
        int cant = randint2(gen);
    
        if(cant != 0){
            if((individuo.CAPACIDAD_TOTAL + cant) < tope_inferior)
                cant = -cant;

            if((individuo.CAPACIDAD[estacion] + cant) < 0)
                individuo.CAPACIDAD[estacion] = 0;
            else
                individuo.CAPACIDAD[estacion] += cant;
        }
    }
    
    individuo.CAPACIDAD_TOTAL = sum(individuo.CAPACIDAD);

    return individuo;
}

/*************************************************************************/
/*************************************************************************/
// Mutación extrema de un individuo
// Recibe:
//      tope_inferior: tope inferior de la cantidad de bicis a mover
//      config: estructura con los datos de la simulación
//      n_modificaciones: número de modificaciones a realizar
// Devuelve
//      Un nuevo individuo mutado

Individuo_POO Individuo_POO::mutacion_extrema(int tope_inferior, Datos_EVAL & config, int n_modificaciones){

    Individuo_POO individuo;
    individuo.setCapacidad(CAPACIDAD);

    for(int i = 0; i < n_modificaciones; i++){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> randint(0, NUM_ESTACIONES-1);
        int estacion = randint(gen);
        float maxcantidad = round(individuo.CAPACIDAD[estacion] * config.getPorCantidad()+1);

        uniform_int_distribution<int> randint2(-maxcantidad, maxcantidad);
        int cant = randint2(gen);

        if(cant != 0){
            if((individuo.CAPACIDAD_TOTAL + cant) < tope_inferior)
                cant = -cant;

            if((individuo.CAPACIDAD[estacion] + cant) < 0)
                individuo.CAPACIDAD[estacion] = 0;
            else
                individuo.CAPACIDAD[estacion] += cant;
        }
    }

    individuo.CAPACIDAD_TOTAL = sum(individuo.CAPACIDAD);
    return individuo;
}

/*************************************************************************/
/*************************************************************************/
// Cruce 5% de dos individuos
// Recibe:
//      uno: primer individuo con el que cruzarse
//      otro: segundo individuo con el que cruzarse
//      cap_min: capacidad mínima de las estaciones

void Individuo_POO::cruceCincoPorCiento(Individuo_POO & uno, Individuo_POO & otro, int cap_min){
    int size = min(uno.CAPACIDAD.size(), otro.CAPACIDAD.size());

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> randint(1, size);
    int cxpoint1 = randint(gen);
    
    uniform_int_distribution<int>randint2(1, round((size-1)*0.05));
    int longitud = randint2(gen);

    vector<int> padre1 = uno.CAPACIDAD;
    vector<int> padre2 = otro.CAPACIDAD;
    Individuo_POO Hijo1, Hijo2;
    Hijo1.setCapacidad(uno.CAPACIDAD);
    Hijo2.setCapacidad(otro.CAPACIDAD);

    for(int x = cxpoint1; x < cxpoint1 + longitud; x++){
        int gen1 = padre1[x % size];
        int gen2 = padre2[x % size];

        if((sum(Hijo1.CAPACIDAD) + (gen2 - gen1)) < cap_min || 
            (sum(Hijo2.CAPACIDAD) + (gen1 - gen2)) < cap_min){
            break;
        }

        Hijo1.CAPACIDAD[x % size] = padre2[x % size];
        Hijo2.CAPACIDAD[x % size] = padre1[x % size];
    }

    uno = Hijo1;
    otro = Hijo2;
}

/*************************************************************************/
/*************************************************************************/
// Evaluación del individuo
// Recibe:
//      config: estructura con los datos de la simulación

void Individuo_POO::evaluacion_individuo(Datos_EVAL & config){

    config.setEvaluaciones(config.getEvaluaciones() + 1);
    LLENAS = 0;
    VACIAS = 0;

    vector<int> ocupacion(CAPACIDAD.size(), 0);

    int idestaciones = config.getDeltas()[0].size();

    /// Acumuladores de costes
    float coste_kms_extra = 0;
    float coste_kms_tendencia = 0;

    bool mov_virtual, entrada_virt;

    float aaaa;
    float movido;
    
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
                if((ocupacion[est_Origen] == CAPACIDAD[est_Origen]) or (ocupacion[est_Origen] == 0)){
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
            
            else if((ocupacion[est_Origen]+pendiente) < 0)
                USUARIOSPERDIDOS -= pendiente + ocupacion[est_Origen];
            
            while(pendiente != 0){   
                if(pendiente > 0){ // Vamos a METER est_Origen
                    if(ocupacion[idest] + pendiente <= CAPACIDAD[idest]){
                        movido = pendiente;
                        pendiente = 0;
                    }
                    else{ // NO HAY BASTANTES HUECOS
                        movido = CAPACIDAD[idest] - ocupacion[idest];
                        pendiente -= movido;
                    }

                    if(!mov_virtual){
                        ocupacion[idest] = ocupacion[idest] + movido;
                    }
                    else{
                        entrada_virt = true;
                        if(ocupacion[idest] == CAPACIDAD[idest])
                            LLENAS += 1;
                    }
                }
                else{ // Vamos a SACAR est_Origen
                    if(ocupacion[idest] + pendiente >= 0){
                        movido = -pendiente;
                        pendiente = 0;
                    }
                    else{ // FALTAN BICIS
                        movido = ocupacion[idest];
                        pendiente += movido;
                    }
                    if(!mov_virtual)
                        ocupacion[idest] -= movido;
                    else{
                        entrada_virt = false;
                        if(ocupacion[idest] == 0)
                            VACIAS += 1;
                    }
                }

                aaaa = movido * CERCAKM[est_Origen][it_estacion];
                if(mov_virtual)
                    coste_kms_tendencia += aaaa;
                else
                    coste_kms_extra += aaaa;

                
                // vamos a la siguiente
                it_estacion++;
                // si no quedan huecos en ninguna estación...
                if(it_estacion == NUM_ESTACIONES){
                    cout << "SOCOROOO... QUE NO ME CABE!!" << endl;
                    cout << "La línea delta es " << linea_delta << "-->" << DELTAS[linea_delta][0];
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
    cout << "INDIVIDUO: " << ToString(); 
}


/*************************************************************************/
/*************************************************************************/
// Copia los datos desde otro objeto de la clase
// PRE: Se ha reservado memoria para los datos

void Individuo_POO::CopiarDatos (const Individuo_POO & otro){
    CAPACIDAD = otro.CAPACIDAD;
    NUM_ESTACIONES = otro.NUM_ESTACIONES;
    FITNESS = otro.FITNESS;
    KM_EXTRA = otro.KM_EXTRA;
    KM_TENDENCIA = otro.KM_TENDENCIA;
    SOBRANTE = otro.SOBRANTE;    
    CAPACIDAD_TOTAL = sum(otro.CAPACIDAD);
    VACIAS = otro.VACIAS;
    LLENAS = otro.LLENAS;
    USUARIOSPERDIDOS = otro.USUARIOSPERDIDOS;
}

/*************************************************************************/
/*************************************************************************/
// Sumatorio de la capacidad de las estaciones

int sum(const vector<int> & capacidad){
    int sumatoria = 0;
    for(int i = 0; i < capacidad.size(); i++)
        sumatoria += capacidad[i];
    
    return sumatoria;
} 

