#!/usr/bin/env python
# coding: utf-8
import csv

import matplotlib.pyplot as plt
import os
import os.path
import sys
import numpy as np
import xlrd
import distutils.dir_util
from datetime import datetime
import pandas as pd


def generarResultados(config,resultados):

    print("*"*70)
    print("*"*70)
    print("")
    print("\t\tGENERANDO RESULTADOS")
    print("")
    print("*"*70)
    print("*"*70)

    cc = dict(config)
    for key in cc.keys():
        cc[key]=str(cc[key]).replace("\n","")
    del cc["CERCA"]
    del cc["CERCA_KM"]
    del cc["DELTAS"]
    del cc["TENDENCIA"]
    
    indi_dict = resultados["MEJOR_IND"].to_dict()
    #print(indi_dict,"***>>>>>>><<<<<<<<<")
    for key in indi_dict.keys():
        #print(key," = ",indi_dict[key])
        cc["MEJOR_INDIV_" + key] = str(indi_dict[key]).replace("\n","")
    
    #RESULTADOS["LISTA_IND"]

    # print("-"*80)
    # print(cc)
    # print("-"*80)
    if (cc["OUTPUT_FILE"] == None):
        cc["OUTPUT_FILE"] = "historico.csv"
    if os.path.exists(cc["PATHSALIDA"])==False:
        distutils.dir_util.mkpath(cc["PATHSALIDA"])
    mifichero = os.path.join(cc["PATHSALIDA"],cc["OUTPUT_FILE"])
    if os.path.exists(mifichero):
        df  = pd.read_csv(mifichero,sep=";")

        df.loc[len(df)] = cc
    else:
        df  = pd.DataFrame([cc])

    df.to_csv(mifichero,sep=";",index=False)

    path = crear_dir_detalles(config)
    grabar_todos(resultados["LISTA_IND"], config, path)
    generar_graficas(resultados["LISTA_IND"], "img", config, path)


## ## Lo primero abrir el log del historico y añadirle la ejecución.
    ## crear_dirs(cc,"")
    ## f = open(os.path.join(cc["PATHSALIDA"],"historico.output"), mode="a")
    ## 
    ## f.write(str(cc) + "\n")
    ## 
    ## 
    ## ## GRABAR LOS PARAMETROS 
    ## 
    ## for key in cc.keys():
    ##     print(key," = ",cc[key])
## 
    ## #print(config)
## 
    ## #cadena = [config["LINEAXLS"] ]
    ## 
    
    # z_output.crear_dirs(CONFIG,"img")
    # sufijo = str("BLOCAL_") + "_" + str(CONFIG["TIEMPO"])  + "_" + str(CONFIG["POR_SEGURIDAD"]) + "_" + \
    #          str(CONFIG["POR_ESTACIONES"]) + "_" + str(CONFIG["FACTOR_SOBRANTE"]) + "_" + \
    #          str(CONFIG["POR_CANT"]) + "_" + str(CONFIG["MOD_K"]) + "_" +str(CONFIG["VALOR_KM"]) + "_" + \
    #          str(CONFIG["MAX_EPOCAS"]) + "_" + \
    #          str(CONFIG["FACTOR_STRESS"]) + "_" + str(CONFIG["HIBRIDO"]) + "_" + str(CONFIG["MAX_ITERACIONES"]) + "_" + \
    #          str(CONFIG["SEMILLA"]) + "_" + str(CONFIG["PASO_VNS"]) + "_" + str(CONFIG["NUM_VECINOS"])
# 
    # CONFIG["PATHRESULTADO"] = os.path.join(CONFIG["PATHSALIDA"],sufijo)
    # Resta = IBase.CAPACIDAD - MEJOR_INDIVIDUO.CAPACIDAD
    # print("Resta de individuos", Resta)
    # z_output.grabar_mejor(MEJOR_INDIVIDUO, CONFIG,"BLOCAL")
    # z_output.grabar_todos(LISTA_INDIVIDUOS, CONFIG)
    # z_output.generar_graficas(LISTA_INDIVIDUOS,sufijo,CONFIG)
    # z_output.grabar_tiempoEjecucion(CONFIG)



    ## f.close()


def guardar_error(config):
    print("Guardando errores de linea: ",config["LINEAXLS"])




def crear_dir_detalles(config):
    cadena = str(config["PATHENTRADA"])
    intermedio =""
    if "enero" in cadena:
        intermedio = "enero"
    if "febrero" in cadena:
        intermedio = "febrero"
    if "marzo_estres" in cadena:
        intermedio = "marzo_estres"
    if "marzo_estres_v2" in cadena:
        intermedio = "marzo_estres_v2"
    if "marzo_reales" in cadena:
        intermedio = "marzo_reales"
    if "marzo" in cadena:
        intermedio = "marzo"
    if "abril" in cadena:
        intermedio = "abril"
    if "mayo" in cadena:
        intermedio = "mayo"
    if "junio" in cadena:
        intermedio = "junio"
    intermedio = intermedio+"_"+str(config["ALGORITMO"])+"_"+str(config["POR_ESTACIONES"])+"_"+str(config["POR_CANT"])+"_"+str(config["FACTOR_SOBRANTE"])+"_"+str(config["SEMILLA"])
    path = os.path.join(config["PATHSALIDA"],intermedio)
    distutils.dir_util.mkpath(path)
    return path




