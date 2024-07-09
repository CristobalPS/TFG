#!/usr/bin/env python
# coding: utf-8

import numpy as np
import random
from math import exp

import librerias.z_io as z_io
from   librerias.z_io import debug
import sys



class Individuo_POO:
    CAPACIDAD = np.array([])
    NUM_ESTACIONES = 0
    FITNESS = 0

    KM_EXTRA = 0
    KM_TENDENCIA = 0
    SOBRANTE = 0
    TENDENCIA_ENTRADA = 0
    TENDENCIA_SALIDA = 0

    CAPACIDAD_TOTAL = 0

    # TODO GUARDAR VACIOAS LLENAS POR ESTACION
    VACIAS = 0
    LLENAS = 0

    USUARIOSPERDIDOS = 0


    def __init__(self):
        """Constructor de clase Persona"""
        self.CAPACIDAD = np.array([])
        self.NUM_ESTACIONES = 0
        self.FITNESS = 0
        self.KM_EXTRA = 0
        self.KM_TENDENCIA = 0
        self.SOBRANTE = 0
        self.VACIAS = 0
        self.LLENAS = 0
        self.CAPACIDAD_TOTAL = 0
        self.USUARIOSPERDIDOS = 0

    # def __init__(self, base):
    #     """Constructor de clase Persona"""
    #     self.CAPACIDAD = base.copy()
    #     self.NUM_ESTACIONES = len(base)
    #     self.FITNESS = 0
    #     self.KM_EXTRA = 0
    #     self.KM_TENDENCIA = 0
    #     self.SOBRANTE = 0
    #     self.VACIAS = 0
    #     self.LLENAS = 0
    #     self.CAPACIDAD_TOTAL = self.CAPACIDAD.sum()

    def setCapacidad(self,BASE):
        self.CAPACIDAD = BASE.copy()
        self.NUM_ESTACIONES = len(BASE)
        self.CAPACIDAD_TOTAL = self.CAPACIDAD.sum()
        self.FITNESS = 0
        self.KM_EXTRA = 0
        self.KM_TENDENCIA = 0
        self.SOBRANTE = 0
        self.VACIAS = 0
        self.LLENAS = 0
        self.USUARIOSPERDIDOS = 0

    def copy(self):
        copia = Individuo_POO()
        copia.CAPACIDAD       = self.CAPACIDAD.copy()
        copia.NUM_ESTACIONES  = self.NUM_ESTACIONES
        copia.FITNESS         = self.FITNESS
        copia.KM_EXTRA        = self.KM_EXTRA
        copia.KM_TENDENCIA    = self.KM_TENDENCIA
        copia.SOBRANTE        = self.SOBRANTE
        copia.VACIAS          = self.VACIAS
        copia.LLENAS          = self.LLENAS
        copia.CAPACIDAD_TOTAL = copia.CAPACIDAD.sum()
        copia.USUARIOSPERDIDOS = self.USUARIOSPERDIDOS

        return copia


    def to_dict(self):
        copia = {}
        copia["CAPACIDAD"]        = self.CAPACIDAD.copy()
        copia["NUM_ESTACIONES"]   = self.NUM_ESTACIONES
        copia["FITNESS"]          = self.FITNESS
        copia["KM_EXTRA"]         = self.KM_EXTRA
        copia["KM_TENDENCIA"]     = self.KM_TENDENCIA
        copia["SOBRANTE"]         = self.SOBRANTE
        copia["VACIAS"]           = self.VACIAS
        copia["LLENAS"]           = self.LLENAS
        copia["CAPACIDAD_TOTAL"]  = self.CAPACIDAD.sum()
        copia["USUARIOSPERDIDOS"] = self.USUARIOSPERDIDOS

        return copia





    def __str__(self):
        # Este no estaba ya cadena=str(self.CAPACIDAD)+"\n"+self.FITNESS
        #cadena = "-" * 80 + "\n"
        cadena = "FITNESS:   " + str(self.FITNESS)
        cadena += "KMS TENDENCIA: " + str(self.KM_TENDENCIA) + "\n"
        cadena += "KMS EXTRA: " + str(self.KM_EXTRA) + "\n"
        cadena += "SOBRANTES: " + str(self.SOBRANTE) + "\n"
        #cadena += "\n[" + ", ".join([str(x) for x in self.CAPACIDAD]) + "]:"
        #cadena += "CAP TOTAL: " + str(self.CAPACIDAD_TOTAL) + "\n"
        #cadena += "LLENAS:    " + str(self.LLENAS) + "\n"
        #cadena += "VACIAS:    " + str(self.VACIAS) + "\n"
        #cadena += "US. PERDIDOS: " + str(self.USUARIOSPERDIDOS) + "\n"

        #cadena += "TENDENCIA_ENTRADA: " + str(self.TENDENCIA_ENTRADA) + "\n"
        #cadena += "TENDENCIA_SALIDA:  " + str(self.TENDENCIA_SALIDA) + "\n"


        return cadena

    def mutacion(self, tope_inferior, config):
        '''
        Generacion de un nuevo individuo, a partir del base, con un numero de cambios
        relativo al numero de estaciones y con una cantidad relativa de bicis movidas.
        :param base: individuo a mutar
        :param porcent_estaciones: porcentaje de movimientos
        :param porcent_cant: porcentaje de la cantidad en función
        :return:
        '''
        minivel = 0

        debug("\n", nivel=minivel)
        debug("-." * 20, nivel=minivel)
        debug("\n", nivel=minivel)

        debug("M  U  T  A  C I O O O N N N N N N NNNNNNNN", nivel=minivel)

        n_modificaciones = round(
            self.NUM_ESTACIONES * float(config['POR_ESTACIONES']))  # 1% de las estaciones se modifican
        if n_modificaciones == 0: n_modificaciones = 1

        debug("modificaciones = ", n_modificaciones, nivel=minivel)

        individuo = Individuo_POO()
        individuo.setCapacidad(self.CAPACIDAD)

        for _ in range(n_modificaciones):
            i = random.randint(0, self.NUM_ESTACIONES - 1)
            maxcantidad = round(
                individuo.CAPACIDAD[i] * config['POR_CANT'] +1 ) # modificamos como máximo esta cantidad
            debug("MAX_CANTIDAD = ", maxcantidad, nivel=minivel)

            cant = random.randint(-maxcantidad, maxcantidad)

            debug("CANTIDAD = ", cant, nivel=minivel)

            if cant != 0:
                # Si voy a restar por debajo de la capacidad mínimo, sumar
                # if (individuo.CAPACIDAD_TOTAL + cant) < ((tope_inferior/100 + 1) * config["TOTALBICIS"]):
                if (individuo.CAPACIDAD_TOTAL + cant) < (tope_inferior):
                    cant = -cant

                if (individuo.CAPACIDAD[i] + cant < 0):
                    individuo.CAPACIDAD[i] = 0
                else:
                    individuo.CAPACIDAD[i] += cant

        individuo.CAPACIDAD_TOTAL = individuo.CAPACIDAD.sum()
        # maribel lo quita individuo.evaluacion_individuo_app_2(config)

        return individuo

    def mutacion_extrema(self, tope_inferior, config, n_modificaciones):
        '''
        Generacion de un nuevo individuo, a partir del base, con un numero de cambios
        relativo al numero de estaciones y con una cantidad relativa de bicis movidas.
        :param base: individuo a mutar
        :param porcent_estaciones: porcentaje de movimientos
        :param porcent_cant: porcentaje de la cantidad en función
        :return:
        '''
        minivel = 0

        debug("\n", nivel=minivel)
        debug("-." * 20, nivel=minivel)
        debug("\n", nivel=minivel)

        debug("M  U  T  A  C I O O O N N N N N N NNNNNNNN", nivel=minivel)

        debug("modificaciones = ", n_modificaciones, nivel=minivel)

        individuo = Individuo_POO()
        individuo.setCapacidad(self.CAPACIDAD)

        for _ in range(n_modificaciones):
            i = random.randint(0, self.NUM_ESTACIONES - 1)
            maxcantidad = round(
                individuo.CAPACIDAD[i] * config['POR_CANT'] +1 ) # modificamos como máximo esta cantidad
            debug("MAX_CANTIDAD = ", maxcantidad, nivel=minivel)

            cant = random.randint(-maxcantidad, maxcantidad)

            debug("CANTIDAD = ", cant, nivel=minivel)

            if cant != 0:
                # Si voy a restar por debajo de la capacidad mínimo, sumar
                # if (individuo.CAPACIDAD_TOTAL + cant) < ((tope_inferior/100 + 1) * config["TOTALBICIS"]):
                if (individuo.CAPACIDAD_TOTAL + cant) < (tope_inferior):
                    cant = -cant

                if (individuo.CAPACIDAD[i] + cant < 0):
                    individuo.CAPACIDAD[i] = 0
                else:
                    individuo.CAPACIDAD[i] += cant

        individuo.CAPACIDAD_TOTAL = individuo.CAPACIDAD.sum()
        # Maribel lo quita individuo.evaluacion_individuo_app_2(config)

        return individuo

    def mutacion_variable(self, tope_inferior, config, n_modificaciones):
        '''
        Generacion de un nuevo individuo, a partir del base, con un numero de cambios
        relativo al numero de estaciones y con una cantidad relativa de bicis movidas.
        :param base: individuo a mutar
        :param porcent_estaciones: porcentaje de movimientos
        :param porcent_cant: porcentaje de la cantidad en función
        :return:  cadena.append( "[" + ", ".join([str(x) for x in self.CAPACIDAD]) + "]")
        '''
        minivel = 0

        debug("\n", nivel=minivel)
        debug("-." * 20, nivel=minivel)
        debug("\n", nivel=minivel)

        debug("M  U  T  A  C I O O O N N N N N N NNNNNNNN", nivel=minivel)

        debug("modificaciones = ", n_modificaciones, nivel=minivel)

        individuo = Individuo_POO(self.CAPACIDAD)

        for _ in range(n_modificaciones):
            i = random.randint(0, self.NUM_ESTACIONES - 1)
            maxcantidad = round(
                individuo.CAPACIDAD[i] * config['POR_CANT'] +1 ) # modificamos como máximo esta cantidad
            debug("MAX_CANTIDAD = ", maxcantidad, nivel=minivel)

            cant = random.randint(-maxcantidad, maxcantidad)

            debug("CANTIDAD = ", cant, nivel=minivel)

            if cant != 0:
                # Si voy a restar por debajo de la capacidad mínimo, sumar
                # if (individuo.CAPACIDAD_TOTAL + cant) < ((tope_inferior/100 + 1) * config["TOTALBICIS"]):
                if (individuo.CAPACIDAD_TOTAL + cant) < (tope_inferior):
                    cant = -cant

                if (individuo.CAPACIDAD[i] + cant < 0):
                    individuo.CAPACIDAD[i] = 0
                else:
                    individuo.CAPACIDAD[i] += cant

        individuo.CAPACIDAD_TOTAL = individuo.CAPACIDAD.sum()
        # Maribel lo quita individuo.evaluacion_individuo_app_2(config)

        return individuo


    def mutacion_conservativa(self,config, intercambios=None, maxcantidad=5):
        #random.seed(config["SEMILLA"])

        #  numero de cambios
        longitud = len(self.CAPACIDAD)
        if intercambios == None:
            intercambios = int(longitud/4)
            # intercambios = 2

        individuo = Individuo_POO()
        individuo.setCapacidad(self.CAPACIDAD)

        for _ in range(intercambios):
            # i = valor aleatorio entre 0 y longitud
            i = random.randint(0, longitud - 1)

            # j = valor aleatorio entre 0 y longitud
            j = random.randint(0, longitud - 1)

            cant = min([random.randint(0, maxcantidad), individuo.CAPACIDAD[i], individuo.CAPACIDAD[j]])

            # Intercambio de la cantidad y los valores
            individuo.CAPACIDAD[i] += cant
            individuo.CAPACIDAD[j] -= cant
            ## -----------------------------------------------

        return individuo


    def cruce(self,otroIndividuo, cap_max, cap_min, config):
        size = min(len(self.CAPACIDAD), len(otroIndividuo.CAPACIDAD))
        cxpoint1 = random.randint(1, size)
        longitud = random.randint(1, size - 1)

        # print(cxpoint1,longitud)

        padre1 = self.CAPACIDAD.copy()
        padre2 = otroIndividuo.CAPACIDAD.copy()

        Hijo1 = Individuo_POO()
        Hijo1.setCapacidad(self.CAPACIDAD)
        Hijo2 = Individuo_POO()
        Hijo2.setCapacidad(otroIndividuo.CAPACIDAD)

        for x in range(cxpoint1, (cxpoint1+longitud)):
            gen1 = padre1[x%size]
            gen2 = padre2[x%size]


            if (Hijo1.CAPACIDAD.sum() + (gen2-gen1) ) < cap_min \
                    or \
                (Hijo2.CAPACIDAD.sum() + (gen1-gen2)) < cap_min:
                #print("CRUCE: Generación de hijo por debajo del mínimo....")
                #print(Hijo1)
                #print(Hijo2)
                break

            Hijo1.CAPACIDAD[x%size] = padre2[x%size]
            Hijo2.CAPACIDAD[x%size] = padre1[x%size]

        # print(self)
        # print(Hijo1)
        # print(Hijo2)

        return Hijo1,Hijo2
        return None



    def cruceCincoPorCiento(self, otroIndividuo, cap_min):


        size = min(len(self.CAPACIDAD), len(otroIndividuo.CAPACIDAD))
        cxpoint1 = random.randint(1, size)
        longitud = random.randint(1, round((size - 1)*0.05))
        #print("Longitud del cruce:", longitud)

        # print(cxpoint1,longitud)

        padre1 = self.CAPACIDAD.copy()
        padre2 = otroIndividuo.CAPACIDAD.copy()

        Hijo1 = Individuo_POO()
        Hijo1.setCapacidad(self.CAPACIDAD)
        Hijo2 = Individuo_POO()
        Hijo2.setCapacidad(otroIndividuo.CAPACIDAD)

        for x in range(cxpoint1, (cxpoint1 + longitud)):
            gen1 = padre1[x % size]
            gen2 = padre2[x % size]

            if (Hijo1.CAPACIDAD.sum() + (gen2 - gen1)) < cap_min \
                or \
                (Hijo2.CAPACIDAD.sum() + (gen1 - gen2)) < cap_min:
                # print("CRUCE: Generación de hijo por debajo del mínimo....")
                # print(Hijo1)
                # print(Hijo2)
                break

            Hijo1.CAPACIDAD[x % size] = padre2[x % size]
            Hijo2.CAPACIDAD[x % size] = padre1[x % size]

        return Hijo1, Hijo2


    def evaluacion_individuo(self, config):
        '''
        Evaluación del individuo USANDO UNA APP móvil que le dice cuál es la estación libre más cercana.
        :param config:
        :return:
        '''
        cont_while=0;
        minivel = 0
        config["EVALUACIONES"] = config["EVALUACIONES"]  + 1
        i = 0
        self.LLENAS = 0
        self.VACIAS = 0
        ocupacion = np.zeros_like(self.CAPACIDAD)
        IDESTACIONES = range(len(config["DELTAS"][0]))#range(self.NUM_ESTACIONES)

        ### Acumuladores de costes
        coste_kms_extra = 0
        coste_kms_tendencia = 0

        self.TENDENCIA_ENTRADA = 0
        self.TENDENCIA_SALIDA = 0

        for linea_delta in range(len(config["DELTAS"])):
            #print("\nLinea_delta:",linea_delta, "-> deltas[75:85]:",config["DELTAS"][linea_delta])
            for est_Origen in IDESTACIONES:
                pendiente = config["DELTAS"][linea_delta, est_Origen] * config[   "FACTOR_STRESS"]  # numero de est_Origen a depositar
                nuevatendencia = config["TENDENCIA"][linea_delta, est_Origen]
                it_stacion = 0  # numero de orden (n) de la mas cercana
                idest = config["CERCA"][est_Origen, it_stacion]  # nombre de la n estacion mas cercana
                if pendiente == 0:
                    if ocupacion[est_Origen] == self.CAPACIDAD[est_Origen] or ocupacion[est_Origen] == 0:
                        mov_virtual = True
                        pendiente = nuevatendencia
                    else:
                        mov_virtual= False
                else:
                    mov_virtual = False

                if pendiente > (self.CAPACIDAD[est_Origen] - ocupacion[est_Origen]):
                    self.USUARIOSPERDIDOS += pendiente - (self.CAPACIDAD[est_Origen] - ocupacion[est_Origen])
                elif (ocupacion[est_Origen]+pendiente) < 0:
                    self.USUARIOSPERDIDOS -= pendiente + ocupacion[est_Origen] #### HHHH AQUI SERÍA += EN VEZ DE -=
                while pendiente != 0:
                    if pendiente > 0:  ### Vamos a METER est_Origen
                        if ocupacion[idest] + pendiente <= self.CAPACIDAD[idest]:  ## HAY HUECOS SUFICIENTES
                            movido = pendiente
                            pendiente = 0
                        else:  ## NO HAY HUECOS BASTANTES
                            #if(est_Origen==80 and linea_delta==172):
                            #        print("aquí empieza a generar:", pendiente)
                            movido = self.CAPACIDAD[idest] - ocupacion[idest]
                            pendiente = pendiente - movido

                        if not mov_virtual:
                            ocupacion[idest] = ocupacion[idest] + movido
                        else:
                            entrada_virt = True
                            if ocupacion[idest] == self.CAPACIDAD[idest]:
                                self.LLENAS += 1

                    else:  ### Vamos a SACAR est_Origen
                        if ocupacion[idest] + pendiente >= 0:  ## HAY SUFICIENTES
                            movido = -pendiente
                            pendiente = 0
                            #print("\t Ocupación[idest]=",ocupacion[idest])

                        else:  ## FALTAN BICIS
                            movido = ocupacion[idest]
                            pendiente = pendiente + movido

                        if not mov_virtual:
                            ocupacion[idest] = ocupacion[idest] - movido
                        else:
                            entrada_virt = False
                            if ocupacion[idest] == 0:
                                self.VACIAS += 1
                                # tendencia = abs(config["TENDENCIA"][linea_delta, est_Origen])  # TODO cambiar

                    aaaa = movido * config["CERCA_KM"][est_Origen, it_stacion]
                    if mov_virtual:
                        coste_kms_tendencia = coste_kms_tendencia + aaaa
                        if entrada_virt:
                            self.TENDENCIA_ENTRADA = self.TENDENCIA_ENTRADA + aaaa
                        else:
                            self.TENDENCIA_SALIDA = self.TENDENCIA_SALIDA + aaaa
                    else:
                        coste_kms_extra = coste_kms_extra + aaaa
                        #if aaaa > 0 :
                        #    print("\nKM: cercanas_kms[",est_Origen,",",it_stacion,"]:",config["CERCA_KM"][est_Origen, it_stacion], " movido:",movido, " se suman,",aaaa," a coste_kms_extra:",coste_kms_extra-aaaa)

                    # vamos a la siguiente
                    it_stacion += 1
                    # si no quedan huecos en ninguna estacion...
                    if it_stacion == self.NUM_ESTACIONES:
                        print("SOCORROOO... QUE NO ME CABE!!")
                        print("La línea delta es ", linea_delta, "-->", config["DELTAS"][linea_delta])
                        sys.exit(-1)
                        break
                    idest = config["CERCA"][est_Origen, it_stacion]
                    #print("Estación de destino:",idest, " consultada desde est_Origen:",est_Origen, " it_station:",it_stacion)

            debug("RESULTADO:        ", "\t".join([str(x) for x in ((self.CAPACIDAD - ocupacion))])[:], nivel=minivel)

        ## CALCULO DE FITNESS
        #bicis_total = np.round(config["TOTALBICIS"] * (1 + config["POR_SEGURIDAD"]) * 1.0)
        bicis_total = (config["TOTALBICIS"] * (1 + config["POR_SEGURIDAD"]) * 1.0)
        plazas_sobrantes = (self.CAPACIDAD_TOTAL - bicis_total) / bicis_total
        self.SOBRANTE = plazas_sobrantes  ##
        fitness = ((coste_kms_extra + coste_kms_tendencia) * config["VALOR_KM"] + self.SOBRANTE * config["FACTOR_SOBRANTE"])
        self.FITNESS = fitness
        self.KM_EXTRA = coste_kms_extra
        self.KM_TENDENCIA = coste_kms_tendencia
        print("INDIVIDUO:", self)
        # sys.exit(-1)

    def fila(self):
        cadena = []
        cadena.append( "[" + ", ".join([str(x) for x in self.CAPACIDAD]) + "]")
        cadena.append(str(self.CAPACIDAD_TOTAL))
        cadena.append(str(self.FITNESS))
        cadena.append(str(self.KM_EXTRA))
        cadena.append(str(self.KM_TENDENCIA))
        cadena.append(str(self.SOBRANTE))
        cadena.append(str(self.LLENAS))
        cadena.append(str(self.VACIAS))
        cadena.append(str(self.USUARIOSPERDIDOS))
        return cadena


