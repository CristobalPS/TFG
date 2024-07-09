#!/usr/bin/env python
# coding: utf-8
import csv

import matplotlib.pyplot as plt
import os
import sys
import numpy as np
import xlrd
import math
import distutils.dir_util
from datetime import datetime
import librerias.z_output as z_output

VERBOSE = True
def debug(*cadenas, nivel=1):
    '''
    Función para dirigir la evaluacion
    :param cadenas: lista (variable) de cadenas a imprimir
    :param nivel: indicador para ocultar algunas depuraciones
    :return:

    if VERBOSE and nivel>0:
        for elem in cadenas:
            print(elem,end="")
            print(" ", end="")
        print("")


    '''
    pass


def lectura_dir(cadena):
    devolver = ""
    for dir in str(cadena).split("/"):
        devolver = os.path.join(devolver,dir)

    return devolver

def lectura_float(cadena):
    # TODO modo prueba de fallos. capturar interrupcion
    # TODO meter valores por defecto
    
    # print("La cadena float es: ","*" + cadena+"*")
    if cadena == "":
        return None
    return float(cadena)

def lectura_int(cadena):
    # TODO modo prueba de fallos. capturar interrupcion
    # TODO meter valores por defecto

    # print("La cadena int es: ","*" + cadena+"*")
    if cadena == "":
        return None
    return int(float(cadena))


#############################################################################
###  LECTURA DE PARAMETROS
#############################################################################
def lectura(entrada):
    '''
    Rutina para analizar los valores de la entrada
    :param entrada: Entrada procedente de la línea de comandos
    :return: diccionario con todas las claves necesarias
    '''


    CONFIG = dict()
    BASE   = None
    
    # Fecha actual
    now = str(datetime.now()).replace(" ","_").replace(":","").replace(":","").replace("-","").replace(".","")
    CONFIG["TIEMPO"]          = now

    CONFIG['COMENTARIO']      = entrada["COMENTARIO"] # Comentario
    CONFIG['ALGORITMO']       = entrada["ALGORITMO"]
    
    CONFIG["PATHENTRADA"]     = lectura_dir(entrada["ENTRADA"])
    CONFIG["PATHSALIDA"]      = lectura_dir(entrada["SALIDA"])
    CONFIG["OUTPUT_FILE"]      = lectura_dir(entrada["OUTPUT_FILE"])

    if "POR_SEGURIDAD" in entrada:
        CONFIG['POR_SEGURIDAD']   = lectura_float(entrada["POR_SEGURIDAD"])
    else:
        CONFIG['POR_SEGURIDAD']=0
    if "VALOR_KM" in entrada:
        CONFIG["VALOR_KM"]        = lectura_float(entrada["VALOR_KM"])
    else:
        CONFIG["VALOR_KM"]=0
    CONFIG["MAX_EPOCAS_BL"]      = lectura_float(entrada["MAX_EPOCAS_BL"])
    CONFIG["FACTOR_SOBRANTE"] = lectura_float(entrada["FACTOR_SOBRANTE"])
    CONFIG['POR_ESTACIONES']  = lectura_float(entrada["POR_ESTACIONES"])
    CONFIG['POR_CANT']        = lectura_float(entrada["POR_CANT"])
    if "MOD_K" in entrada:
        CONFIG['MOD_K']           = lectura_float(entrada["MOD_K"])
    else:
        CONFIG['MOD_K']=10
    if 'AUMENTO_TRAFICO' in entrada.keys():
        CONFIG["FACTOR_STRESS"]   = lectura_float(entrada["AUMENTO_TRAFICO"])
    else:
        CONFIG["FACTOR_STRESS"]=1
    if "HIBRIDO" in entrada:
        CONFIG["HIBRIDO"]         = lectura_int  (entrada["HIBRIDO"])>0
    else:
        CONFIG["HIBRIDO"]=0
    CONFIG["MAX_ITERACIONES"] = lectura_int  (entrada["MAX_ITERACIONES"])
    CONFIG["SEMILLA"]         = lectura_int  (entrada["SEMILLA"])
    if "PASO_VNS" in entrada:
        CONFIG["PASO_VNS"]        = lectura_float(entrada["PASO_VNS"])
    else:
        CONFIG["PASO_VNS"] = 0
    CONFIG["NUM_VECINOS"]     = lectura_int  (entrada["NUM_VECINOS"])
    CONFIG["TAM_POBLACION"]   = lectura_int  (entrada["TAM_POBLACION"])
    CONFIG["TAM_ELITE"]       = lectura_int  (entrada["TAM_ELITE"])
    CONFIG["PROB_CRUCE"]      = lectura_float(entrada["PROB_CRUCE"])
    CONFIG["PROB_MUT"]        = lectura_float(entrada["PROB_MUT"])
    CONFIG["TAM_TORNEO"]      = lectura_int  (entrada["TAM_TORNEO"])
    CONFIG["GEN_BUSQUEDALOCAL"] = lectura_int  (entrada["GEN_BUSQUEDALOCAL"])
    CONFIG["BL_FIJA"]      = lectura_int  (entrada["BL_FIJA"])
    CONFIG["MAX_EPOCAS_EVO"]      = lectura_float(entrada["MAX_EPOCAS_EVO"])
    CONFIG["EVALUACIONES"]    = 0
    CONFIG["N_LLAMADAS_EVALUACION"]    = 0
    CONFIG["PATHRESULTADO"]   = CONFIG["PATHSALIDA"]   # TODO QUITAR ESTE VALOR
    CONFIG["HIJOS_MEJOR_PADRE"]    = []

    
    # ====  CARGA DE DATOS DE FICHEROS   ====================================
    BASE                      = carga_vector_int_parejaEstaciones (CONFIG["PATHENTRADA"]+"capacidades.csv",cabecera=False) # de igual tipo
    CONFIG["CERCA"]           = cargamatriz_int  (CONFIG["PATHENTRADA"]+"cercanas_indices.csv", cabecera=False) # de igual tipo
    CONFIG["CERCA_KM"]        = cargamatriz_float(CONFIG["PATHENTRADA"]+"cercanas_kms.csv", cabecera=False) # de igual tipo
    CONFIG["TENDENCIA"]       = cargamatriz_int(CONFIG["PATHENTRADA"] + "tendencia_media.csv", cabecera=True) # de igual tipo
    #CONFIG["TENDENCIA"]      = cargamatriz_int  (CONFIG["PATHENTRADA"]+"tendencia_media.csv")
    CONFIG["DELTAS"]          = cargamatriz_int(CONFIG["PATHENTRADA"]+"deltas.csv", cabecera=True) # da error y la última fila está llena de comas, además uno son floats y otro enteros
    CONFIG["TOTALBICIS"]      = CONFIG["DELTAS"].cumsum(axis=0).sum(axis=1).max()


    print("Tamaños de los datos leidos:")
    print("Individuo_Base:",BASE.size)
    print("Matriz Cerca:",CONFIG["CERCA"].shape)
    print("Matriz Cerca_Km:", CONFIG["CERCA_KM"].shape)
    print("Matriz Deltas:", CONFIG["DELTAS"].shape)
    print("Matriz Tendencias:", CONFIG["TENDENCIA"].shape)

    return CONFIG,BASE


