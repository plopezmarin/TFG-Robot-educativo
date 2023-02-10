
#include <comunicacion_camara.h>


void setup() {
  // put your setup code here, to run once:
  inicializa_camara();
  //randomSeed(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool goal = false;
  bool flag_encontrado = false;
  char random_accion[2] = { 'R', 'L' };
  int objetivo[3] = { 0, 2, 3 };
  int obj_actual = 0;
  float distancia_tag;
  float posicion_x;
  int elementos;


  while (!goal) {

    struct tag_struct *array_TAG = detecta_tag(&elementos);
    //ordena_tags(arrayTAGs, n_tags, ID, ASCENDENTE);

    flag_encontrado = busca_id(array_TAG, objetivo[obj_actual], &distancia_tag, &posicion_x);
    free(array_TAG);

    if (!flag_encontrado) {
      int r_a = random(2);
      int r_v = random(50, 56);
      int r_t = (0.3 + rand() % 2);

      comunicacion(random_accion[r_a], r_v, r_t);  //QUITAR LA DE AVANZAR DEL ARRAY

      comunicacion('F', 55, 0.5);
    } else {  //Si encontrado
      //Alinear al robot con el tag
      if (posicion_x < -0.2) {
        comunicacion('L', 52, 0.1);
      } else if (posicion_x > 0.2) {
        comunicacion('R', 52, 0.1);
      }
      //avanzar hacia el tag
      comunicacion('F', 55, 2.5 * distancia_tag);
      buzzer(2489, 2);  //sonar buzzer

      comunicacion('B', 55, 2);
      obj_actual++;
      comunicacion('R', 60, 4);
      if (obj_actual > 3) goal = true;
    }
  }
  //free(array_TAG);
}
