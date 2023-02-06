/*
LIBRERIA PARA LA COMUNICACION CAMARA-COCHE

*/

#ifndef comunicacion_camara_h
#define comunicacion_camara_h
 
#include "Arduino.h"

//VARIABLES PARA LA COMUNICACION
const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION
const char EOA = '\x020';  //END OF ACTION

//SET-UP CAMARA Y PROTOCOLO
void inicializa_camara();

//Ordena el array de TAGS según ID o distancia y en orden ASCENDENTE o DESCENDENTE
void ordena_tags(tag_struct* arrayT, int longitud, int sortBy, int orden);

/*FUNCION PARA BUSCAR TAG
Indica si el tag indicado se ha encontrado y esta en el array de TAGS*/
bool busca_id(int id, float* distancia);

//Calculo del checksum basado en el metodo XOR
uint8_t Checksum(const byte *datos, size_t longitud);

/*FUNCION PARA ESTABLECER LA CONEXION
Envia el caracter 'SYN' por el puerto serie, para indicar al coche que se va a establecer la comunicacion
y activa el flag de conectado*/
void inicia_comunicacion(const int timeout);

/*FUNCION PARA FINALIZAR LA COMUNICACION
Esta función envia el caracter 'EOT' por el puerto serie, para indicar al coche que se va finalizar la comuicacion
también desactiva el flag de conectado*/
void termina_comunicacion(const int timeout);

//FUNCION PARA OBTENRER EL ACK DE RESPUESTA
int espera_ACK(int timeout);

/*FUNCION PARA OBTERNER EOA(Fin de accion)
Esta función espera el tiempo que dura la accion enviada + 100mseg */
int espera_EOA(int parametro2);

/*FUNCION PARA ENVIAR LA ORDEN
Primero escribe el caracter 'STX', para indicar al coche que lo que se envía es una orden
Segundo, envia la orden
Tercero, computa y envia el checksum
Finalmente envia el 'ETX' y espera el ACK*/
void envia_datos(byte* estruct_accion, int longitud, const int timeout);

/*HANDLER DE COMUNICACION
Comprueba con el flag conectado, si la conexion esta estblecida:
En caso afirmativo - Inicializa la estructura con la orden indicada por parametro y la envia
En caso negativo - Invoca a inicia_comunicacion*/
void comunicacion(char mov, int parametro1, float parametro2);

/*FUNCION QUE DETECTA LOS TAG DE UNA CAPTURA DE IMAGEN
Crea e inicializa un array dinámico con estucturas que incluyen la información relativa a cada tag detectado*/
void detecta_tag();


#endif