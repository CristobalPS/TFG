#!/usr/bin/env python
# coding: utf-8
import os
import sys

import BLocalParalela as Local
import numpy as np
import random
import librerias.z_io as z_io
import librerias.z_output as z_output
import gc
from BLocalParalela import busqueda_local_mejor
from Paralelismo import evaluarPoblacion
from datetime import datetime
from numba import cuda
from z_Individuo import Individuo_POO
from librerias.z_io import debug
from librerias.z_output import generarResultados, guardar_error


## VARIABLES GLOBALES
## CONFIG = None

def elmejor(Poblacion, cuantos=1):

    ll = np.array([i.FITNESS for i in Poblacion]).argsort()
    return [Poblacion[i] for i in ll][:cuantos]



def Seleccion_Torneo(Poblacion, veces):

    lista = random.sample(Poblacion,veces)
    Mejor = lista[0]
    MejorFit = Mejor.FITNESS

    for i in range(1,veces):
        indi = lista[i]
        if indi.FITNESS < MejorFit:
            Mejor = indi
            MejorFit = indi.FITNESS

    return Mejor.copy()


def Reemplazo_Torneo(Poblacion, veces):

    lista = []
    for k in range(veces):
        lista.append(random.randint(0,len(Poblacion)-1))

    index = lista[0]
    PeorFit = Poblacion[index].FITNESS

    for indice in lista:
        indi = Poblacion[indice]
        if indi.FITNESS > PeorFit:
            index = indice
            PeorFit = indi.FITNESS


    return index

def evolutivoGeneracional(IBase, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device):

    BEST_INDIVIDUOS=[IBase]
    Poblacion = [IBase]
    Offspring = []
    elite = []
    Mejor = IBase.copy()
    CONFIG.update({"CAPACIDADES_BASE": IBase.CAPACIDAD})
    for i in range(CONFIG["TAM_POBLACION"] - 1):
        miind = IBase.mutacion_extrema(100, CONFIG, IBase.NUM_ESTACIONES)
        Poblacion.append(miind)
    print("Primera Población generada con :", CONFIG["TAM_POBLACION"], " individuos ")
    evaluarPoblacion(Poblacion, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device)
    Poblacion = sorted(Poblacion, key=lambda x: x.FITNESS)
    BEST_INDIVIDUOS.append(Poblacion[0].copy())
    iteraciones = 0
    epocas_sin_cambio = 0
    probCruce = CONFIG["PROB_CRUCE"]
    probMutacion = CONFIG["PROB_MUT"]
    probCruceYMutacion = probMutacion-(1-probCruce)
    if probCruceYMutacion<0:
        print(" Probabilidades no compatibles")
    hijos_mejor_padres= []
    while epocas_sin_cambio < CONFIG["MAX_EPOCAS_EVO"]:
        iteraciones += 1
        # print("Offsprint.len:",len(Offspring), " poblacion len ", len(Poblacion), " CONFIG TAM_ELITE:", CONFIG["TAM_ELITE"])
        while len(Offspring) < CONFIG["TAM_POBLACION"] - CONFIG["TAM_ELITE"]:
            # print("Estamos en epoca:", epocas_sin_cambio)
            Hijo1 = Seleccion_Torneo(Poblacion, CONFIG["TAM_TORNEO"])
            Hijo2 = Seleccion_Torneo(Poblacion, CONFIG["TAM_TORNEO"])
            #print("Padres")
            #print("Hijo1:", Hijo1)
            #print("Hijo2:", Hijo2)

            #if CONFIG["PROB_CRUCE"]<random.random():
            #    Hijo1, Hijo2 = Hijo1.cruceCincoPorCiento(Hijo2, 1000, 320, CONFIG)
            #if CONFIG["PROB_MUT"]<random.random():
            #    Hijo1 = Hijo1.mutacion(tope_inferior=451, config=CONFIG)
            #if CONFIG["PROB_MUT"] < random.random():
            #    Hijo2 = Hijo2.mutacion(tope_inferior=451, config=CONFIG)

            if probCruce>random.random():
                Hijo1, Hijo2 = Hijo1.cruceCincoPorCiento(Hijo2, 1000)
                if probCruceYMutacion>random.random():
                    Hijo1 = Hijo1.mutacion(tope_inferior=451, config=CONFIG)
                    Hijo2 = Hijo2.mutacion(tope_inferior=451, config=CONFIG)
            else:
                Hijo1 = Hijo1.mutacion(tope_inferior=451, config=CONFIG)
                Hijo2 = Hijo2.mutacion(tope_inferior=451, config=CONFIG)


            Offspring.append(Hijo1)
            Offspring.append(Hijo2)

            # Ahora se debería evaluar a toda la lista de individuos y luego ya reemplazar y añadir la élite

        # fitnessPoblacion = [ind.FITNESS for ind in Poblacion]
        # print("\n\n", len(Poblacion), " Padres It:", iteraciones, "Ordenados:\n")
        # for ind in Poblacion:
        #    print(ind)
        evaluarPoblacion(Offspring, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device)
        Offspring = sorted(Offspring, key=lambda x: x.FITNESS)
        # for ind in Offspring:
        #    print(ind)
        # print(len(Offspring)," Hijos It:",iteraciones, " Ordenados:\n")
        if(Offspring[0].FITNESS<Poblacion[0].FITNESS):
            hijos_mejor_padres.append(1)
            print("------------------------------------------------>>>>>>>>>>>>>>>>>>>    Hijos mejores")
        else:
            hijos_mejor_padres.append(0)
            print("------------------------------------------------>>>>>>>>>>>>>>>>>>>    No mejores")

        epocas_sin_cambio += 1
        for i in range(CONFIG["TAM_ELITE"]):
            elite.append((Poblacion[i]))
        Poblacion = elite + Offspring
        Offspring = []
        elite = []
        if iteraciones%CONFIG["GEN_BUSQUEDALOCAL"] == 0:
            print("Busqueda local INTERMEDIA con ", epocas_sin_cambio, " épocas y con BL_FIJA=",CONFIG["BL_FIJA"])
            print("Comienza la búsqueda con:",Poblacion[0])
            base = Poblacion[0].copy()
            indMinimo, _ = busqueda_local_mejor(base, CONFIG, deltas_device, cercanias_device, cercaniasKm_device,
                                             tendencias_device)
            print("Termina la búsqueda con IndMinimo:",indMinimo)
            Poblacion[0]=indMinimo

        Poblacion = sorted(Poblacion, key=lambda x: x.FITNESS)
        BEST_INDIVIDUOS.append(Poblacion[0].copy())
        # print("Número de individuos en BEST ", len(BEST_INDIVIDUOS))
        # if BEST_INDIVIDUOS[-2].FITNESS > Poblacion[0].FITNESS:
        #     print("Reseteo de las epocas...", epocas_sin_cambio, " a 0 en la Iteracion:", iteraciones)
        #     epocas_sin_cambio = 0
        #     # for ind in Poblacion:
        #     #    print(ind)
        #     print(Poblacion[0])

    print("Busqueda local FINAL")
    # bl_fija = CONFIG["BL_FIJA"]
    # CONFIG["BL_FIJA"]= 0
    Minimo, Best_Individuos_Busqueda = busqueda_local_mejor(Poblacion[0], CONFIG, deltas_device, cercanias_device, cercaniasKm_device,
                                     tendencias_device)
    # CONFIG["BL_FIJA"]= bl_fija
    print("Individuo Final")
    print(Minimo)
    CONFIG["finali"] = datetime.now()
    CONFIG["HIJOS_MEJOR_PADRE"] = hijos_mejor_padres
    CONFIG.update({"IBASE-CAPACIDADES_MEJOR": (IBase.CAPACIDAD - Minimo.CAPACIDAD)})
    BEST_INDIVIDUOS.append(Minimo.copy())
    return Minimo, BEST_INDIVIDUOS


