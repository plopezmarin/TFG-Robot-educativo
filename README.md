# mBot
TFG mBot


## SENSORES
  ### Ultrasonidos
  Para este caso considero crear una función que nos devueelva la distancia hasta el objeto, de este modo podrá usarse en condicionales y umbrales.
  
  ### Sigue-lineas
  Este hará uso de las funciones para las diferentes acciones. Básicamente si detecta que se sale hacia la izquierda *girar a la derecha* y viceversa, en caso de ir bien *avanzar*.
  Por lo que veo este tipo de sensor funciona devolviendo 0 si se sale y 1 si va por la línea. Lo hace de la siguiente forma:
  * Si se sale hacia la izquierda, devuelve: 01 (Por el lado izquierdo no detecta la línea, blanco; y por el dercho si).    
  * Simétrico para la dercha
  * Si esta fuera por completo, devuelve 00
  * Si esta en la línea, devuelve 11

        PSEUDOCODIGO
        
        FUNCION SEGUIR LINEA (Condicion finalizacion)
          SI (sigue-lineas == 01) Accion: *girar derecha*
          SI (sigue-lineas == 10) Accion: *girar izquierda*
          SI (sigue-lineas == 00) Accion: *Frenar*
          SI (sigue-lineas == 11) Accion: *Avanzar*
        //Condicion de finalizacion puede ser por tiempo o algun evento como que deja de encontrar linea (00)
        
        //OTRA OPCION: El usuario define este algoritmo, pero se le proporcionaría una abstracción de las funciones de lectura de los sensores y del control del motor, de tal modo que solo tenga que ver que devuelve el sensor y poner el sentido de giro.
    

## ACCIONES
FREANAR
    Básicamenete es poner ambos motores a 0V de tensión para que dejen de girar
    
AVANZAR DELANTE/ATRÁS
    Para avanzar debemos ajustar ambos motores con la misma potencia y la misma velocidad, para cambiar el sentido basta con cambiar el sentido de la corriente en amboss motores.
    
    PSEUDOCODIGO
    
    VOID AVANZAR (sentido, velocidad, tiempo)
      REPETIR *tiempo*  //ver como establecer cuanto avanzar, ya sea unidades de tiempo, revoluciones, distancia... 
        En funcion del *sentido* establecer el sentido de la corriente    //ambas ruedas igual
        Establecer la velocidad de ambas ruedas   //debe ser la misma
      
    
GIRAR DERECHA/IZQUIERDA
  Para girar basta con que una rueda gire más rápido que la otra ó incluso que giren en sentido contrario (así sería un angulo de giro más cerrado).
  Se pueden hacer dos acciones: girar sin desplazarse y girar desplazandose:
  
    PSEUDOCODIGO
    
    FUNCION GIRAR (sentido_giro, cuanto)
      REPETIR *cuanto*    //ver cuanto giramos: grados, tiempo...
        En funcioón del *sentido* establecer sentido de la corriente    //según se gire una rueda debe estar frenada (No corriente)
      
      
    FUNCION GIRAR AVANZANDO (sentido_giro, velocidad, cuanto)
      REPETIR *cuanto*
        En funcion del *sentido* establecer sentido de la corriente   //en este caso ambas ruedas deben girar porque también avanza
        Establecer la velocidad de ambas ruedas    // deben girar las dos pero una más rápido que la otra
      
GIRAR CAMARA

    FUNCION GIRAR CAMARA (sentido, cuanto)
      Establecer snetido de giro
      Girar lo que se indique *cuanto*

GRABAR VIDEO/ HACER FOTO

    FUNCION GRABAR VIDEO (duracion)
      Iniciar la grabacion cuando se invoque  
      Grabar tiempo indicado
      Devolver video

*Está la opción también de que el usuario pueda controlar los motores de forma independiente pudiendo ajustar la velocidad de cada uno de estos.
      
## MANDO
Para controlar el mBot, se pueden reutilizar las funciones mencionadas.


## LIBRERIA DE FUNCIONES
  * Lectura_ultrasonidos();
  * Lectura_sigueLineas();
  * Seguir_linea( parar );
  * Frenar ();
  * Avanzar (sentido, cuanto);
  * Girar (sentido, cuanto);
  * Girar Avanzando (Sentido, cuanto);
  * Control_motor (motor, velocidad);     //Controlar cada motor; el parametro motor, para indicar cual de los dos
  * Mover_camara(sentido, cuanto, velocidad);
  * Capturar_video();
  * Capturar_foto();
  * Encender led (color);

