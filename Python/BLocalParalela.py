# !/usr/bin/env python
# coding: utf-8
import sys
import random
import librerias.z_io as z_io
import librerias.z_output as z_output
from Paralelismo import evaluarPoblacion
from datetime import datetime
from numba import cuda
from z_Individuo import Individuo_POO
from librerias.z_io import debug
from librerias.z_output import generarResultados


def busqueda_entorno_gpu(miindividuo, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device):
    '''
            AQUÍ PODRÍAMOS LLAMAR A AMBOS MÉTODOS PARA COMPARAR
        '''

    poblacion = []
    #poblacion.append(miindividuo)
    for i in range(CONFIG["NUM_VECINOS"]):
        vecino = miindividuo.mutacion(tope_inferior=451, config=CONFIG)
        
        # vecino.evaluacion_individuo_app_2(CONFIG)
        #if i==0:
        #    print("primer Vecino:",vecino.CAPACIDAD)
        # if vecino.FITNESS < miindividuo.FITNESS:
        #    mejor_individuo = vecino
        poblacion.append(vecino)

    #print("Evaluación Paralela")
    evaluarPoblacion(poblacion, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device)
    poblacionOrdenada = sorted(poblacion, key=lambda x: x.FITNESS)
    #print("Se devuelve el mejor_individuo: ",poblacionOrdenada[0], " y se partía de:",miindividuo)
    mejor_individuo = poblacionOrdenada[0]
    return mejor_individuo


def busqueda_entorno(miindividuo, CONFIG):
    '''
        AQUÍ PODRÍAMOS LLAMAR A AMBOS MÉTODOS PARA COMPARAR
    '''
    mejor_individuo = miindividuo.copy()
    lista = []

    for _ in range(CONFIG["NUM_VECINOS"]):
        vecino = miindividuo.mutacion(tope_inferior=451, config=CONFIG)
        vecino.evaluacion_individuo_app_2(CONFIG)
        if vecino.FITNESS < miindividuo.FITNESS:
            mejor_individuo = vecino
        lista.append(vecino)


    return mejor_individuo


def busqueda_local_mejor(base, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device):

    minivel = 1000
    # IndMinimo = Individuo_POO()
    #IndMinimo.setCapacidad(base.CAPACIDAD)
    if base.FITNESS == 0:
        base.evaluacion_individuo(CONFIG)  # AQUI SE EVALUA en SECUENCIAL PORQUE ES EL PRIMERO
    Individuos_List = []
    Individuos_List.append(base)
    IndMinimo = base
    epocas = 0
    # bucle de la evolución principal, se repite durante MAX_EPOCAS sin mejora, osea que el número de generaciones es variable

    while epocas < CONFIG["MAX_EPOCAS_BL"]:

        mejor_vecino = busqueda_entorno_gpu(IndMinimo.copy(),CONFIG, deltas_device, cercanias_device, cercaniasKm_device,tendencias_device) ##Bucle fuera
        #debug(mejor_vecino, nivel =minivel)
        #epocas = 100
        #print("Esta cambiada la búsqueda local para depurar y pongo épocas a 100!!!")
        Individuos_List.append(mejor_vecino.copy())
        if mejor_vecino.FITNESS >= IndMinimo.FITNESS:
            epocas += 1
            # No hay mejora, se incrementan las épocas y se pone un . en el log
            print(".", end= " ")
        else:
            epocas += 1
            IndMinimo = mejor_vecino.copy()
            #Individuos_List.append(IndMinimo)
            print("ep:",epocas, "F:", IndMinimo.FITNESS, end = "#\n")
            if CONFIG["BL_FIJA"] == 0:
                    epocas=0

    #print(IndMinimo)

    return IndMinimo, Individuos_List



def lanzador(CONFIG, BASE):
    CONFIG["starti"] = datetime.now()
    semilla_generada = random.random()
    print("Semilla generada:",semilla_generada)
    CONFIG["SEMILLA"]=semilla_generada
    #random.seed(0.24205531282932957)
    print("CONFIG[SEMILLA]=",CONFIG["SEMILLA"])

    #random.seed(CONFIG["SEMILLA"])

    IBase = Individuo_POO()
    IBase.setCapacidad(BASE)
    CONFIG.update({"CAPACIDADES_BASE": IBase.CAPACIDAD})
    print(" Individuo BASE", IBase)
    IBase.evaluacion_individuo(CONFIG);
    print("Individuo Base Evaluado:______________",IBase.FITNESS, "Km Tendencia", IBase.KM_TENDENCIA, " Km Extra:", IBase.KM_EXTRA);
    # se copia una sola vez a la GPU para toda la evaluación
    deltas_device = cuda.to_device(CONFIG["DELTAS"])
    cercanias_device = cuda.to_device(CONFIG["CERCA"])
    cercaniasKm_device = cuda.to_device(CONFIG["CERCA_KM"])
    tendencias_device = cuda.to_device(CONFIG["TENDENCIA"])

    MEJOR_INDIVIDUO, LISTA_INDIVIDUOS = busqueda_local_mejor(IBase.mutacion_extrema(100, CONFIG, IBase.NUM_ESTACIONES),
                                                             CONFIG, deltas_device, cercanias_device,
                                                             cercaniasKm_device, tendencias_device)

    RESULTADOS = {}
    RESULTADOS["MEJOR_IND"] = MEJOR_INDIVIDUO
    RESULTADOS["LISTA_IND"] = LISTA_INDIVIDUOS
    CONFIG.update({"IBASE-CAPACIDADES_MEJOR": (IBase.CAPACIDAD - MEJOR_INDIVIDUO.CAPACIDAD)})
    CONFIG["finali"] = datetime.now()

    generarResultados(CONFIG, RESULTADOS)


if __name__ == "__main__":
    '''
    Esta es la función principal donde sólo debería tener llamadas a las ejecuciones
    '''

    print(sys.argv[1])

    debug("Programa principal Búsqueda Local Paralela")

    EXPERIMENTOS = z_io.extraer_ejecucion(sys.argv[1])

    for orden, (cc, bb) in enumerate(EXPERIMENTOS):
        # print("Linea", str(orden), ": ", str(cc)[:], "-", str(bb))
        try:
            lanzador(cc, bb)
        except Exception as inst:
            print("Ha habido algún error")
            print(str(inst))
            print(inst.args)
            z_output.guardar_error(cc)


