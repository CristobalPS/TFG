/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Implementación de las funciones de paralelismo CUDA
/***************************************************************************/
#include "Paralelismo.h"
#include <iostream>
#include <cuda_runtime.h>

using namespace std;

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
    int d_tamf, int d_tamc, int idest_size, int cerc_tamc, int tend_tamc){
    
    // primero hay que asignar a cada hebra qué individuo evalua
    // segundo comprobar que todos los datos necesarios están disponibles
    // tercero asignar fitness calculado por cada hebra al vector  de resultados
    
    int i = threadIdx.x + (blockIdx.x * blockDim.x);
    if(i < c_tamf){
        bool entrada_virt = false;
        bool mov_virtual;
        int contadorKmExtra = 0;
        int llenas = 0;
        int vacias = 0;
        float movido;

        const int n_estaciones = 259;
        int ocupacion[n_estaciones];
        float costeKmsExtra;

        for(int cont = 0; cont < n_estaciones; cont++)
            ocupacion[cont] = 0;
        
        for(int linea_delta = 0; linea_delta < d_tamf; linea_delta++){
            for(int est_Origen = 0; est_Origen < idest_size; est_Origen++){
                // if(i == 0){
                //     printf("iteracion del bucle grande: %d, iteracion del bucle pequeño: %d\n", linea_delta, est_Origen);
                // }
                float pendiente = deltas_device[linea_delta * d_tamc + est_Origen] * factor_stress;
                int nuevatendencia = tendencias_device[linea_delta * tend_tamc + est_Origen];
                int it_estacion = 0;
                int idest = cercanias_device[est_Origen * cerc_tamc];

                if(pendiente == 0){
                    if((ocupacion[est_Origen] == capacidades_device[i * c_tamc + est_Origen] || ocupacion[est_Origen] == 0)){
                        mov_virtual = true;
                        pendiente = nuevatendencia;
                    }
                    else
                        mov_virtual = false;
                }
                else
                    mov_virtual = false;

                if(pendiente > (capacidades_device[i * c_tamc + est_Origen] - ocupacion[est_Origen]))
                    usuariosPerdidos_device[i] += pendiente - (capacidades_device[i * c_tamc + est_Origen] - ocupacion[est_Origen]);
                else if((ocupacion[est_Origen] + pendiente) < 0)
                    usuariosPerdidos_device[i] -= pendiente + ocupacion[est_Origen];

                while(pendiente != 0){
                    if(pendiente > 0){ // Vamos a meter est_Origen
                        if((ocupacion[idest] + pendiente) <= capacidades_device[i * c_tamc + idest]){ // Hay huecos suficientes
                            movido = pendiente;
                            pendiente = 0;
                        }
                        else{ // No hay bastantes huecos
                            movido = capacidades_device[i * c_tamc + idest] - ocupacion[idest];
                            pendiente -= movido;
                        }
                        if(!mov_virtual)
                            ocupacion[idest] = ocupacion[idest] + movido;
                        else{
                            entrada_virt = true;
                            if(ocupacion[idest] == capacidades_device[i * c_tamc + idest])
                                llenas++;
                        }
                    }
                    else{   // Vamos a sacar est_Origen
                        if((ocupacion[idest] + pendiente) >= 0){ // Hay suficientes
                            movido = -pendiente;
                            pendiente = 0;
                        }
                        else{ // Faltan bicis
                            movido = ocupacion[idest];
                            pendiente += movido;
                        }
                        if(!mov_virtual)
                            ocupacion[idest] -= movido;
                        else{
                            entrada_virt = false;
                            if(ocupacion[idest] == 0)
                                vacias++;
                        }
                    }

                    costeKmsExtra = movido * cercaKm_device[est_Origen * ckm_tamc + it_estacion];

                    if(mov_virtual){
                        costeKmsTendencia_device[i] += costeKmsExtra;
                    }
                    else{
                        costeKmsExtra_device[i] += costeKmsExtra;
                        contadorKmExtra++;
                    }
                    
                    // vamos a la siguiente
                    it_estacion++;

                    // si no quedan huecos en ninguna estación
                    if(it_estacion == num_estaciones)
                        break;

                    idest = cercanias_device[est_Origen * cerc_tamc + it_estacion];
            
                }
            }
        }
        float bicis_total = (totalBicis * (1 + porSeguridad) * 1.0);
        plazasSobrantes_device[i] = (capacidades_total_device[i] - bicis_total) / bicis_total;
        fitness_device[i] = ((costeKmsExtra_device[i] + costeKmsTendencia_device[i]) * valor_kms + plazasSobrantes_device[i] * factor_sobrante);
        estaciones_llenas_device[i] = llenas;
        estaciones_vacias_device[i] = vacias;
    }
}

/*************************************************************************/
/*************************************************************************/
// Función de evaluación 
// Recibe:
//      Poblacion: vector con los individuos a evaluar
//      config: datos de configuración
//      deltas_device: matriz con los deltas de las estaciones
//      cercanias_device: matriz con las cercanías de las estaciones
//      cercanias_km_device: matriz con los km de las cercanías de las estaciones
//      tendencias_device: matriz con las tendencias de las estaciones