def lanzador(CONFIG, BASE):

    CONFIG["starti"] = datetime.now()
    semilla_generada = random.random()
    print("Semilla generada:",semilla_generada)
    CONFIG["SEMILLA"]=semilla_generada
    #random.seed(CONFIG["SEMILLA"])
    print("CONFIG[SEMILLA]=",CONFIG["SEMILLA"])
    #random.seed(CONFIG["SEMILLA"])
    print("Max iteraciones: ",CONFIG["MAX_ITERACIONES"])
    print("Tamaño de Poblacion: ", CONFIG["TAM_POBLACION"])

    IBase  = Individuo_POO()
    IBase.setCapacidad(BASE)

    deltas_device = cuda.to_device(CONFIG["DELTAS"])
    cercanias_device = cuda.to_device(CONFIG["CERCA"])
    cercaniasKm_device = cuda.to_device(CONFIG["CERCA_KM"])
    tendencias_device = cuda.to_device(CONFIG["TENDENCIA"])
    IBase.evaluacion_individuo(CONFIG)
    print("Mes:", CONFIG["PATHENTRADA"])
    print("Individuo Base Evaluado:______________",IBase.FITNESS, "Km Tendencia", IBase.KM_TENDENCIA, " Km Extra:", IBase.KM_EXTRA, " Sobrante:", IBase.SOBRANTE);

    #LISTA_INDIVIDUOS=[IBase.copy()]
    #MEJOR_INDIVIDUO=IBase.copy()
    CONFIG["IBASE-CAPACIDADES_MEJOR"]=IBase.CAPACIDAD
    MEJOR_INDIVIDUO, LISTA_INDIVIDUOS = evolutivoGeneracional(IBase,CONFIG, deltas_device, cercanias_device,
                                                             cercaniasKm_device, tendencias_device)

    RESULTADOS: object = {}
    RESULTADOS["MEJOR_IND"] = MEJOR_INDIVIDUO
    RESULTADOS["LISTA_IND"] = LISTA_INDIVIDUOS

    CONFIG["finali"] = datetime.now()
    generarResultados(CONFIG, RESULTADOS)
    kk1 = deltas_device.copy_to_host()
    kk2 = cercanias_device.copy_to_host()
    kk3 = cercaniasKm_device.copy_to_host()
    kk4 = tendencias_device.copy_to_host()
    gc.collect()


if __name__ == "__main__":

    debug("Algoritmo Evolutivo Paralelo")
    print(sys.argv[1])
    EXPERIMENTOS = z_io.extraer_ejecucion(sys.argv[1])
    # print(EXPERIMENTOS)
    for orden, (cc, bb) in enumerate(EXPERIMENTOS):
        print("Linea", str(orden), ": ", str(cc)[:], "-", str(bb))
        try:
            lanzador(cc, bb)
        except Exception as inst:
            print("Ha habido algún error")
            print(str(inst))
            print(inst.args)
            z_output.guardar_error(cc)

