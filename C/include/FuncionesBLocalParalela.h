/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de cabecera asociado a las funciones de pbúsqueda local 
// Funciones tanto para CUDA como para OpenMP
/***************************************************************************/

#ifndef FUNCIONESBLOCALPARALELA
#define FUNCIONESBLOCALPARALELA
#include "Paralelismo.h"
#include "Paralelismo_OMP.h"
#include "z_Individuo.h" 
#include "z_io.h"
#include "z_output.h"
#include <cstdio>
#include <cuda_runtime.h>
#include <stdio.h>

/*************************************************************************/
/*************************************************************************/    
// Ordena una población según el FITNESS
// Recibe:
//      poblacion: Lista de individuos sin ordenar
//      poblacion_ordenada: Lista de individuos ordenados según el fitness

void ordenarPoblacion(vector<Individuo_POO> & poblacion, vector<Individuo_POO> & poblacion_ordenada);

/*************************************************************************/
/*************************************************************************/  
// Búsqueda en el entorno usando la GPU (CUDA y OpenMP)

Individuo_POO busqueda_entorno_gpu(Individuo_POO miindividuo, Datos_EVAL & config,
                                    int * deltas_device, int df, int dc, int dt,
                                    int * cercanias_device, int cc, int ct,
                                    float * cercanias_km_device, int ckmc, int ckmt,
                                    int * tendencias_device, int tc, int tt, string cadena);

/*************************************************************************/
/*************************************************************************/  
// Búsqueda del mejor individuo

pair<Individuo_POO, vector<Individuo_POO>> busqueda_local_mejor(Individuo_POO base,
                                            Datos_EVAL & config, vector<vector<int>> & deltas, 
                                            vector<vector<int>> & cercanias, vector<vector<float>> & cercanias_km,
                                            vector<vector<int>> & tendencias, string cadena);
    
/*************************************************************************/
/*************************************************************************/  
// Función lanzador 

void lanzador(Datos_EVAL & config, vector<int> & base, string cadena);

#endif