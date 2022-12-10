const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION

struct accion_tag {
  char movimiento;
  int parametro1;
  //int parametro2;
} Accion_leida;



//IDENTIFICADORES DE TAREAS
TaskHandle_t Tarea1;
//TaskHandle_t Tarea2;

bool peligro = false;
bool timeout_flag = false;
volatile bool inicio_comunicacion = false;

// SEMAFOROS
SemaphoreHandle_t semaforo;
// ESTA FUNCION SIRVE PARA CREAR UN SEMAFORO:
void crea_semaforo() {
  semaforo = xSemaphoreCreateMutex();
  xSemaphoreGive(semaforo);  //lo libera
}

// BLOQUEO E.M.
void lock() {
  xSemaphoreTake(semaforo, portMAX_DELAY);
}

// DESBLOQUEO E.M.
void unlock() {
  xSemaphoreGive(semaforo);
}

const int DELANTE = 1;
const int ATRAS = 2;

const int DERECHA = 1;
const int IZQUIERDA = 2;

// SENSORES DE ULTRASONIDO
// Pines
const int trigger1 = 5;
const int echo1 = 18;
const int trigger2 = 17;
const int echo2 = 16;

// variables auxiliares
float distancia;
long tiempo;

int Trigpin = 0;
int Echopin = 0;

// SENSOR DE LUMINOSIDAD
int luminosidad = 0;
const int LUMIPin = 35;


// MOTORES
//Pines
const int IN1Pin = 26;
const int IN2Pin = 27;
const int IN3Pin = 14;
const int IN4Pin = 12;

//Canales
const int PWMChannelA = 0;
const int PWMChannelB = 1;

//GPIO
const int PWMPinA = 2;
const int PWMPinB = 15;

// Para controlar la velocidad usando PWM
const int freq = 1000;
const int resolution = 8;


//INTERRUPCION PARA INICIAR COMUNICACIÓN CON ESP32-CAM
// salta cuando el pin 19 se pone a 1
/*void IRAM_ATTR interrupcion_comunicacion() {
  inicio_comunicacion = true;
  Serial.print("comunicacion");
}*/

uint16_t Fletcher16(byte *datos, int cont) {
  uint8_t suma1 = 0;
  uint8_t suma2 = 0;

  for (size_t indice = 0; indice < cont; ++indice) {
    suma1 = suma1 + (uint8_t)datos[indice];
    suma2 = suma2 + suma1;
  }
  return (suma2 << 8) | suma1;
}

void establecimiento_conexion(byte *datos, const uint8_t longitud, const int timeout) {
  if (Serial.read() == SYN) {
    Serial.write(ACK);
    comunicacion_lectura(datos, longitud, timeout);
  }
  if (Serial.read() == EOT) inicio_comunicacion = false;
  ;
}

int comunicacion_lectura(byte *estruc_datos, uint8_t longitud_estructura, int timeout) {
  unsigned long inicio = millis();

  while (Serial.available() < (longitud_estructura + 4) && (millis() - inicio) < timeout)
    ;

  if (Serial.available() >= longitud_estructura + 4) {
    if (Serial.read() == STX) {
      //acabar esta parte
      Serial.readBytes(estruc_datos, longitud_estructura);
      char movimiento = Accion_leida.movimiento;
      switch (movimiento) {
        case 'F':
          avanza(DELANTE, Accion_leida.parametro1);
          break;
        case 'B':
          avanza(ATRAS, Accion_leida.parametro1);
          break;
        case 'L':
          gira(IZQUIERDA, Accion_leida.parametro1);
          break;
        case 'R':
          gira(DERECHA, Accion_leida.parametro1);
          break;
        case 'S':
          frena();
          break;
      }

      if (Serial.read() == ETX && Serial.read() | Serial.read() << 8 == Fletcher16(estruc_datos, longitud_estructura)) {
        Serial.write(ACK);
        return 0;
      }
      return -1;
    }
  }
  return 1;
}

// FUNCIONES BÁSICAS

// Funcion para el control de los motores
void control_motorA(char IN1, char IN2, long velocidad) {
  ledcWrite(PWMChannelA, velocidad);

  digitalWrite(IN1Pin, IN1);
  digitalWrite(IN2Pin, IN2);
}

void control_motorB(char IN3, char IN4, long velocidad) {
  ledcWrite(PWMChannelB, velocidad);

  digitalWrite(IN3Pin, IN3);
  digitalWrite(IN4Pin, IN4);
}


// FUNCIONES INTERMEDIAS, NOMBRE PROVISIONAL
void avanza_delante(int velocidad) {
  //velocidad = map(velocidad, 0, 255, 0, 100); //ajustar rango a 0-100
  velocidad = (velocidad * 255) / 100;
  control_motorA(HIGH, LOW, velocidad);
  //cambiar luego si eso, es asi por los pines
  control_motorB(LOW, HIGH, velocidad);
}

