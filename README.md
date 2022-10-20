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

  #### PSEUDOCODIGO
  - SI (sigue-lineas == 01) *Accion: girar derecha*
  - SI (sigue-lineas == 10) *Accion: girar izquierda*
  - SI (sigue-lineas == 00) *Accion: Parar*
  - SI (sigue-lineas == 11) *Accion: avanzar*
    
    
  
  ### Cámara

## ACCIONES
  * Parar
  * Avanzar delante/atrás:
    * Velocidad
    * Cuanto avanzar
  * Girar izquierda/derecha:
    * Velocidad de giro
    * Cuanto girar

  * Girar cámara
  * Grabar o hacer foto