def extraer_ejecucion(fichero, hoja = 'Hoja1'):

    print("Leyendo fichero: ", fichero)
    wb = xlrd.open_workbook(fichero)
    hoja1 = wb.sheet_by_name(hoja)

    parametros = []
    for i in range(hoja1.ncols):
        valor = str(hoja1.cell_value(0,i))
        if valor!= "":
            parametros.append((i,valor))

    ejecuciones = []
    for linea in range(1,hoja1.nrows):
        orden = dict()
        for (p,v) in parametros:
            orden[v] = str(hoja1.cell_value(linea,p))
        if len(orden["COMENTARIO"].strip())==0:
            print("Linea", str(linea+1),": ",str(orden))
            try:
                cc,bb = lectura(orden)
                cc["LINEAXLS"] = linea + 1
                ejecuciones.append((cc,bb))
            except:
                cc1 = {}
                cc1["LINEAXLS"] = linea + 1
                z_output.guardar_error(cc1)
        

    return ejecuciones




def carga_vector_int(fichero,cabecera=True):

    f = open(fichero,"r")
    if cabecera:
        next(f, None)
    reader = csv.reader(f)

    contenido = []
    for linea in reader:
        contenido.append(int(linea[0]))

    return np.array(contenido)   ##  TODO  Esto es necesario??    , dtype=np.int32) cambio insignificante

def carga_vector_int_parejaEstaciones(fichero,cabecera=False):

    f = open(fichero,"r")
    if cabecera:
        next(f, None)
    reader = csv.reader(f)

    contenido = []
    for linea in reader:
        contenido.append(int(linea[1]))

    return np.array(contenido)   ##  TODO  Esto es necesario??    , dtype=np.int32) cambio insignificante

def cargamatriz_int_Maribel(fichero,cabecera=True):

    f = open(fichero,"r")
    if cabecera:
        next(f, None)
    reader = csv.reader(f,delimiter=",")

    contenido = []
    #contador = 0;
    for linea in reader:
        #contador= contador+1
        #print("\tlinea:",contador, " contenido:",linea)
        if linea[0]=="":
            continue
        enteros_linea = []
        #contador2 = 0
        for x in linea:
            flotante = float(x)
            #print("dato:",contador2," valor",x)
            #contador2=contador2+1
            entero = math.ceil(flotante)
            enteros_linea.append(entero)
        contenido.append(enteros_linea)
        #contenido.append([int(x) for x in linea])

    return np.array(contenido)  ##  TODO   mirar esto , dtype=np.int32)

def cargamatriz_int(fichero,cabecera=True):

    f = open(fichero,"r")
    if cabecera:
        next(f, None)
    reader = csv.reader(f,delimiter=",")

    contenido = []
    for linea in reader:
        contenido.append([int(x) for x in linea])

    return np.array(contenido)  ##  TODO   mirar esto , dtype=np.int32)

def cargamatriz_float(fichero,cabecera=True):

    f = open(fichero,"r")
    if cabecera:
        next(f, None)
    reader = csv.reader(f)

    contenido = []
    for linea in reader:
        contenido.append([float(x) for x in linea])

    return np.array(contenido)   ## TODO , dtype=np.float32)