void avanza_atras(int velocidad) {
  velocidad = (velocidad * 255) / 100;
  //velocidad = map(velocidad, 0, 255, 0, 100); //ajustar rango a 0-100
  control_motorA(LOW, HIGH, velocidad);
  control_motorB(HIGH, LOW, velocidad);
}

void frena_motores() {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, LOW, 0);
}

void gira_derecha() {
  control_motorA(HIGH, LOW, 96);
  control_motorB(LOW, LOW, 0);
}

void gira_izquierda() {
  control_motorA(LOW, LOW, 0);
  control_motorB(HIGH, LOW, 96);
}

//Funcion para inicializar el controlador de los motores
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
}

//FUNCIONES PARA EL MOVIMIENTO DEL COCHE
// SENTIDO: DELANTE / ATRAS
// VELOCIDAD: RAPIDO / LENTO
void avanza(int sentido, int velocidad) {
  // NOTA: Para el mando se puede usar la funcion map()

  if (sentido == 1) {
    avanza_delante(velocidad);
  }

  if (sentido == 2) {
    avanza_atras(velocidad);
  }
}

//DIRECCION: IZQUIERDA / DERECHA
void gira(int direccion, int tiempo) {
  if (direccion == 1) {
    gira_derecha();
    delay(tiempo);
  }
  if (direccion == 2) {
    gira_izquierda();
    delay(tiempo);
  }
}


void frena() {
  frena_motores();
  delay(500);
}

//Funcion para inicializar el sensor de ultrasonidos
void inicializa_sensor_ultrasonidos(int trigpin, int echopin) {
  pinMode(trigpin, OUTPUT);  // Trigger pin = salida
  pinMode(echopin, INPUT);   // Echo pin = entrada
}

// FUNCIÓN PARA OBTENER EL VALOR DEL SENSOR DE ULTRASONIDOS
int lectura_ultrasonidos(int sensor) {
  if (sensor == 1) {
    Trigpin = trigger1;
    Echopin = echo1;
  } else if (sensor == 2) {
    Trigpin = trigger2;
    Echopin = echo2;
  }

  //limpiar el pin
  digitalWrite(Trigpin, LOW);
  delayMicroseconds(2);

  // Enviar pulsos cada 10 useg
  digitalWrite(Trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigpin, LOW);

  // Leer el pin echo, devuelve el tiempo de viaje de la onda en mseg
  tiempo = pulseIn(Echopin, HIGH);

  // Calcular la distancia
  distancia = tiempo / 59;
  return distancia;
}

// FUNCION PARA EL SENSOR DE LUMINOSIDAD
int lectura_luminosidad() {
  luminosidad = analogRead(LUMIPin);
  return luminosidad;
}

// TAREA PARA EL SENSOR DE ULTRASONIDOS, EN EL CORE 0
void tarea_ultrasonidos(void *parameter) {
  for (;;) {
    lock();
    if (lectura_ultrasonidos(1) < 20 /*|| lectura_ultrasonidos(2)<20*/) {
      //peligro = true;
      frena();
      while (lectura_ultrasonidos(1) < 20)
        ;
    }
    unlock();
    vTaskDelay(100);  //bloquea la tarea 100 ticks
  }
}

void setup() {
  Serial.begin(9600);

  //Inicializar SENSOR ULTRASONIDOS
  inicializa_sensor_ultrasonidos(trigger1, echo1);
  inicializa_sensor_ultrasonidos(trigger2, echo2);

  // Inicializar motores
  inicializa_motores();

  // Crear semaforo
  crea_semaforo();

  // CREACION DE LA TAREA PARA EL MULTIPROCESAMIENTO
  xTaskCreatePinnedToCore(
    tarea_ultrasonidos,   // Función para implemntar la tarea
    "TareaUltrasonidos",  //Nombre de la tarea
    10000,                // tamaño de la pila **ver que es
    NULL,                 // Parametro de entrada para la tarea, suele ser null
    1,                    // Prioridad de la tarea, 1 = que la del loop
    &Tarea1,              // Handle de la tarea
    0);                   // CORE donde se ejecuta


  //descomentar luego
  //pinMode(19, INPUT);
  //attachInterrupt(19, &interrupcion_comunicacion, RISING);

}


void loop() {
  lock();
  //avanza(DELANTE, 70);


  //if (inicio_comunicacion == true) {
  establecimiento_conexion((byte *)&Accion_leida, sizeof(Accion_leida), 1000);
   // vTaskDelay(500);
  //}


  unlock();
  vTaskDelay(5);
}