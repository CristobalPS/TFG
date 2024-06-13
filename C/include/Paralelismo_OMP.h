/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de cabecera asociado a las funciones de paralelismo usando OMP
/***************************************************************************/

#ifndef Z_PARALELISMO2
#define Z_PARALELISMO2
#include "z_io.h"
#include "z_output.h"
#include <omp.h>

/*************************************************************************/
/*************************************************************************/
// Función de evaluación en la GPU (uso de OMP)

void omp_evaluacion(int * capacidades_device, int capacidades_columnas, int capacidades_tam, 
                    float * fitness_device, int fitness_tam, 
                    int * deltas_device, int deltas_filas, int deltas_columnas, int deltas_tam, 
                    int * cercanias_device, int cercanias_columnas, int cercanias_tam, 
                    float * cercaniaskm_device, int cercaniaskm_columnas, int cercaniaskm_tam,
                    int * tendencias_device, int tendencias_columnas, int tendencias_tam, 
                    int num_estaciones, float factor_stress, int porSeguridad, 
                    int totalBicis, float valor_kms, 
                    int  * capacidades_total_device, int capacidades_total_tam, int factor_sobrante,
                    int * usuariosPerdidos_device, int usuariosPerdidos_tam,
                    float * costeKmsExtra_device, int costeKmsExtra_tam,
                    float * costeKmsTendencia_device, int costeKmsTendencia_tam,
                    float * plazasSobrantes_device, int plazasSobrantes_tam, 
                    int * estaciones_llenas_device, int estaciones_llenas_tam,
                    int * estaciones_vacias_device, int estaciones_vacias_tam,
                    int idest_size, int tam_poblacion, int nteams, int nhebras);


/*************************************************************************/
/*************************************************************************/
// Función de evaluación (OMP)
// Recibe:
//      poblacion: vector con los individuos a evaluar
//      config: datos de configuración
//      deltas: puntero al vector de deltas (necesario para OpenMP)
//      df: filas de la matriz de deltas
//      dc: columnas de la matriz de deltas
//      dt: tamaño del vector de deltas
//      cercanias: puntero al vector de cercanias (necesario para OpenMP)
//      cc: columnas de la matriz de cercanias
//      ct: tamaño del vector de cercanias
//      cercanias_km: puntero al vector de cercanias km (necesario para OpenMP)
//      ckmc: columnas de la matriz de cercanias km
//      ckmt: tamaño del vector de cercanias
//      tendencias: puntero al vector de tendencias (necesario para OpenMP)
//      tc: columnas de la matriz de tendencias
//      tt: tamaño del vector de tendencias

void evaluarPoblacion_OMP(vector<Individuo_POO> & Poblacion, Datos_EVAL & config, 
                        int * deltas, int df, int dc, int dt,
                        int * cercanias, int cc, int ct,
                        float * cercanias_km, int ckmc, int ckmt, 
                        int * tendencias, int tc, int tt);


#endif