#include "Arduino.h"
#include "accion_coche.h"
#include "sensor_coche.h"
#include "ESP32Tone.h"

// DEFINICIÓN DE LOS PARÁMETROS NECESARIOS PARA LA CONFIGURACIÓN DE LOS MOTORES
//Pines de entrada del controlador de motor
 int IN1Pin = 26;
 int IN2Pin = 27;
 int IN3Pin = 14;
 int IN4Pin = 12;

//Canales de PWM
 int PWMChannelA = 0;
 int PWMChannelB = 1;

//GPIO del que se obtiene la señal de PWM
 int PWMPinA = 2;
 int PWMPinB = 15;

// Parametros para controlar la velocidad usando PWM
 int freq = 1000;		//Hz
 int resolution = 8;	//bits

//FUNCION DE SETUP: Esta función incializa los motores
void inicializa_motores() {
  // Ponemos todos los pines como salida
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(IN3Pin, OUTPUT);
  pinMode(IN4Pin, OUTPUT);

  //Configurar la señal PWM
  ledcSetup(PWMChannelA, freq, resolution);
  ledcSetup(PWMChannelB, freq, resolution);

  // GPIO donde se obtiene la señal y el canal
  ledcAttachPin(PWMPinA, PWMChannelA);
  ledcAttachPin(PWMPinB, PWMChannelB);

  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, LOW, 0);
}

// FUNCIONES BAJO NIVEL: Control del sentido de giro y velocidad de los motores, por separado
void control_motorA(char IN1, char IN2, long velocidad) {
  ledcAttachPin(PWMPinA, PWMChannelA);

  velocidad = (velocidad * 255) / 100;  //convierte velocidad a escala 0 - 255
  ledcWrite(PWMChannelA, velocidad);

  digitalWrite(IN1Pin, IN1);
  digitalWrite(IN2Pin, IN2);
}

void control_motorB(char IN3, char IN4, long velocidad) {
  ledcAttachPin(PWMPinB, PWMChannelB);	//REconfigura el GPIO con el canal de PWM

  velocidad = (velocidad * 255) / 100;  //convierte velocidad a escala 0 - 255 (ciclo trabajo)
  ledcWrite(PWMChannelB, velocidad);	//modificar el ciclo de trabajo para variar la velocidad

  digitalWrite(IN3Pin, IN3);
  digitalWrite(IN4Pin, IN4);
}

void delante_ruedaA_ruedaB(int A, int B) {
  if (sensor_delantero.lectura_ultrasonidos() > 20) {
    control_motorA(HIGH, LOW, A);
    control_motorB(LOW, HIGH, B);
  } else frena_motores();
}

void atras_ruedaA_ruedaB(int A, int B) {
  if (sensor_trasero.lectura_ultrasonidos() > 20) {
    control_motorA(LOW, HIGH, A);
    control_motorB(HIGH, LOW, B);
  } else frena_motores();
}


/* FUNCIONES NIVEL MEDIO: invocan las de nivel bajo para controlar los motores e 
implementan el control del tiempo de giro y los choques (sensor ultrasonidos)*/
void avanza_delante(int velocidad, float tiempo) {
  delante_ruedaA_ruedaB(velocidad, velocidad);

  //avanza el tiempo indicado si no hay nada delante
  unsigned long inicio = millis();
  while (millis() - inicio < (tiempo * 1000)) {
    if (sensor_delantero.lectura_ultrasonidos() < 20) {
      break;
    }
  }
}

void avanza_atras(int velocidad, float tiempo) {
  atras_ruedaA_ruedaB(velocidad, velocidad);

  //avanza el tiempo indicado si no hay nada delante
  unsigned long inicio = millis();
  while (millis() - inicio < (tiempo * 1000)) {
    if (sensor_trasero.lectura_ultrasonidos() < 20) {
      break;
    }
  }
}

void frena_motores() {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, LOW, 0);
}

void gira_derecha(int velocidad, float tiempo) {
  control_motorA(HIGH, LOW, velocidad);
  control_motorB(LOW, LOW, 0);

  vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);  //delay del tiempo indicado en segundos
}

void gira_izquierda(int velocidad, float tiempo) {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, HIGH, velocidad);

  vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);  //delay del tiempo indicado en segundos
}


/*FUNCIONES DE NIVEL ALTO: Estas son las funciones que el usaurio usará rincipalmente.
Para cada una de ellas los parámteros son:
	SENTIDO: DELANTE / ATRAS
	VELOCIDAD: RAPIDO / LENTO
	TIEMPO: EN SEGUNDOS 							*/
void avanza(int sentido, int velocidad, float tiempo) {
  if (sentido == 1) {
    avanza_delante(velocidad, tiempo);
  }

  if (sentido == 2) {
    avanza_atras(velocidad, tiempo);
  }

  frena_motores();
}

//DIRECCION: IZQUIERDA / DERECHA
void gira(int direccion, int velocidad, float tiempo) {
  if (direccion == 1) {
    gira_derecha(velocidad, tiempo);
    frena_motores();
  }
  if (direccion == 2) {
    gira_izquierda(velocidad, tiempo);
    frena_motores();
  }
}

// Función para frenar, durante un tiempo indicado
void frena(float tiempo) {
  frena_motores();
  vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
}

void toca_nota(int nota, float duracion) {
  tone2(23, nota, duracion * 1000);
  delay(100);
}


//Poner luego como ejemplo
void toca_melodia(int melodia[], int duracion, int cantidad) {

  for (int nota_actual = 0; nota_actual < cantidad; nota_actual++) {
    toca_nota(melodia[nota_actual], duracion);
    delay(1000);
    Serial.println(melodia[nota_actual]);
  }
}
