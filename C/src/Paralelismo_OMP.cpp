/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Implementación de las funciones de paralelismo OMP
/***************************************************************************/
#include "Paralelismo_OMP.h"
#include <iostream>

using namespace std;

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
                    int idest_size, int tam_poblacion, int nteams, int nhebras){
    

    #pragma omp target enter data map(to: deltas_device[:deltas_tam], cercanias_device[:cercanias_tam]) \
                                    map(to: cercaniaskm_device[:cercaniaskm_tam], tendencias_device[:tendencias_tam]) \
                                    map(to: capacidades_device[:capacidades_device_tam], capacidades_total_device[:capacidades_total_tam]) \
                                    map(to: fitness_device[:fitness_tam], usuariosPerdidos_device[:usuariosPerdidos_tam]) \
                                    map(to: estaciones_llenas_device[:estaciones_llenas_tam], estaciones_vacias_device[:estaciones_vacias_tam]) \
                                    map(to: plazasSobrantes_device[:plazasSobrantes_tam], costeKmsExtra_device[:costeKmsExtra_tam]) \
                                    map(to: costeKmsTendencia_device[:costeKmsTendencia_tam], idest_size)
    #pragma omp target teams distribute parallel for num_teams(nteams) thread_limit(nhebras)
    for(int i = 0; i < tam_poblacion; i++){
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

        for(int linea_delta = 0; linea_delta < deltas_filas; linea_delta++){
            for(int est_Origen = 0; est_Origen < idest_size; est_Origen++){
                float pendiente = deltas_device[linea_delta * deltas_columnas + est_Origen] * factor_stress;
                int nuevatendencia = tendencias_device[linea_delta * tendencias_columnas + est_Origen];
                
                int it_estacion = 0;
                int idest = cercanias_device[est_Origen * cercanias_columnas];

                if(pendiente == 0){
                    if((ocupacion[est_Origen] == capacidades_device[i * capacidades_columnas + est_Origen] || ocupacion[est_Origen] == 0)){
                        mov_virtual = true;
                        pendiente = nuevatendencia;
                    }
                    else
                        mov_virtual = false;
                }
                else
                    mov_virtual = false;

                if(pendiente > (capacidades_device[i * capacidades_columnas + est_Origen] - ocupacion[est_Origen]))
                    usuariosPerdidos_device[i] += pendiente - (capacidades_device[i * capacidades_columnas + est_Origen] - ocupacion[est_Origen]);
                else if((ocupacion[est_Origen] + pendiente) < 0)
                    usuariosPerdidos_device[i] -= pendiente + ocupacion[est_Origen];

                while(pendiente != 0){
                    if(pendiente > 0){ // Vamos a meter est_Origen
                        if((ocupacion[idest] + pendiente) <= capacidades_device[i * capacidades_columnas + idest]){ // Hay huecos suficientes
                            movido = pendiente;
                            pendiente = 0;
                        }
                        else{ // No hay bastantes huecos
                            movido = capacidades_device[i * capacidades_columnas + idest] - ocupacion[idest];
                            pendiente -= movido;
                        }
                        if(!mov_virtual)
                            ocupacion[idest] = ocupacion[idest] + movido;
                        else{
                            entrada_virt = true;
                            if(ocupacion[idest] == capacidades_device[i * capacidades_columnas + idest])
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

                    costeKmsExtra = movido * cercaniaskm_device[est_Origen * cercaniaskm_columnas + it_estacion];

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

                    idest = cercanias_device[est_Origen * cercanias_columnas + it_estacion];
            
                }
            }
        }

        float bicis_total = (totalBicis * (1 + porSeguridad) * 1.0);
        plazasSobrantes_device[i] = (capacidades_total_device[i] - bicis_total) / bicis_total;
        fitness_device[i] = ((costeKmsExtra_device[i] + costeKmsTendencia_device[i]) * valor_kms + plazasSobrantes_device[i] * factor_sobrante);
        estaciones_llenas_device[i] = llenas;
        estaciones_vacias_device[i] = vacias;
    }
    #pragma omp target exit data map(from: fitness_device[:fitness_tam]) map(from: usuariosPerdidos_device[:usuariosPerdidos_tam]) \
                                    map(from: estaciones_llenas_device[:estaciones_llenas_tam]) map(from: estaciones_vacias_device[:estaciones_vacias_tam]) \
                                    map(from: plazasSobrantes_device[:plazasSobrantes_tam]) map(from: costeKmsExtra_device[:costeKmsExtra_tam]) \
                                    map(from: costeKmsTendencia_device[:costeKmsTendencia_tam]) \
                                    map(delete: deltas_device[:deltas_tam]) map(delete: cercanias_device[:cercanias_tam]) \
                                    map(delete: cercaniaskm_device[:cercanias_km_tam]) map(delete: tendencias_device[:tendencias_tam]) \
                                    map(delete: capacidades_device[:capacidades_device_tam]) map(delete: capacidades_total_device[:capacidades_total_tam]) \
                                    map(delete: fitness_device[:fitness_tam]) map(delete: usuariosPerdidos_device[:usuariosPerdidos_tam]) \
                                    map(delete: estaciones_llenas_device[:estaciones_llenas_tam]) map(delete: estaciones_vacias_device[:estaciones_vacias_tam]) \
                                    map(delete: plazasSobrantes_device[:plazasSobrantes_tam]) map(delete: costeKmsExtra_device[:costeKmsExtra_tam]) \
                                    map(delete: costeKmsTendencia_device[:costeKmsTendencia_tam])

}


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
                            int * tendencias, int tc, int tt){

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
    
        // Se copia una sola vez a la GPU para la evaluación
        // CAPACIDADES
        int capacidades_filas = capacidades.size();
        int capacidades_columnas = capacidades[0].size();
        vector<int> capacidades_vector;
        for(const auto & i : capacidades){
            capacidades_vector.insert(capacidades_vector.end(), i.begin(), i.end());
        }
        int * capacidades_device = capacidades_vector.data();
        int capacidades_device_tam = capacidades_vector.size();

        // FITNESS
        float * fitness_device = fitness.data();
        int fitness_tam = fitness.size();

        // CAPACIDADES TOTALES
        int * capacidades_total_device = capacidades_total.data();
        int capacidades_total_tam = capacidades_total.size();

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

        // PLAZAS SOBRANTES
        float * plazasSobrantes_device = plazasSobrantes.data();
        int plazasSobrantes_tam = plazasSobrantes.size();

        // COSTE KMS EXTRA
        float * costeKmsExtra_device = costeKmsExtra.data();
        int costeKmsExtra_tam = costeKmsExtra.size();

        // COSTE KMS TENDENCIA
        float * costeKmsTendencia_device = costeKmsTendencia.data();
        int costeKmsTendencia_tam = costeKmsTendencia.size();
        
        // idEstaciones
        int idEstaciones_device = Poblacion[0].getNumEstaciones();

        // USUARIOS PERDIDOS
        int * usuariosPerdidos_device = usuariosPerdidos.data();
        int usuariosPerdidos_tam = usuariosPerdidos.size();
        
        // ESTACIONES LLENAS
        int * estaciones_llenas_device = estaciones_llenas.data();
        int estaciones_llenas_tam = estaciones_llenas.size();

        // ESTACIONES VACIAS
        int * estaciones_vacias_device = estaciones_vacias.data();
        int estaciones_vacias_tam = estaciones_vacias.size();
        
        // Definimos el número de bloques y hebras que se van a usar
        int nteams = ceil(Poblacion.size() / 32);
        int nhebras = 32;

        omp_evaluacion(capacidades_device, capacidades_columnas, capacidades_device_tam, fitness_device, fitness_tam,
                        deltas, df, dc, dt, cercanias, cc, ct, cercanias_km, ckmc, ckmt,
                        tendencias, tc, tt, num_estaciones_device, factor_stress_device, 
                        por_seguridad_device, totalbicis_device, valor_kms_device,
                        capacidades_total_device, capacidades_total_tam, factor_sobrante_device,
                        usuariosPerdidos_device, usuariosPerdidos_tam, costeKmsExtra_device, costeKmsExtra_tam,
                        costeKmsTendencia_device, costeKmsTendencia_tam,
                        plazasSobrantes_device, plazasSobrantes_tam, 
                        estaciones_llenas_device, estaciones_llenas_tam, estaciones_vacias_device, estaciones_vacias_tam,
                        idEstaciones_device, Poblacion.size(), nteams, nhebras);
        #pragma omp barrier

        // No es necesaria la copia en las variables originales porque se pasan por referencia
    }
    catch(const exception& e){
        cerr << "Error en la función evaluarPoblacion copiando los datos de vuelta" << endl;
        cerr << e.what() << '\n';
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
    catch(const exception& e){
        cerr << "Error en la función evaluarPoblacion restaurando los datos de vuelta" << endl;
        cerr << e.what() << '\n';
        guardar_error(config);
    }
}
