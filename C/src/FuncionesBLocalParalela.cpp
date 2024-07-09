/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de con las definiciones de búsqueda local 
// Funciones tanto para CUDA como para OpenMP
/***************************************************************************/

#include "FuncionesBLocalParalela.h"
#include <iostream>

using namespace std;

/*************************************************************************/
/*************************************************************************/    
// Ordena una población según el FITNESS
// Recibe:
//      poblacion: Lista de individuos sin ordenar
//      poblacion_ordenada: Lista de individuos ordenados según el fitness

void ordenarPoblacion(vector<Individuo_POO> & poblacion, vector<Individuo_POO> & poblacion_ordenada){
    if(poblacion != poblacion_ordenada)
        poblacion_ordenada = poblacion;
    
    sort(poblacion_ordenada.begin(), poblacion_ordenada.end(), [](Individuo_POO a, Individuo_POO b){
        return a.getFitness() < b.getFitness();
    });
}

/*************************************************************************/
/*************************************************************************/  
// Búsqueda en el entorno usando la GPU (CUDA y OPENMP)

Individuo_POO busqueda_entorno_gpu(Individuo_POO miindividuo, Datos_EVAL & config,
                                    int * deltas_device, int df, int dc, int dt,
                                    int * cercanias_device, int cc, int ct,
                                    float * cercanias_km_device, int ckmc, int ckmt,
                                    int * tendencias_device, int tc, int tt, string cadena){
    
    Individuo_POO mejor_individuo, vecino;
    vector<Individuo_POO> poblacion, poblacion_ordenada;
    
    for(int i = 0; i < config.getNumVecinos(); i++){
        vecino = miindividuo.mutacion(451, config);
        poblacion.push_back(vecino);    
    }

    if(cadena == "CUDA"){
        evaluarPoblacion_CUDA(poblacion, config, deltas_device, cercanias_device, 
                            cercanias_km_device, tendencias_device, df, dc, 
                            ckmc, cc, tc);
    }
    if(cadena == "OMP"){
        evaluarPoblacion_OMP(poblacion, config, deltas_device, df, dc, dt,
                            cercanias_device, cc, ct, 
                            cercanias_km_device, ckmc, ckmt,
                            tendencias_device, tc, tt);
    }
                    
    ordenarPoblacion(poblacion, poblacion_ordenada);

    mejor_individuo = poblacion_ordenada[0];

    return mejor_individuo;
};


/*************************************************************************/
/*************************************************************************/  
// Búsqueda del mejor individuo

pair<Individuo_POO, vector<Individuo_POO>> busqueda_local_mejor(Individuo_POO base,
                                            Datos_EVAL & config, vector<vector<int>> & deltas, 
                                            vector<vector<int>> & cercanias, vector<vector<float>> & cercanias_km,
                                            vector<vector<int>> & tendencias, string cadena){
    if (base.getFitness() == 0)
        base.evaluacion_individuo(config);

    vector<Individuo_POO> Individuos_List;
    Individuos_List.push_back(base);
    Individuo_POO IndMinimo = base;
    int epocas = 0;

    // Cambiamos las matrices a vectores para que puedan ser usados por la GPU
    // DELTAS
    int deltas_filas = deltas.size();
    int deltas_columnas = deltas[0].size();
    vector<int> deltas_vector;
    for(const auto & i : deltas){
        deltas_vector.insert(deltas_vector.end(), i.begin(), i.end());
    }
    int deltas_tam = deltas_vector.size() * sizeof(int);

    // TENDENCIAS
    int tendencias_columnas = tendencias[0].size();
    vector<int> tendencias_vector;
    for(const auto & i : tendencias){
        tendencias_vector.insert(tendencias_vector.end(), i.begin(), i.end());
    }
    int tendencias_tam = tendencias_vector.size() * sizeof(int);

    // CERCANIAS
    int cercanias_columnas = cercanias[0].size();
    vector<int> cercanias_vector;
    for(const auto & i : cercanias){
        cercanias_vector.insert(cercanias_vector.end(), i.begin(), i.end());
    }
    int cercanias_tam = cercanias_vector.size() * sizeof(int);

     // CERCANIAS KM
    int cercaniaskm_columnas = cercanias_km[0].size();
    vector<float> cercanias_km_vector;
    for(const auto & i : cercanias_km){
        cercanias_km_vector.insert(cercanias_km_vector.end(), i.begin(), i.end());
    }
    int cercanias_km_tam = cercanias_km_vector.size() * sizeof(float);

    // Punteros necesarios
    int * deltas_device;
    int * cercanias_device;
    float * cercanias_km_device;
    int * tendencias_device;

    if(cadena == "CUDA"){
        // DELTAS
        cudaMalloc(&deltas_device, deltas_tam);
        cudaMemcpy(deltas_device, deltas_vector.data(), deltas_tam, cudaMemcpyHostToDevice);

        // CERCANIAS
        cudaMalloc(&cercanias_device, cercanias_tam);
        cudaMemcpy(cercanias_device, cercanias_vector.data(), cercanias_tam, cudaMemcpyHostToDevice);

        // CERCANIAS KM
        cudaMalloc(&cercanias_km_device, cercanias_km_tam);
        cudaMemcpy(cercanias_km_device, cercanias_km_vector.data(), cercanias_km_tam, cudaMemcpyHostToDevice);

        // TENDENCIAS
        cudaMalloc(&tendencias_device, tendencias_tam);
        cudaMemcpy(tendencias_device, tendencias_vector.data(), tendencias_tam, cudaMemcpyHostToDevice);
    }
    if(cadena == "OMP"){
        // DELTAS
        deltas_device = deltas_vector.data();
        
        // CERCANIAS
        cercanias_device = cercanias_vector.data();
        
        // CERCANIAS KM
        cercanias_km_device = cercanias_km_vector.data();

        // TENDENCIAS
        tendencias_device = tendencias_vector.data();
    }

    while(epocas < config.getMaxEpocasBL()){
        Individuo_POO mejor_vecino;
        mejor_vecino = busqueda_entorno_gpu(IndMinimo, config, deltas_device, deltas_filas, deltas_columnas, deltas_tam,
                                            cercanias_device, cercanias_columnas, cercanias_tam,
                                            cercanias_km_device, cercaniaskm_columnas, cercanias_km_tam, 
                                            tendencias_device, tendencias_columnas, tendencias_tam, cadena);
      

        Individuo_POO mejor_vecino_copia = mejor_vecino;
        Individuos_List.push_back(mejor_vecino_copia);
        if(mejor_vecino.getFitness() >= IndMinimo.getFitness()){
            epocas++;
            // No hay mejora, se incrementan las épocas y se pone un . en el log
            cout << ". " << endl;
        }
        else{
            epocas++;
            IndMinimo = mejor_vecino;

            cout << "ep: " << epocas << " F: " << IndMinimo.getFitness() << "#\n";
            if(config.getBLFija() == 0)
                epocas = 0;
        } 
    }
    return make_pair(IndMinimo, Individuos_List);
}

