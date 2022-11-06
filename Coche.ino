const int RAPIDO = 192;
const int LENTO = 96;

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


// FUNCIONES BÁSICAS

// Funcion para el control de los motores
void control_motorA(char IN1, char IN2, int velocidad) {
  ledcWrite(PWMChannelA, velocidad);

  digitalWrite(IN1Pin, IN1);
  digitalWrite(IN2Pin, IN2);
}

void control_motorB(char IN3, char IN4, int velocidad) {
  ledcWrite(PWMChannelB, velocidad);

  digitalWrite(IN3Pin, IN3);
  digitalWrite(IN4Pin, IN4);
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
void avanza(int sentido, int velocidad) {
  // NOTA: Para el mando se puede usar la funcion map()

  if (sentido == 1) {
    control_motorA(HIGH, LOW, velocidad);
    control_motorB(LOW, HIGH, velocidad);
  }

  if (sentido == 2) {
    control_motorA(LOW, HIGH, velocidad);
    control_motorB(HIGH, LOW, velocidad);
  }
}

void gira(int dirección){
  if (direccion == 1){
    control_motorA(HIGH, LOW, 96);
    control_motorB(LOW, LOW, 0);
  }
  if (direccion == 2){
    control_motorA(LOW, LOW, 0);
    control_motorB(HIGH, LOW, 96);
  }
}


void frena() {
    control_motorA(LOW, LOW, 0);
    control_motorB(LOW, LOW, 0);
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
  return luminosidad
}

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);

  //Inicializar SENSOR ULTRASONIDOS
  inicializa_sensor_ultrasonidos(trigger1, echo1);
  inicializa_sensor_ultrasonidos(trigger2, echo2);

  // Inicializar motores
  inicializa_motores();
}

void loop() {


  while (lectura_ultrasonidos(1) > 20) {
    //Serial.println(lectura_ultrasonidos(1));
    avanza(DELANTE, LENTO);
  }


  frena();
  //delay(100);
  while (lectura_ultrasonidos(2) > 20) {
    //Serial.println(lectura_ultrasonidos(2));
    avanza(ATRAS, LENTO);
  }
  frena();

  //delay(50);
}
