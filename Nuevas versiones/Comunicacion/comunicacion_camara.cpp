//esp32-camera
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER  // Has PSRAM
#include "camera_pins.h"

// apriltag
#include "apriltag.h"
#include "tag25h9.h"
#include "apriltag_pose.h"
#include "common/image_u8.h"
#include "common/zarray.h"

#include <algorithm>

const int ID = 0;
const int DISTANCIA = 1;
const int ASCENDENTE = 2;
const int DESCENDENTE = 3;

//FLAG que indica si la comunicación esta establecida
bool conectado = false;

apriltag_detector_t *td = apriltag_detector_create();	//Crear el detector
apriltag_family_t *tf = tag25h9_create();				//Crear la familia

//VARIABLES PARA LA COMUNICACION
const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION
const char EOA = '\x020';  //END OF ACTION

//int TAG = 0;
bool flag_fin_accion = true;

//prueba
int tagid = 0;

//Estructura para la información extraida de cada tag detectado
struct tag_struct {
  int identificador;  // IDENTIFICADOR DEL TAG
  double x;           // POSICION EN X DEL TAG
  double y;           // POSICION EN Y DEL TAG
  double z;           // POSICION EN Z DEL TAG (distancia)
} Info_tag;

//Estructura para la orden
struct accion_struct {
  char movimiento;
  int parametro1;
  float parametro2;
};

struct tag_struct *arrayTAGs;  //puntero al array de estructuras
int n_tags = 0;                //contador del numero de tags detectados

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
  if (flag_fin_accion) {
    Serial.write(SYN);
    int ack = espera_ACK(timeout);
    //caso ok
    if (ack == 0) {
      conectado = true;
    }
  }
}

//FUNCION PARA FINALIZAR LA COMUNICACIÓN
void termina_comunicacion(const int timeout) {
  Serial.write(EOT);  //End of Transmision
  int ack = espera_ACK(timeout);
  if (ack == 0) {
    conectado = false;
  }
}

//PARA CAPTAR EL ACK
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
  conectado = false;
  return -1;  //NO RESPUESTA EXCEDE TIMEOUT
}

//FUNCION para comprobar que se recibe la indicacion de que el coche ha terminado de ejecutar la accion
int espera_EOA(int parametro2) {
  int tiempo = (parametro2 * 1000) + 100;
  unsigned long inicio = millis();
  //esperamos recibir respuesta en lo que dura la accion
  while (!Serial.available() && (millis() - inicio) < tiempo)
    ;
  if (Serial.available()) {
    if (Serial.read() == EOA) return 0;
    else {
      //conectado = false;
      return 1;
    }
  }
  conectado = false;
  return -1;
}

//Envia la estructura por el puerto serie
void envia_datos(byte *estruct_accion, int longitud, const int timeout) {
  Serial.write(STX);
  Serial.write(estruct_accion, longitud);
  uint8_t checksum = Checksum(estruct_accion, longitud);
  Serial.write((byte)checksum);
  Serial.write(ETX);  //fin de trama

  int ack = espera_ACK(timeout);
  if (ack != 0) {
    termina_comunicacion(timeout);
  }
  flag_fin_accion = true;  //¿?¿?
}

//COMPROBAR INTEGRIDAD DE LOS DATOS CON CHECKSUM XOR
uint8_t Checksum(const byte *datos, size_t longitud) {
  uint8_t valor = 0;
  for (size_t i = 0; i < longitud; i++) {
    valor ^= (uint8_t)datos[i];
  }
  return ~valor;
}

//Handler de comunicacion
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

//Funcion para inicializar los parámetros de la cámara
void inicializa_camara() {
  Serial.begin(9600);
  flag_fin_accion = true;

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
  config.pixel_format = PIXFORMAT_GRAYSCALE;  //Escala de grises

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

  //struct apriltag_detector:
  td->nthreads = 1;
  td->quad_decimate = 1.0;
  td->quad_sigma = 0.8;
  td->refine_edges = 0;
  td->decode_sharpening = 0.4;
  td->debug = 0;
  
  //Inicia comunicación
  inicia_comunicacion(200);
}

void detecta_tag() {
  double x, y, z;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Fallo de captura");
    return;
  }

  //Estrutura para detector de la pose:
  apriltag_detection_info_t info;
  info.tagsize = 0.158;  //tamaño en metros - revisar
  info.fx = 150;
  info.fy = 150;
  info.cx = 80;  //(160x120) FRAMESIZE_QQVGA
  info.cy = 60;

  //image_u8_t *im = image_u8_create_stride(fb->width, fb->height, fb->width);
  image_u8_t im = {
    .width = fb->width,
    .height = fb->height,
    .stride = fb->width,
    .buf = fb->buf
  };

  zarray_t *detections = apriltag_detector_detect(td, &im);

//CREACION DEL ARRAY DE ESTRUCTURAS
  n_tags = zarray_size(detections);
  arrayTAGs = (struct tag_struct *)malloc(n_tags * sizeof(struct tag_struct));

  //RECORRER EL ARRAY - EXTRAER LOS DATOS Y ENVIARLOS EN UNA ESTRUCTURA
  //para el primero lo hacemos fuera del bucle, para poder comprobar luego que no hay elementos repetidos

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

  apriltag_detections_destroy(detections);

  //devolver buffer
  esp_camera_fb_return(fb);
}

//Indica si se ha encontrado el tag con el ID dado.
bool busca_id(int id, float *distancia) {
  //al estar ordenado el id puede estar como mucho en la posicion de su id

  for (int t = 0; t < n_tags; t++) {
    if (arrayTAGs[t].identificador == id) {
      *distancia=arrayTAGs[t].z;
      return true;
    }
  }
  return false;
}