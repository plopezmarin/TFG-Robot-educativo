/*
LIBRERIA PARA LOS MOVIMIENTOS DEL COCHE
Esta librería incluye todos los movimientos/acciones que puede realizar el coche
*/

#ifndef accion_h
#define accion_h
 
#include "Arduino.h"
#include "accion_coche.h"
#include "pitches.h"

/*DEFINICION DE LAS CONSTANTES PARA LOS MOVIMIENTOS
Se usa simplemente para poder invocar las funciones usando el nombre de la acción
pero usando enteros para identificarlas ya que son más rápdios que un String*/ 
const int DELANTE = 1;
const int ATRAS = 2;

const int DERECHA = 1;
const int IZQUIERDA = 2;

//FUNCIONES
//Inicilizar los parámetros para que funcionen los motres
void inicializa_motores();

/*Estas dos funciones son las básicas para el control de los dos motores, permiten establecer el giro y la velocidad de cada motor*/
void control_motorA(char IN1, char IN2, long velocidad);
void control_motorB(char IN3, char IN4, long velocidad);

//new
void delante_ruedaA_ruedaB(int A, int B);
void atras_ruedaA_ruedaB(int A, int B);

//Avanzar hacia delante a la velocidad y el tiempo indicados
void avanza_delante(int velocidad, float tiempo);

//Retroceder a la velocidad y el tiempo indicados
void avanza_atras(int velocidad, float tiempo);

//detiene el giro de ambos motores
void frena_motores();

//control de motores para girar a la derecha
void gira_derecha(int velocidad, float tiempo);

//control de motores para girar a la izquierda
void gira_izquierda(int velocidad, float tiempo);

//ESTAS SON LAS FUNCIONES PARA EL USUARIO
//Para avanzar indicar sentido: DELANTE o ATRAS, velocidad: 0-100, tiempo en segundos
void avanza(int sentido, int velocidad, float tiempo);

//Para girar indicar sentido: IZQUIERDA o DERECHA, velocidad 0-100, tiempo en segundos
void gira(int direccion, int velocidad, float tiempo);

//Para frenar indicar tiempo en segundos (tiempo que este frenado)
void frena(float tiempo);

//Para tocar una nota indicar la nota (consultar libreria pitches.h) y la duración de esta
void toca_nota(int nota, float duracion);


#endif