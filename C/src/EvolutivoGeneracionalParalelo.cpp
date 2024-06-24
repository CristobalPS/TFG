#include <FuncionesBLocalParalela.h>
#include <iostream>
#include <vector>
#include <valarray>
#include <algorithm>
#include <random>
using namespace std;

Individuo_POO Seleccion_Torneo(vector<Individuo_POO> & Poblacion, int veces){
    random_device rd;
    mt19937 gen(rd());

    vector<Individuo_POO> lista(veces);

    for(int i = 0; i < veces; i++){
        int index = gen() % Poblacion.size();
        lista[i] = Poblacion[index];
    }

    Individuo_POO mejor = lista[0];
    float MejorFit = mejor.getFitness();

    for(int i = 1; i < veces; i++){
        if(lista[i].getFitness() < MejorFit){
            mejor = lista[i];
            MejorFit = mejor.getFitness();
        }
    }

    return mejor;
}

int Reemplazo_Torneo(vector<Individuo_POO> & Poblacion, int veces){
    random_device rd;
    mt19937 gen(rd());

    vector<int> lista(veces);
    for(int i = 0; i < veces; i++){
        lista[i] = gen() % Poblacion.size();
    }

    int index = lista[0];
    float PeorFit = Poblacion[index].getFitness();

    for(int indice : lista){
        if(Poblacion[indice].getFitness() > PeorFit){
            index = indice;
            PeorFit = Poblacion[index].getFitness();
        }
    }

    return index;
}

// Función de evaluación
pair<Individuo_POO, vector<Individuo_POO>> evolutivoGeneracional(Individuo_POO & IBase, Datos_EVAL & config, 
                                                                vector<vector<int>> & deltas, vector<vector<int>> & cercanias, 
                                                                vector<vector<float>> & cercanias_km, vector<vector<int>> & tendencias, string cadena){
    
    vector<Individuo_POO> BEST_INDIVIDUOS;
    vector<Individuo_POO> Poblacion;
    vector<Individuo_POO> Offspring;
    vector<Individuo_POO> elite;
    BEST_INDIVIDUOS.push_back(IBase);
    Poblacion.push_back(IBase);

    Individuo_POO mejor = IBase;
    config.setCapacidadesBase(IBase.getCapacidad());
    for(int i = 0; i < config.getTamPoblacion()-1; i++){
        Individuo_POO miind = IBase.mutacion_extrema(100, config, IBase.getNumEstaciones());
        Poblacion.push_back(miind);
    }
    cout << "Primera Población generada con: " << config.getTamPoblacion() << " individuos" << endl;
    
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
    
        evaluarPoblacion_CUDA(Poblacion, config, deltas_device, cercanias_device, cercanias_km_device, tendencias_device,
                                deltas_filas, deltas_columnas, cercanias_columnas, cercaniaskm_columnas, tendencias_columnas);
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

        evaluarPoblacion_OMP(Poblacion, config, deltas_device, deltas_filas, deltas_columnas, deltas_tam,
                            cercanias_device, cercanias_columnas, cercanias_tam, cercanias_km_device, 
                            cercaniaskm_columnas, cercanias_km_tam,
                            tendencias_device, tendencias_columnas, tendencias_tam);
    }

    vector<Individuo_POO> poblacion_ordenada;
    ordenarPoblacion(Poblacion, Poblacion);

    BEST_INDIVIDUOS.push_back(Poblacion[0]);
    int iteraciones = 0;
    int epocas_sin_cambio = 0;
    float probCruce = config.getProbCruce();
    float probMutacion = config.getProbMut();
    float probCruceYMutacion = probMutacion - (1-probCruce);

    if(probCruceYMutacion < 0)
        cout << "Probabilidades no compatibles" << endl;

    vector<int> hijos_mejor_padres;

    while(epocas_sin_cambio < config.getMaxEpocasEvo()){
        iteraciones++;

        while(Offspring.size() < config.getTamPoblacion() - config.getTamElite()){
            Individuo_POO Hijo1 = Seleccion_Torneo(Poblacion, config.getTamTorneo());
            Individuo_POO Hijo2 = Seleccion_Torneo(Poblacion, config.getTamTorneo());

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dis(0, 1);

            if(probCruce > dis(gen)){
                Hijo1.cruceCincoPorCiento(Hijo1, Hijo2, 1000);
                if(probCruceYMutacion > dis(gen)){
                    Hijo1 = Hijo1.mutacion(451, config);
                    Hijo2 = Hijo2.mutacion(451, config);
                }
            }
            else{
                Hijo1 = Hijo1.mutacion(451, config);
                Hijo2 = Hijo2.mutacion(451, config);
            }

            Offspring.push_back(Hijo1);
            Offspring.push_back(Hijo2); 
        }

        if(cadena == "CUDA"){
            evaluarPoblacion_CUDA(Offspring, config, deltas_device, cercanias_device, cercanias_km_device, tendencias_device,
                                    deltas_filas, deltas_columnas, cercanias_columnas, cercaniaskm_columnas, tendencias_columnas);
        }
        if(cadena == "OMP"){
            evaluarPoblacion_OMP(Offspring, config, deltas_device, deltas_filas, deltas_columnas, deltas_tam,
                            cercanias_device, cercanias_columnas, cercanias_tam, cercanias_km_device, 
                            cercaniaskm_columnas, cercanias_km_tam,
                            tendencias_device, tendencias_columnas, tendencias_tam);
        }
        ordenarPoblacion(Offspring, Offspring);

        if(Offspring[0].getFitness() < Poblacion[0].getFitness()){
            hijos_mejor_padres.push_back(1);
            cout << "------------------------------->>>>>>>>>>>>>>>  Hijos mejores" << endl;
        }
        else{
            hijos_mejor_padres.push_back(0);
            cout << "------------------------------->>>>>>>>>>>>>>>  No mejores" << endl;
        }

        epocas_sin_cambio++;
        for(int i = 0; i < config.getTamElite(); i++){
            elite.push_back(Poblacion[i]);
        }
        Poblacion.clear();
        Poblacion = elite;
        Poblacion.insert(Poblacion.end(), Offspring.begin(), Offspring.end());
        Offspring.clear();
        elite.clear();

        if(iteraciones % config.getGenBusquedaLocal() == 0){
            cout << "Búsqueda local INTERMEDIA con " << epocas_sin_cambio << " épocas ";
            cout << "y con BL_FIJA = " << config.getBLFija() << endl;
            cout << "Comienza la búsqueda con " << Poblacion[0].ToString() << endl;

            Individuo_POO base = Poblacion[0];
            pair<Individuo_POO, vector<Individuo_POO>> valores;
            valores = busqueda_local_mejor(base, config, deltas, cercanias, cercanias_km, tendencias, cadena);
            Individuo_POO IndMinimo = valores.first;
            cout << "Termina la búsqueda con IndMinimo: " << IndMinimo.ToString() << endl;
            Poblacion[0] = IndMinimo;
        }

        ordenarPoblacion(Poblacion, Poblacion);
        BEST_INDIVIDUOS.push_back(Poblacion[0]);

        // if(BEST_INDIVIDUOS[BEST_INDIVIDUOS.size()-2].getFitness() > Poblacion[0].getFitness()){
        //     cout << "Reseteo de las épocas... " << epocas_sin_cambio << " a 0 en la iteración: " << iteraciones << endl;
        //     epocas_sin_cambio = 0;
        //     cout << Poblacion[0].ToString() << endl;
        // }
    }

    cout << "Búsqueda local FINAL" << endl;
    //int bl_fija = config.getBLFija();
    // config.setBLFija(0);
    pair<Individuo_POO, vector<Individuo_POO>> valores;
    valores = busqueda_local_mejor(Poblacion[0], config, deltas, cercanias, cercanias_km, tendencias, cadena);
    Individuo_POO Minimo = valores.first;
    vector<Individuo_POO> Best_Individuo_Busqueda = valores.second;
    //config.setBLFija(bl_fija);
    cout << "Individuo Final" << endl << Minimo.ToString() << endl;

    time_t t2 = time(NULL);
    tm * tm2 = localtime(&t2);
    config.setTiempoFinal(tm2);

    config.setHijosMejorPadre(hijos_mejor_padres);
    valarray<int> ibase_capacidad(IBase.getCapacidad().data(), IBase.getCapacidad().size());
    valarray<int> minimo_capacidad(Minimo.getCapacidad().data(), Minimo.getCapacidad().size());
    config.setResta(ibase_capacidad - minimo_capacidad);

    BEST_INDIVIDUOS.push_back(Minimo);

    return make_pair(Minimo, BEST_INDIVIDUOS);
}

