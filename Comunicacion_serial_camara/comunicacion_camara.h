/*
LIBRERIA PARA LA COMUNICACION CAMARA-COCHE

*/

#ifndef comunicacion_camara_h
#define comunicacion_camara_h
 
#include "Arduino.h"
#include "pitches/pitches.h"	//notas musicales

#define BUZZER_PIN 15

//VARIABLES PARA LA COMUNICACION
const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION
const char EOA = '\x020';  //END OF ACTION

// Para usar como parámetros
const int ID = 0;
const int DISTANCIA = 1;
const int ASCENDENTE = 2;
const int DESCENDENTE = 3;

//ESTRUCTURA PARA LA INFORMACION DEL TAG
struct tag_struct {
	int identificador;  // IDENTIFICADOR DEL TAG
	double x;           // POSICION EN X DEL TAG
	double y;           // POSICION EN Y DEL TAG
	double z;           // POSICION EN Z DEL TAG (distancia)
};

//ESTRUCTURA PARA LA ORDEN
struct accion_struct {
	char movimiento;
	int parametro1;
	float parametro2;
};

//SET-UP CAMARA Y PROTOCOLO
void inicializa_camara();

//Ordena el array de TAGS según ID o distancia y en orden ASCENDENTE o DESCENDENTE
void ordena_tags(tag_struct* arrayT, int longitud, int sortBy, int orden);

/*FUNCION PARA BUSCAR TAG
Indica si el tag indicado se ha encontrado y esta en el array de TAGS
Adicionalemente también devuelve la distancia en el eje X y el eje Z del tag*/
bool busca_id(tag_struct* arrayTAGs, int id, float* distancia, float *x_pos);

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
int espera_EOA(float parametro2);

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
struct tag_struct *detecta_tag(int *n_tags_p);

/*FUNCION DEL BUZZER
Hace sonar el buzzer, según la nota (pitches.h), el tiempo indicado (segundos)*/
void buzzer(int nota, float duracion);

//Para tocar una melodia, se debe crear un array de enteros que incluya las notas que queremos tocar (consultar libreria pitches.h)
//la duracion de las notas y cuantas notas hay en el array
// un ejemplo de array seria: int melody[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
// para indicar el tamaño, podemos pasarlo directamente nostros o si no lo sabemos exacto podemos obtenerlo con la siguente formula
// cantidad = sizeof(array)/sizeof(array[0])
//void toca_melodia(int melodia[], int duracion, int cantidad);

#endif
