/**********************************************************************************
* LIBRERIA ESP32 - CAM
* DETECCION DE APRILTAGS, COMUNICACION DE ORDENES, BUZZER
**********************************************************************************/

//esp32-camera
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER  // Has PSRAM
#include "CameraPins/camera_pins.h"
#include "comunicacion_camara.h"

// apriltag-librerias
#include "apriltag.h"
#include "tag25h9.h"  //en caso de trabajar con otra familia, reemplazar
#include "apriltag_pose.h"
#include "common/image_u8.h"
#include "common/zarray.h"

#include <algorithm>

// Para usar como parámetros
/*const int ID = 0;
const int DISTANCIA = 1;
const int ASCENDENTE = 2;
const int DESCENDENTE = 3;*/

//FLAG que indica si la comunicación esta establecida
bool conectado = false;

apriltag_detector_t *td = apriltag_detector_create();  //Crear el detector
apriltag_family_t *tf = tag25h9_create();              //Crear la familia

//Estructura para la orden
/*struct accion_struct {
  char movimiento;
  int parametro1;
  float parametro2;
};*/

/*contador del numero de tags detectados - este es interno para los métodos de esta clase
  el usuario debe definirse uno en su sketch para el método de detecta_tag*/
int n_tags = 0;

//Ordena el array de TAGS, por ID o por distancia
void ordena_tags(tag_struct *arrayT, int longitud, int sortBy, int orden) {
  if (orden == ASCENDENTE) {
    if (sortBy == ID) {
      std::sort(arrayT, arrayT + longitud, [](const tag_struct &a, const tag_struct &b) {
        return a.identificador < b.identificador;
      });
    } else if (sortBy == DISTANCIA) {
      std::sort(arrayT, arrayT + longitud, [](const tag_struct &a, const tag_struct &b) {
        return a.x < b.x;
      });
    }

  } else if (orden == DESCENDENTE) {
    if (sortBy == ID) {
      std::sort(arrayT, arrayT + longitud, [](const tag_struct &a, const tag_struct &b) {
        return a.identificador > b.identificador;
      });
    } else if (sortBy == DISTANCIA) {
      std::sort(arrayT, arrayT + longitud, [](const tag_struct &a, const tag_struct &b) {
        return a.x > b.x;
      });
    }
  }
}

//FUNCION PARA ESTABLECER LA CONEXION
void inicia_comunicacion(const int timeout) {
  Serial.write(SYN);
  int ack = espera_ACK(timeout);

  if (ack == 0) {
    conectado = true;        //caso ok
  } else conectado = false;  //En caso de fallo
}

//FUNCION PARA FINALIZAR LA COMUNICACIÓN
void termina_comunicacion(const int timeout) {
  Serial.write(EOT);  //End of Transmision
  int ack = espera_ACK(timeout);
  if (ack == 0) {
    conectado = false;
  }
  conectado = false;
}

//RECEPCION DEL ACK
int espera_ACK(int timeout) {
  unsigned long inicio = millis();
  while (!Serial.available() && (millis() - inicio) < timeout)
    ;
  if (Serial.available()) {
    if (Serial.read() == ACK) return 0;  //TODO OK
    else {
      //conectado = false;
      return 1;  //ERROR DE ACK
    }
  }
  //conectado = false;
  return -1;  //NO RESPUESTA EXCEDE TIMEOUT
}

//FUNCION para comprobar que se recibe la indicacion de que el coche ha terminado de ejecutar la accion
int espera_EOA(float parametro2) {
  int tiempo = (parametro2 * 1000) + 150;
  unsigned long inicio = millis();
  //esperamos recibir respuesta en lo que dura la accion
  while (!Serial.available() && (millis() - inicio) < tiempo)
    ;
  if (Serial.available()) {
    if (Serial.read() == EOA) return 0;
    else {
      return 1;
    }
  }
  return -1;
}

//ENVIA LA ESTRUCTURA POR EL PUERTO SERIE
void envia_datos(byte *estruct_accion, int longitud, const int timeout) {
  Serial.write(STX);
  Serial.write(estruct_accion, longitud);
  uint8_t checksum = Checksum(estruct_accion, longitud);
  Serial.write((byte)checksum);
  Serial.write(ETX);  //fin de trama

  int ack = espera_ACK(timeout);
  if (ack < 0) {  //excede timeout
    termina_comunicacion(timeout);
  }
}

//COMPROBAR INTEGRIDAD DE LOS DATOS CON CHECKSUM XOR
uint8_t Checksum(const byte *datos, size_t longitud) {
  uint8_t valor = 0;
  for (size_t i = 0; i < longitud; i++) {
    valor ^= (uint8_t)datos[i];
  }
  return ~valor;
}

//HANDLER COMUNICACION
void comunicacion(char mov, int parametro1, float parametro2) {
  if (conectado) {
    accion_struct accion{ mov, parametro1, parametro2 };
    envia_datos((byte *)&accion, sizeof(accion), 200);
    int eoa = espera_EOA(parametro2);
    if (eoa != 0) termina_comunicacion(100);
  } else {
    inicia_comunicacion(200);
  }
}