/*************************************************************************/
/*************************************************************************/  
// Función lanzador

void lanzador(Datos_EVAL & config, vector<int> & base, string cadena){
    time_t t1 = time(NULL);
    tm * tm1 = localtime(&t1);
    config.setTiempoInicial(tm1);
    
    // Generamos la semilla aleatoria
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0, 1.0);
    float semilla_generada = dis(gen);

    cout << "Semilla generada: " << semilla_generada << endl;
    config.setSemilla(semilla_generada);
    cout << "CONFIG[SEMILLA]=" << config.getSemilla() << endl;

    Individuo_POO IBase;
    IBase.setCapacidad(base);
    config.setCapacidadesBase(IBase.getCapacidad());
    
    cout << "Individuo BASE " << IBase.ToString() << endl;
    IBase.evaluacion_individuo(config);
    cout << endl << "Individuo Base Evaluado________________" << setprecision(8) << IBase.getFitness() << " Km Tendencia " << IBase.getKmTendencia() << " Km Extra " << IBase.getKmExtra() << endl;
    vector<vector<int>> DELTAS = config.getDeltas();          // DELTAS
    vector<vector<int>> CERCANIAS = config.getCerca();        // CERCANIAS
    vector<vector<float>> CERCANIAS_KM = config.getCercaKM(); // CERCANIAS KM
    vector<vector<int>> TENDENCIAS = config.getTendencia();   // TENDENCIAS

    pair<Individuo_POO, vector<Individuo_POO>> valores;
    valores = busqueda_local_mejor(IBase.mutacion_extrema(100, config, IBase.getNumEstaciones()), config, DELTAS, CERCANIAS, CERCANIAS_KM, TENDENCIAS, cadena);

    Individuo_POO mejor_individuo = valores.first;
    vector<Individuo_POO> lista_individuos = valores.second;
    Resultado resultados;
    resultados.m_individuo = &mejor_individuo;
    resultados.lista_individuos = lista_individuos;

    valarray<int> ibase_capacidad(IBase.getCapacidad().data(), IBase.getCapacidad().size());
    valarray<int> m_ind_capacidad(mejor_individuo.getCapacidad().data(), mejor_individuo.getCapacidad().size());

    config.setResta(ibase_capacidad - m_ind_capacidad);
    
    time_t t2 = time(NULL);
    tm * tm2 = localtime(&t2);
    config.setTiempoFinal(tm2);
   
    cout << "Tiempo final: " << config.getTiempoFinal() << endl;
    generarResultados(config, resultados, IBase);
}