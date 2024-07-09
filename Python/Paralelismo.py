# !/usr/bin/env python
# coding: utf-8
import gc
from datetime import datetime
import sys
import os
# import json
# import csv
import random

import librerias.z_io as z_io
from   librerias.z_io import debug
import librerias.z_output as z_output

import z_Individuo
from z_Individuo import Individuo_POO
import numpy as np
import numba
from gc import collect
from numba import cuda, int32, float32, float64
from math import ceil


def cuda_imprimir(i, capacidades_device, fitness_device,
                  num_estaciones, deltas_device,
                  factor_stress, porSeguridad,
                  totalBicis, capacidades_total_device,
                  valor_kms, tendencias_device,
                  tendenciasIn_device, tendenciasOut_device,
                  usuariosPerdidos_device, factor_sobrante,
                  cercania_device, cercaKm_device,
                  costeKmsExtra_device, costekmsTendencia_device,
                  plazasSobrantes_device, idEstaciones_device):
    print("d:Capadidades_device:", capacidades_device[i][0])
    print("d:fitness_device:", fitness_device[i])
    print("d:num_estaciones:", num_estaciones[0])
    print("d:deltas_device:", deltas_device[i][0])
    print("d:factor_stress:", factor_stress[0])
    print("d:porSeguridad:", porSeguridad[0])
    print("d:total_bicis:", totalBicis[0])
    print("d:capacidades_total_device:", capacidades_total_device[i])
    print("d:valor_Kms:", valor_kms[0])
    print("d:tendencias_device:", tendencias_device[i][0])
    print("d:tendenciasIn_device:", tendenciasIn_device[i])
    print("d:tendenciasOut_device:", tendenciasOut_device[i])
    print("d:usuariosPerdidos_device:", usuariosPerdidos_device[i])
    print("d:factorSobrante:", factor_sobrante[0])
    print("d:cercania_device:", cercania_device[i][0])
    print("d:cercaKm_device:", cercaKm_device[i][0])
    print("d:costeKmsExtra_device:", costeKmsExtra_device[i])
    print("d:costeKmsTendencia_device:", costekmsTendencia_device[i])
    print("d:plazasSobrantes_device:", plazasSobrantes_device[i])
    print("d:id_estaciones_device:", idEstaciones_device[i])

    '''
    fitness_device[i] = i;
    plazasSobrantes_device[i]=i+1;
    costeKmsExtra_device[i] = i + 2;
    costekmsTendencia_device[i] = i+3;
    usuariosPerdidos_device[i] = i+4;
    '''