//INICIALIZA PARAMETROS CÁMARA - librería camera_pins.h
void inicializa_camara() {
  Serial.begin(115200);  //Configura baudios puerto serie - mismo coche
  pinMode(15, OUTPUT);   //Buzzer -pin como salida

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;  //Escala de grises - para el algoritmo de detección

  config.frame_size = FRAMESIZE_QQVGA;  //Resolución más pequeña para obtener más velocidad de procesamiento
  config.fb_count = 1;

  //Inicializar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // AprilTAG
  apriltag_detector_add_family(td, tf);

  /*Inicializar objeto detector apriltag_detector:
  Se puede modificar la configuración para intentar mejorar resultados*/
  td->nthreads = 1;
  td->quad_decimate = 1.0;
  td->quad_sigma = 0.8;
  td->refine_edges = 0;
  td->decode_sharpening = 0.4;
  td->debug = 0;  //desactivado- no necesario, es preferible mayor velocidad

  //Inicia comunicación
  inicia_comunicacion(200);
}

/*FUNCION QUE OBTIENE UN ARRAY DE TAGS EN UN FOTOGRAMA
puntero a un array de estructuras tag_struct*/
struct tag_struct *detecta_tag(int *n_tags_p) {
  double x, y, z;  //variables para la posicion del tag

  camera_fb_t *fb = esp_camera_fb_get();  //captura de fotograma, alamcena datos en memoria, puntero fb
  if (!fb) {
    Serial.println("Fallo de captura");
    return NULL;
  }

  //Estrutura para detector de la pose:
  apriltag_detection_info_t info;
  info.tagsize = 0.158;  //tamaño en metros del tag
  info.fx = 131;
  info.fy = 131;
  info.cx = 80;  //(160x120) FRAMESIZE_QQVGA
  info.cy = 60;

  //parametros del frame
  image_u8_t im = {
    .width = fb->width,
    .height = fb->height,
    .stride = fb->width,
    .buf = fb->buf
  };

  //Obteer un array (zarray_t) con las detecciones
  zarray_t *detections = apriltag_detector_detect(td, &im);

  //Obtener el número de tags detectados
  *n_tags_p = zarray_size(detections);  //para el usuario
  n_tags = zarray_size(detections);     //para la propia clase

  //CREACION DEL ARRAY DE ESTRUCTURAS - tamaño dinámico
  tag_struct *arrayTAGs = (struct tag_struct *)malloc(n_tags * sizeof(struct tag_struct));

  //RECORRER EL ARRAY - EXTRAER LOS DATOS Y ENVIARLOS EN UNA ESTRUCTURA
  for (int i = 0; i < n_tags; i++) {
    apriltag_detection_t *det;  //ESTRUTURA QUE ALMACENA LA INFORMACIÓN RELATIVA A UN TAG
    apriltag_pose_t pose;       //ESTRUCTURA PARA LA INFORMACION DE LA POSE DE CADA TAG.

    zarray_get(detections, i, &det);  //EXTRAE DEL CONJUNTO DE DETECCIONES CADA TAG DETECTADO (enterder tag detectado como la estrucutra mencionada antes)

    info.det = det;                                //ASOCIAMOS EL TAG DETECTADO A LA ESTRUCTURA CON LOS DATOS DE LA CAMARA PARA EL CALCULO DE LA POSE
    double err = estimate_tag_pose(&info, &pose);  //ESTIMACION DE LA POSE -> OBTENEMOS DATOS EN LA ESTRUCUTURA DE LA POSE

    /*AHORA EXTRAEMOS TODA LA INFORMACIÓN DE LOS TAGS, PARA EL CASO LO QUE MÁS N0S INTERESA ES:
    EL ID, LOS DATOS DE LA POSE (X, Y, Z) */
    arrayTAGs[i].identificador = det->id;
    arrayTAGs[i].x = pose.t->data[0];
    arrayTAGs[i].y = pose.t->data[1];
    arrayTAGs[i].z = pose.t->data[2];
  }

  apriltag_detections_destroy(detections);  //liberar la memoria del array creado

  //devolver buffer
  esp_camera_fb_return(fb);
  return arrayTAGs;
}

//INDICA SI EL TAG CON ID INDICADO ESTA EN LAS DETECCIONES O NO
bool busca_id(tag_struct *arrayTAGs, int id, float *distancia, float *x_pos) {
  //al estar ordenado el id puede estar como mucho en la posicion de su id
  for (int t = 0; t < n_tags; t++) {
    if (arrayTAGs[t].identificador == id) {
      //si está - devuelve la distancia en el eje x y el eje z
      *distancia = arrayTAGs[t].z;
      *x_pos = arrayTAGs[t].x;
      return true;
    }
  }
  return false;
}

//HACE SONAR EL BUZZER
void buzzer(int nota, float duracion) {
  tone(BUZZER_PIN, nota, duracion * 1000);
}