def grabar_todos(Lista, config, path):
    #fres = open(os.path.join(config["PATHSALIDA"],str(config["SEMILLA"])+os.path.sep,str(config["SEMILLA"])+"_all_best.csv"), mode='a+')
    fres = open(os.path.join(path+os.path.sep,str(config["SEMILLA"])+"_all_best.csv"), mode='a+')
    grabar = csv.writer(fres, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL,lineterminator="\n")
    cabecera = ("MES","FITNESS", "CAPACIDAD_TOTAL", "KM_EXTRA", "KM_TENDENCIA", "SOBRANTE",
                "VACIAS", "LLENAS", "BICIS TOTAL", "PERDIDOS","CAPACIDAD", "IBASE-CAPACIDADES_MEJOR")

    grabar.writerow(cabecera)

    for Mejor in Lista:
        cadena = str(Mejor.CAPACIDAD).replace("\n","")
        registro = (str(config["PATHENTRADA"])[32:39],  # aquí hay que arreglar este 32:39 porque si cambia el directorio no irá
                Mejor.FITNESS,
                Mejor.CAPACIDAD_TOTAL,
                Mejor.KM_EXTRA,
                Mejor.KM_TENDENCIA,
                Mejor.SOBRANTE,
                Mejor.VACIAS,
                Mejor.LLENAS,
                np.round(config["TOTALBICIS"] * (1 + config["POR_SEGURIDAD"]) * 1.0),
                Mejor.USUARIOSPERDIDOS,
                cadena,
                config["IBASE-CAPACIDADES_MEJOR"]
        )

        grabar.writerow(registro)
    fres.close()

# Experimentación con parámetros
def pintar(datosY, titulo, config, ejex = "", ejey = "",nombre ="", grabar=False):
    '''
    Pintar las gráficas que necesitemos
    :param lcos: dataset a pintar
    :param cont: número de CONFIG
    :param titulo: titulo de la gráfica
    :param grabar: flag para grabar en fichero o no
    '''

    plt.clf()
    plt.scatter(range(len(datosY)),datosY)   ### TODO NO LO VEO
    plt.xlabel(ejex)
    plt.ylabel(ejey)
    plt.ylim(bottom=0)
    plt.title("Experiment: " + titulo)
    if grabar:
        plt.savefig(nombre+".png")
    else:
        plt.show()

def generar_graficas(LISTA_INDIVIDUOS,sufijo, config, path):

    imagenes = os.path.join(path,sufijo)
    sufijo = "best"
    xx = [i.FITNESS for i in LISTA_INDIVIDUOS]
    pintar(xx, titulo="FITNESS_"+sufijo,config=config, ejex="Iterations", ejey="Fitness", nombre = imagenes + "_fitness_", grabar=True)

    xx = [i.KM_EXTRA for i in LISTA_INDIVIDUOS]
    pintar(xx, titulo="KMS_EXTRA_"+sufijo,config=config, ejex="Iterations", ejey="Extra kms", nombre = imagenes + "_extrakms_", grabar=True)

    xx = [i.KM_TENDENCIA for i in LISTA_INDIVIDUOS]
    # z_io.pintar(xx,titulo="KMS FICTICIOS_"+sufijo,config=CONFIG, grabar=True)
    pintar(xx, titulo="KMS_TEND_"+sufijo,config=config, ejex="Iterations", ejey="Extra Tends", nombre = imagenes + "_tendskms_", grabar=True)

    xx = [i.SOBRANTE for i in LISTA_INDIVIDUOS]
    # z_io.pintar(xx,titulo="SOBRANTE_"+sufijo,config=CONFIG, grabar=True)
    pintar(xx, titulo="SOBRA_"+sufijo,config=config, ejex="Iterations", ejey="Remaining", nombre = imagenes + "_sobra_", grabar=True)

    xx = [i.LLENAS for i in LISTA_INDIVIDUOS]
    # z_io.pintar(xx,titulo="LLENAS_"+sufijo,config=CONFIG,grabar=True)
    pintar(xx, titulo="FULL_"+sufijo,config=config, ejex="Iterations", ejey="Full stations", nombre = imagenes + "_fullstations_", grabar=True)

    xx = [i.VACIAS  for i in LISTA_INDIVIDUOS]
    # z_io.pintar(xx,titulo="LLENAS_"+sufijo,config=CONFIG,grabar=True)
    pintar(xx, titulo="EMPTY_"+sufijo,config=config, ejex="Iterations", ejey="Empty stations", nombre = imagenes + "_emptystations_", grabar=True)




