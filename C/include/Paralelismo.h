/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de cabecera asociado a las funciones de paralelismo usando OMP
/***************************************************************************/

#ifndef Z_PARALELISMO
#define Z_PARALELISMO
#include "z_io.h"
#include "z_output.h"
#include <cuda_runtime.h>


/*************************************************************************/
/*************************************************************************/
// Función de evaluación en la GPU (uso de la librería CUDA)

__global__ void cuda_evaluacion(int * capacidades_device, int c_tamf, 
                                int c_tamc, float * fitness_device, int num_estaciones, 
                                int * deltas_device, float factor_stress, int porSeguridad, 
                                int totalBicis, int * capacidades_total_device, float valor_kms, 
                                int * tendencias_device, /*float * tendenciasIn_device, float * tendenciasOut_device, */
                                int * usuariosPerdidos_device, int factor_sobrante,
                                int * cercanias_device, float * cercaKm_device, int ckm_tamc,
                                float * costeKmsExtra_device, float * costeKmsTendencia_device, 
                                float * plazasSobrantes_device, 
                                int * estaciones_llenas_device, int * estaciones_vacias_device, 
                                int d_tamf, int d_tamc, int idest_size, int cerc_tamc, int tend_tamc);
  
/*************************************************************************/
/*************************************************************************/
// Función de evaluación (CUDA)
// Recibe:
//      poblacion: vector con los individuos a evaluar
//      config: datos de configuración
//      deltas_device: matriz con los deltas de las estaciones
//      cercanias_device: matriz con las cercanías de las estaciones
//      cercanias_km_device: matriz con los km de las cercanías de las estaciones
//      tendencias_device: matriz con las tendencias de las estaciones
                  
void evaluarPoblacion_CUDA(vector<Individuo_POO> & Poblacion, Datos_EVAL & config, 
                            int * deltas_device, int * cercanias_device,
                            float * cercanias_km_device, int * tendencias_device,
                            int filas_deltas, int columnas_deltas, int columnas_cercanias_km,
                            int columnas_cercanias, int columnas_tendencias);

#endif