/*************************************************************************/
/*************************************************************************/  
// Función lanzador

void lanzadorEv(Datos_EVAL & config, vector<int> & base, string cadena){
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

    IBase.evaluacion_individuo(config);
    cout << "Mes: " << config.getPathEntrada() << endl;
    cout << endl << "Individuo Base Evaluado________________" << IBase.getFitness() << " Km Tendencia " << IBase.getKmTendencia() << " Km Extra " << IBase.getKmExtra() << endl;

    vector<vector<int>> DELTAS = config.getDeltas();          // DELTAS
    vector<vector<int>> CERCANIAS = config.getCerca();        // CERCANIAS
    vector<vector<float>> CERCANIAS_KM = config.getCercaKM(); // CERCANIAS KM
    vector<vector<int>> TENDENCIAS = config.getTendencia();   // TENDENCIAS

    pair<Individuo_POO, vector<Individuo_POO>> valores;
    valarray<int> CAPACIDAD(IBase.getCapacidad().data(), IBase.getCapacidad().size());

    config.setResta(CAPACIDAD);
    valores = evolutivoGeneracional(IBase, config, DELTAS, CERCANIAS, CERCANIAS_KM, TENDENCIAS, cadena);

    Individuo_POO mejor_individuo = valores.first;
    vector<Individuo_POO> lista_individuos = valores.second;
    Resultado resultados;
    resultados.m_individuo = &mejor_individuo;
    resultados.lista_individuos = lista_individuos;

    time_t t2 = time(NULL);
    tm * tm2 = localtime(&t2);
    config.setTiempoFinal(tm2);
   
    cout << "Tiempo final: " << config.getTiempoFinal() << endl;
    generarResultados(config, resultados, IBase);
}

int main(int argc, char * argv[]){

    if(argc != 3){
        cerr << "Error en el número de argumentos" << endl;
        cerr << "Uso: " << argv[0] << " <fichero_configuracion.csv>" << " modo (CUDA/OMP)";
        cerr << endl;
        exit(-1);
    }

    Datos_EVAL cc;
    vector<int> bb;
    int lineas_leidas = 0;
    vector<Datos_EVAL> EXPERIMENTOS;
    EXPERIMENTOS = extraer_ejecucion(argv[1], cc, bb, lineas_leidas);

    for(int i = 0; i < EXPERIMENTOS.size(); i++){
        // cout << "Linea " << lineas_leidas << ": " << cc.ToString() << " - [";
        // for(int i = 0; i < bb.size(); i++){
        //     cout << bb[i] << " ";
        // }
        // cout << "]" << endl;
        try{
            lanzadorEv(EXPERIMENTOS[i], bb, argv[2]);
        }
        catch(const exception& e)
        {
            cout << "Ha habido algún error" << endl;
            cout << e.what() << endl;
            guardar_error(cc);
        }
    }
}