@cuda.jit
def cuda_evaluacion(capacidades_device, fitness_device,
                    num_estaciones, deltas_device,
                    factor_stress, porSeguridad,
                    totalBicis, capacidades_total_device,
                    valor_kms, tendencias_device,
                    usuariosPerdidos_device, factor_sobrante,
                    cercania_device, cercaKm_device,
                    costeKmsExtra_device, costekmsTendencia_device,
                    plazasSobrantes_device, idEstaciones_device,
                    estaciones_llenas_device, estaciones_vacias_device):
    # primero hay que asignar a cada hebra qué individuo evalua
    # segundo comprobar que todos los datos necesarios están disponibles
    # tercero asignar fitness calculado por cada hebra al vector  de resultados

    i = cuda.threadIdx.x + (cuda.blockIdx.x * cuda.blockDim.x)
    if i < len(capacidades_device):

        entrada_virt = False
        contadorKmExtra = 0
        llenas = 0
        vacias = 0
        n_estaciones = 259
        ocupacion = numba.cuda.local.array(n_estaciones, dtype=int32)
        costeKmsExtra = numba.cuda.local.array(1, dtype=float64)

        for cont in range(ocupacion.shape[0]):
            ocupacion[cont] = 0

        for linea_delta in range(deltas_device.shape[0]):  # esto tiene que ir de 0 a 15
            # if i == 0:
            #    print("d:Capadidades_device:", capacidades_device[0][0])
            #    print("Estacion:", linea_delta)
            #    print("linea_delta:", linea_delta)
            for est_Origen in range(idEstaciones_device.shape[0]):

                # pendiente = np.round(deltas_device[linea_delta, est_Origen] * factor_stress)  # numero de est_Origen a depositar
                pendiente = deltas_device[linea_delta, est_Origen] * factor_stress[0]  # la línea original es la de arriba
                nuevatendencia = tendencias_device[linea_delta, est_Origen]

                it_stacion = 0  # numero de orden (n) de la mas cercana
                idest = cercania_device[est_Origen, 0]  # nombre de la n estacion mas cercana
                '''if nuevatendencia != 0:
                    mov_virtual = True
                    pendiente = nuevatendencia
                else:
                    mov_virtual = False
                '''

                if pendiente == 0:
                    if ocupacion[est_Origen] == capacidades_device[i][est_Origen] or ocupacion[est_Origen] == 0:
                        mov_virtual = True
                        pendiente = nuevatendencia
                    else:
                        mov_virtual= False
                else:
                    mov_virtual = False

                if pendiente > (capacidades_device[i][est_Origen] - ocupacion[est_Origen]):
                    # PARA GONZALO: los usuarios perdidos también es un dato de salida... de cada individuo supongo no?, osea que le tengo que pasar el vector que se me ha pasado, pero no está inicializado en ninguna parte
                    usuariosPerdidos_device[i] += pendiente - (capacidades_device[i][est_Origen] - ocupacion[est_Origen])
                    # if i == 0:
                    #     print("pendiente en el if ", pendiente)
                    #     print("capac en el if ", capacidades_device[i][est_Origen])
                    #     print("ocupacion en el if ", ocupacion[est_Origen])

                elif (ocupacion[est_Origen] + pendiente) < 0:
                    usuariosPerdidos_device[i] -= (pendiente + ocupacion[est_Origen])
                    # if i == 0:
                    #    sumados = pendiente + ocupacion[est_Origen]
                    #    print("P:Se suman en el elif", sumados)
                    # if i==0:
                #     print("usuariosPerdidos:", usuariosPerdidos_device[i])

                # mov_virtual = False

                while pendiente != 0:
                    if pendiente > 0:  ### Vamos a METER est_Origen
                        if ocupacion[idest] + pendiente <= capacidades_device[i][idest]:  ## HAY HUECOS SUFICIENTES
                            movido = pendiente
                            pendiente = 0

                        else:  ## NO HAY HUECOS BASTANTES
                            movido = capacidades_device[i][idest] - ocupacion[idest]
                            pendiente = pendiente - movido

                        if not mov_virtual:
                            ocupacion[idest] = ocupacion[idest] + movido

                        else:
                            entrada_virt = True
                            if ocupacion[idest] == capacidades_device[i][idest]:
                                llenas += 1

                    else:  ### Vamos a SACAR est_Origen
                        if ocupacion[idest] + pendiente >= 0:  ## HAY SUFICIENTES
                            movido = -pendiente
                            pendiente = 0

                        else:  ## FALTAN BICIS
                            movido = ocupacion[idest]
                            pendiente = pendiente + movido

                        if not mov_virtual:
                            ocupacion[idest] = ocupacion[idest] - movido
                        else:
                            entrada_virt = False
                            if ocupacion[idest] == 0:
                                vacias += 1
                                # tendencia = abs(config["TENDENCIA"][linea_delta, est_Origen])  # TODO cambiar

                    costeKmsExtra = movido * cercaKm_device[est_Origen, it_stacion]

                    if mov_virtual:
                        costekmsTendencia_device[i] += costeKmsExtra
                    else:
                        costeKmsExtra_device[i] += costeKmsExtra
                        contadorKmExtra = contadorKmExtra + 1

                        # if i == 0:
                        #    print("P: Movido:", movido, "P: CercaKm:", cercaKm_device[est_Origen, it_stacion])
                        #    print("P:CosteKmsExtraParcial:", costeKmsExtra)
                        #    print("P:CosteKmsExtra_device[i]:", costeKmsExtra_device[i], "contador:", contadorKmExtra)

                    # vamos a la siguiente
                    it_stacion += 1

                    # si no quedan huecos en ninguna estacion...
                    if it_stacion == num_estaciones[0]:
                        # print("SOCORROOO... QUE NO ME CABE!!")
                        # sys.exit(-1)
                        break
                    idest = cercania_device[est_Origen, it_stacion]


            # print("RESULTADO:        ", "\t".join([str(x) for x in ((self.CAPACIDAD - ocupacion))])[:], nivel=minivel)
            # "\t".join([str(round(float(x), 2)) for x in (config["TENDENCIA"][linea_delta])]))

        ## CALCULO DE FITNESS
        bicis_total = (totalBicis[0] * (1 + porSeguridad[0]) * 1.0)
        plazasSobrantes_device[i] = (capacidades_total_device[i] - bicis_total) / bicis_total

        # # f1: Kilometros*e^plazassobrantes
        ## fitness = (coste_kms_extra + coste_kms_tendencia+1)* exp(1+self.SOBRANTE)

        ## f2 =  Kilometros*e^plazassobrantes
        # fitness = exp(1+self.SOBRANTE + coste_kms_extra + coste_kms_tendencia)

        ## f3: Kilometros+plazassobrantesnp.zeros
        fitness_device[i] = (
                (costeKmsExtra_device[i] + costekmsTendencia_device[i]) * valor_kms[0] + plazasSobrantes_device[i] *
                factor_sobrante[0])


        # Yo creo que aquí el sobrante está mal, debería ser plazas_sobrantes_device[i][idest]
        estaciones_llenas_device[i] = llenas
        estaciones_vacias_device[i] = vacias