void evaluarPoblacion_CUDA(vector<Individuo_POO> & Poblacion, Datos_EVAL & config, 
                            int * deltas_device, int * cercanias_device,
                            float * cercanias_km_device, int * tendencias_device,
                            int filas_deltas, int columnas_deltas, int columnas_cercanias_km,
                            int columnas_cercanias, int columnas_tendencias){
    
    vector<float> fitness(Poblacion.size(), 0);
    vector<int> usuariosPerdidos(Poblacion.size(), 0);
    vector<float> costeKmsExtra(Poblacion.size(), 0);
    vector<float> costeKmsTendencia(Poblacion.size(), 0);
    vector<float> plazasSobrantes(Poblacion.size(), 0);
    vector<int> estaciones_llenas(Poblacion.size(), 0);
    vector<int> estaciones_vacias(Poblacion.size(), 0);
    
    try{
        vector<vector<int>> capacidades;
        vector<int> capacidades_total;

        for(int i = 0; i < Poblacion.size(); i++){
            capacidades.push_back(Poblacion[i].getCapacidad());
            capacidades_total.push_back(Poblacion[i].getCapacidadTotal());
            usuariosPerdidos.push_back(Poblacion[i].getUsuariosPerdidos());
        }

        //vector<float> tendenciasIn(Poblacion.size(), 0);
        //vector<float> tendenciasOut(Poblacion.size(), 0);

        // Paso de variables a la GPU
        // CAPACIDADES (pasamos de matriz a vector)
        int filas_capacidades = capacidades.size();
        int columnas_capacidades = capacidades[0].size();
        vector<int> capacidades_vector;

        for(const auto & i : capacidades)
            capacidades_vector.insert(capacidades_vector.end(), i.begin(), i.end());

        int * capacidades_device; // Vector en la GPU
        int capacidades_tam = capacidades_vector.size() * sizeof(int);
        cudaMalloc(&capacidades_device, capacidades_tam);
        cudaMemcpy(capacidades_device, capacidades_vector.data(), capacidades_tam, cudaMemcpyHostToDevice);

        // FITNESS
        float * fitness_device;
        float fitness_tam = fitness.size() * sizeof(float);
        cudaMalloc(&fitness_device, fitness_tam);
        cudaMemcpy(fitness_device, fitness.data(), fitness_tam, cudaMemcpyHostToDevice);

        // CAPACIDADES TOTALES
        int * capacidades_total_device;
        int capacidades_total_tam = capacidades_total.size() * sizeof(int);
        cudaMalloc(&capacidades_total_device, capacidades_total_tam);
        cudaMemcpy(capacidades_total_device, capacidades_total.data(), capacidades_total_tam, cudaMemcpyHostToDevice);

        // NUM ESTACIONES
        int num_estaciones_device = capacidades[0].size();

        // FACTOR STRESS
        float factor_stress_device = config.getFactorStress();

        // POR SEGURIDAD
        int por_seguridad_device = static_cast<int>(config.getPorSeguridad());

        // TOTAL BICIS
        int totalbicis_device = config.getTotalBicis();

        // VALOR KMS
        float valor_kms_device = config.getValorKM();

        // FACTOR SOBRANTE
        int factor_sobrante_device = static_cast<int>(config.getFactorSobrante());

        /*
        // TENDENCIAS IN
        float * tendenciasIn_device;
        int tendenciasIn_tam = tendenciasIn.size() * sizeof(float);
        cudaMalloc(&tendenciasIn_device, tendenciasIn_tam);
        cudaMemcpy(tendenciasIn_device, tendenciasIn.data(), tendenciasIn_tam, cudaMemcpyHostToDevice);

        // TENDENCIAS OUT
        float * tendenciasOut_device;
        int tendenciasOut_tam = tendenciasOut.size() * sizeof(float);
        cudaMalloc(&tendenciasOut_device, tendenciasOut_tam);
        cudaMemcpy(tendenciasOut_device, tendenciasOut.data(), tendenciasOut_tam, cudaMemcpyHostToDevice);
        */
        // PLAZAS SOBRANTES
        float * plazas_sobrantes_device;
        int plazas_sobrantes_tam = plazasSobrantes.size() * sizeof(float);
        cudaMalloc(&plazas_sobrantes_device, plazas_sobrantes_tam);
        cudaMemcpy(plazas_sobrantes_device, plazasSobrantes.data(), plazas_sobrantes_tam, cudaMemcpyHostToDevice);

        // COSTE KMS EXTRA  
        float * costeKmsExtra_device;
        int costeKmsExtra_tam = costeKmsExtra.size() * sizeof(float);
        cudaMalloc(&costeKmsExtra_device, costeKmsExtra_tam);
        cudaMemcpy(costeKmsExtra_device, costeKmsExtra.data(), costeKmsExtra_tam, cudaMemcpyHostToDevice);

        // COSTE KMS TENDENCIA
        float * costeKmsTendencia_device;
        int costeKmsTendencia_tam = costeKmsTendencia.size() * sizeof(float);
        cudaMalloc(&costeKmsTendencia_device, costeKmsTendencia_tam);
        cudaMemcpy(costeKmsTendencia_device, costeKmsTendencia.data(), costeKmsTendencia_tam, cudaMemcpyHostToDevice);

        // idEstaciones
        int idEstaciones_device = Poblacion[0].getNumEstaciones();

        // USUARIOS PERDIDOS
        int * usuariosPerdidos_device;
        int usuariosPerdidos_tam = usuariosPerdidos.size() * sizeof(int);
        cudaMalloc(&usuariosPerdidos_device, usuariosPerdidos_tam);
        cudaMemcpy(usuariosPerdidos_device, usuariosPerdidos.data(), usuariosPerdidos_tam, cudaMemcpyHostToDevice);

        // ESTACIONES LLENAS
        int * estaciones_llenas_device;
        int estaciones_llenas_tam = estaciones_llenas.size() * sizeof(int);
        cudaMalloc(&estaciones_llenas_device, estaciones_llenas_tam);
        cudaMemcpy(estaciones_llenas_device, estaciones_llenas.data(), estaciones_llenas_tam, cudaMemcpyHostToDevice);

        // ESTACIONES VACIAS
        int * estaciones_vacias_device;
        int estaciones_vacias_tam = estaciones_vacias.size() * sizeof(int);
        cudaMalloc(&estaciones_vacias_device, estaciones_vacias_tam);
        cudaMemcpy(estaciones_vacias_device, estaciones_vacias.data(), estaciones_vacias_tam, cudaMemcpyHostToDevice);
        int num_bloques = ceil(Poblacion.size() / 32);

        cuda_evaluacion<<<num_bloques, 32>>>(capacidades_device, filas_capacidades, columnas_capacidades,
                                            fitness_device, num_estaciones_device, deltas_device, factor_stress_device,
                                            por_seguridad_device, totalbicis_device, capacidades_total_device, valor_kms_device,
                                            tendencias_device, /*tendenciasIn_device, tendenciasOut_device,*/ usuariosPerdidos_device,
                                            factor_sobrante_device, cercanias_device, cercanias_km_device, columnas_cercanias_km,
                                            costeKmsExtra_device, costeKmsTendencia_device, plazas_sobrantes_device,
                                            estaciones_llenas_device, estaciones_vacias_device, filas_deltas, columnas_deltas,
                                            idEstaciones_device, columnas_cercanias, columnas_tendencias);

        cudaDeviceSynchronize();

        // Copia de valores de la GPU a la CPU
        cudaMemcpy(fitness.data(), fitness_device, fitness_tam, cudaMemcpyDeviceToHost);
        cudaMemcpy(plazasSobrantes.data(), plazas_sobrantes_device, plazas_sobrantes_tam, cudaMemcpyDeviceToHost);
        cudaMemcpy(costeKmsExtra.data(), costeKmsExtra_device, costeKmsExtra_tam, cudaMemcpyDeviceToHost);
        cudaMemcpy(costeKmsTendencia.data(), costeKmsTendencia_device, costeKmsTendencia_tam, cudaMemcpyDeviceToHost);
        cudaMemcpy(usuariosPerdidos.data(), usuariosPerdidos_device, usuariosPerdidos_tam, cudaMemcpyDeviceToHost);
        cudaMemcpy(estaciones_llenas.data(), estaciones_llenas_device, estaciones_llenas_tam, cudaMemcpyDeviceToHost);
        cudaMemcpy(estaciones_vacias.data(), estaciones_vacias_device, estaciones_vacias_tam, cudaMemcpyDeviceToHost);
    
        // Liberamos la memoria en la GPU
        cudaFree(capacidades_device);
        cudaFree(fitness_device);
        cudaFree(capacidades_total_device);
        //cudaFree(tendenciasIn_device);
        //cudaFree(tendenciasOut_device);
        cudaFree(plazas_sobrantes_device);
        cudaFree(costeKmsExtra_device);
        cudaFree(costeKmsTendencia_device);
        cudaFree(usuariosPerdidos_device);
        cudaFree(estaciones_llenas_device);
        cudaFree(estaciones_vacias_device);
    }
    catch(exception & e){
        cerr << "Error en la evaluación de la población: " << e.what() << endl;
        guardar_error(config);
    }
    try{
        for(int i = 0; i < fitness.size(); i++){
            Poblacion[i].setFitness(fitness[i]);
            Poblacion[i].setSobrante(plazasSobrantes[i]);
            Poblacion[i].setCosteKmExtra(costeKmsExtra[i]);
            Poblacion[i].setCosteKmsTendencia(costeKmsTendencia[i]);
            Poblacion[i].setUsuariosPerdidos(usuariosPerdidos[i]);
            Poblacion[i].setEstacionesLlenas(estaciones_llenas[i]);
            Poblacion[i].setEstacionesVacias(estaciones_vacias[i]);    
        }
        config.setEvaluaciones(config.getEvaluaciones() + fitness.size());
        config.setNLLamadasEvaluacion(config.getNLLamadasEvaluacion() + 1);
    }
    catch(exception & e){
        cerr << "Error en la actualización de la población: " << e.what() << endl;
        guardar_error(config);
    }
}