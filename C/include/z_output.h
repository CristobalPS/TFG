/***************************************************************************/
// CRISTÓBAL PÉREZ SIMÓN
// Fichero de cabecera asociado a las funciones de salida
/***************************************************************************/

#ifndef Z_OUTPUT_H
#define Z_OUTPUT_H

#include "z_Individuo.h"
#include "z_io.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Datos referentes a la salida
///////////////////////////////////////////////////////////////////////////
class Individuo_POO; // Declaración anticipada
struct Resultado{
    Individuo_POO * m_individuo; // Mejor individuo de la población

    vector<Individuo_POO> lista_individuos; // Mejor individuo de la población
};

/*************************************************************************/
/*************************************************************************/
// Generación de los resultados en un fichero
// Recibe
//      config: Fichero de configuración con los valores inicializados
//      resultados: Resultados de la ejecución

class Datos_EVAL; // Declaración anticipada
struct Resultado; // Declaración anticipada
void generarResultados(Datos_EVAL & config, Resultado & resultados, Individuo_POO & base);

/*************************************************************************/
/*************************************************************************/
// Guarda los errores de la ejecución

class Datos_EVAL; // Declaración anticipada
void guardar_error(Datos_EVAL & datos);

/*************************************************************************/
/*************************************************************************/
// Crea los detalles del directorio
//
// Recibe:
//      config: Archivo de configuración
// Devuelve:
//      Ruta del directorio creado

class Datos_EVAL; // Declaración anticipada
string crear_dir_detalles(Datos_EVAL & config);

/*************************************************************************/
/*************************************************************************/
// Guarda todos los detalles en un arhivo csv
// Recibe:
//      lista: Estructura con los individuos más relevantes
//      cc: Datos de configuración
//      rutaArchivo: Ruta del archivo de salida
class Datos_EVAL; // Declaración anticipada
struct Resultado; // Declaración anticipada
void grabar_todos(vector<Individuo_POO> & lista, Datos_EVAL & cc, string & rutaArchivo);


#endif