def evaluarPoblacion(Poblacion, CONFIG, deltas_device, cercanias_device, cercaniasKm_device, tendencias_device):
    try:
        capacidades = [ind.CAPACIDAD.copy() for ind in Poblacion]
        fitness = []
        #for x in Poblacion:
        #    fitness.append(x.FITNESS)
        #mostrar fitness de vecinos, para comprobar luego:
        #print ("Fitness Poblacion:", fitness )
        fitness = np.zeros(len(Poblacion)).astype(np.float64)
        capacidades_total = [ind.CAPACIDAD_TOTAL.copy() for ind in Poblacion]
        #tendenciasIn = np.zeros(len(Poblacion)).astype(np.float64)
        #tendenciasOut = np.zeros(len(Poblacion)).astype(np.float64)
        plazas_sobrantes = np.zeros(len(Poblacion)).astype(np.float64)
        coste_kms_extra = np.zeros(len(Poblacion)).astype(np.float64)
        coste_kms_tendencia = np.zeros(len(Poblacion)).astype(np.float64)
        usuariosPerdidos= [ind.USUARIOSPERDIDOS for ind in Poblacion]
        #usuariosPerdidos = [0 for ind in Poblacion]
        # HHHH Modificar los usuarios perdidos cuando lancemos experimentos a los que ya tenían los individuos
        estaciones_llenas = np.zeros(len(Poblacion)).astype(np.int32)
        estaciones_vacias = np.zeros(len(Poblacion)).astype(np.int32)

        capacidades_device = cuda.to_device(capacidades)  # las capacidades que proponen todos los individuos, vamos todos los cromosomas.
        fitness_device = cuda.to_device(fitness)
        capacidades_total_device = cuda.to_device(capacidades_total)


        num_estaciones_device = cuda.to_device(np.array([len(capacidades[0])]).astype(np.int32))   # creo que no es necesario que es un escalar, habría que ver en qué config está y asignarlo en la llamada
        # Se podría quitar para acelerar, Maribel
        factor_stress_device = cuda.to_device(np.array([CONFIG["FACTOR_STRESS"]]).astype(np.float32))
        #Se podría quitar para acelerar, Maribel
        por_seguridad_device = cuda.to_device(np.array([CONFIG["POR_SEGURIDAD"]]).astype(np.int32))
        totalbicis_device = cuda.to_device(np.array([CONFIG["TOTALBICIS"]]).astype(np.int32))
        #Se podría quitar para acelerar, Maribel
        valor_kms_device = cuda.to_device(np.array([CONFIG["VALOR_KM"]]).astype(np.float32))
        factor_sobrante_device = cuda.to_device(np.array([CONFIG["FACTOR_SOBRANTE"]]).astype(np.int32))
        #tendenciasIn_device = cuda.to_device(tendenciasIn)  # es el vector de tendencias de entrada que debe contener las tendencias de entrada de cada individuo
        #tendenciasOut_device = cuda.to_device(tendenciasOut) # es el vector de tendencias de salida que debe tener todas las tendencias out de cada individuo
        plazas_sobrantes_device = cuda.to_device(plazas_sobrantes)  # Creo que debería ser un vector de salida, pero en la línea 509 de la evaluación, aparece como un cálculo normal, quiero decir que quizás no haya que pasar este valor o
        coste_kms_extra_device = cuda.to_device(coste_kms_extra)
        coste_kms_tendencia_device = cuda.to_device(coste_kms_tendencia)
        idEstaciones_device = cuda.to_device(np.arange(Poblacion[0].NUM_ESTACIONES).astype(np.int32))
        usuariosPerdidos_device= cuda.to_device(usuariosPerdidos)
        estaciones_llenas_device = cuda.to_device(estaciones_llenas)
        estaciones_vacias_device = cuda.to_device(estaciones_vacias)

        cuda_evaluacion[ceil(len(Poblacion) / 32), 32](capacidades_device, fitness_device,
                                                               num_estaciones_device, deltas_device,
                                                               factor_stress_device, por_seguridad_device,
                                                               totalbicis_device, capacidades_total_device,
                                                               valor_kms_device, tendencias_device,
                                                               usuariosPerdidos_device, factor_sobrante_device,
                                                               cercanias_device, cercaniasKm_device,
                                                               coste_kms_extra_device, coste_kms_tendencia_device,
                                                               plazas_sobrantes_device, idEstaciones_device,
                                                               estaciones_llenas_device, estaciones_vacias_device)
        cuda.synchronize() 
        fitness = fitness_device.copy_to_host()
        plazas_sobrantes = plazas_sobrantes_device.copy_to_host()  # Creo que debería ser un vector de salida, pero en la línea 509 de la evaluación, aparece como un cálculo normal, quiero decir que quizás no haya que pasar este valor o
        coste_kms_extra = coste_kms_extra_device.copy_to_host()
        coste_kms_tendencia = coste_kms_tendencia_device.copy_to_host()
        usuariosPerdidos = usuariosPerdidos_device.copy_to_host()
        estaciones_llenas = estaciones_llenas_device.copy_to_host()
        estaciones_vacias = estaciones_vacias_device.copy_to_host()



    except Exception as inst:
        print("Error en la función evaluarPoblacion copiando los datos de vuelta")
        print(str(inst))
        print(inst.args)
        z_output.guardar_error(CONFIG)


    try:
        #print("Fitness Paralelos:   ", fitness)
        for index in range(len(fitness)):
            vecino = Poblacion[index]
            vecino.FITNESS = fitness[index]
            vecino.SOBRANTE = plazas_sobrantes[index]
            vecino.KM_EXTRA = coste_kms_extra[index]
            vecino.KM_TENDENCIA = coste_kms_tendencia[index]
            vecino.USUARIOSPERDIDOS = usuariosPerdidos[index]
            vecino.LLENAS = estaciones_llenas[index]
            vecino.VACIAS = estaciones_vacias[index]
            #if index==0:
            #    print("Fitness primer vecino:", vecino)
        CONFIG["EVALUACIONES"] = CONFIG["EVALUACIONES"] + len(fitness)
        CONFIG["N_LLAMADAS_EVALUACION"] = CONFIG["N_LLAMADAS_EVALUACION"]+1

    except Exception as inst:
        print("Error en la función EvaluarPoblacion restaurando los datos de vuelta")
        print(str(inst))
        print(inst.args)
        z_output.guardar_error(CONFIG)


    gc.collect()
