

#include <Servo.h>

Servo muneca;
Servo indice;                  //Declaración de servos
Servo anular;

int redPin = 5;
int greenPin = 6;              //ASIGNACION DE PINES PARA LED RGB INDICADOR             
int bluePin = 9;

int sensor = 0;

int posmun = 0;                //Posicion actual de rotación de la muñeca

int posminind = 180;           //Posiciones minimas y maximas de servos para dedos indice y medio
int posmaxind = 0;

int posminanu = 0;             //Posiciones minimas y maximas de servos para dedos anular y meñique
int posmaxanu = 180;

const int numReadings = 10;     //numerode lecturas a promediar

int readings[numReadings];      // array de lecturas
int readIndex = 0;              // indice de lectura actual
int total = 0;                  // total de lecturas

void setup() {
  
  pinMode(A3, INPUT_PULLUP);                   //ASIGNACION DE PIN DE ENTRADA PARA SENSOR
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);                   //ASIGNACION DE PINES PARA LED RGB INDICADOR
  pinMode(bluePin, OUTPUT);

  muneca.attach(4);
  indice.attach(2);                            //ASIGNACION DE PINES DE SALIDA PARA SERVOS
  anular.attach(3);

  Serial.begin(115200);

}

void loop() {

  total = total - readings[readIndex];        //Se toma el promedio de 10 muestras de la señal para tener una lectura estable
  readings[readIndex] = analogRead(A3);
  total = total + readings[readIndex];                   
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }



  sensor = map((total / numReadings), 1023, 0, 0, 10023);   //Se amplifica la señal 10 veces para hacer mas facil el filtrado
  Serial.println(sensor);                                   //Reporta el promedio calculado por serial
  

  if (sensor < 500 ) {                      //Cuando la lectura del sensor es menor a 500 los dedos se extienden 
                                            //y la muñeca queda en la ultima posición registrada
                                            
    indice.write(posminind);               //POSICIONAMIENTO DE LOS DEDOS Y MUÑECA
    anular.write(posminanu);
    muneca.write(posmun);
    
    analogWrite(redPin, 0);                //INDICADOR LED RGB EN VERDE 
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
    
    delay(50);
  }

  if (sensor > 500 && sensor < 2000) {         //Cuando la lectura del sensor esta entre 500 y 3000 
                                               //se acciona el servo correspondiente a los dedos indice y medio

    indice.write(posmaxind);
    anular.write(posminanu);                     //POSICIONAMIENTO DE LOS DEDOS 
    muneca.write(posmun);
    
    Serial.println("INDICE_MEDIO");                 //REPORTE DEL ESTADO POR SERIAL
     
    analogWrite(redPin, 0);
    analogWrite(greenPin, 150);                    //INDICADOR LED RGB EN AZUL ARTICO
    analogWrite(bluePin, 255);
    
    delay(20);
  }


  if (sensor > 2300 && sensor < 4500) {        //Cuando la lectura del sensor esta entre 3500 y 6500 
                                               //se acciona el servo correspondiente a los dedos anular y meñique

    anular.write(posmaxanu);
    indice.write(posmaxind);                   //POSICIONAMIENTO DE LOS DEDOS 
    muneca.write(posmun);
    
    Serial.println("ANULAR_MEÑIQUE");          //REPORTE DEL ESTADO POR SERIAL
    
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);                   //INDICADOR LED RGB EN AZUL PERSIA
    analogWrite(bluePin, 255);
    
    delay(20);
  }

  if (sensor > 5000 && sensor < 7500) {        //Cuando la lectura del sensor esta entre 8000 y 9000 
                                               //se libera la tension en los dedos y la muñeca gira a la derecha,
                                               //al detectar una lectura menor se ejecuta el filtro anterior y queda la muñeca estatica
                                               //al detectar una lectura mayor se ejecuta el filtro siguiente y gira la muñeca a la izquierda 

    indice.write(posminind);
    anular.write(posminanu);                   //POSICIONAMIENTO DE LOS DEDOS Y GIRO DE MUÑECA
    constrain(posmun, 0, 90);
    posmun -= 5;
    muneca.write(posmun);
    
    Serial.println("GIRO_DERECHA");             //REPORTE DEL ESTADO POR SERIAL
    
    analogWrite(redPin, 255);
    analogWrite(greenPin, 165);                 //INDICADOR LED RGB EN AMARILLO
    analogWrite(bluePin, 0);
    
    delay(5);
  }
  
  if (sensor > 7500 ) {                        //Cuando la lectura del sensor esta entre 8000 y 9000 
                                               //se libera la tension en los dedos y la muñeca gira a la izquierda,
                                               //al detectar una lectura menor se ejecuta el filtro anterior y gira la muñeca a la izquierda
                                               //al detectar una lectura mayor, la muñeca llega al punto maximo de giro y se detiene
    indice.write(posminind);
    anular.write(posminanu);                  //POSICIONAMIENTO DE LOS DEDOS Y GIRO DE MUÑECA
    constrain(posmun, 90, 180);
    posmun += 5;
    muneca.write(posmun);
    
    Serial.println("GIRO_IZQUIERDA");        //REPORTE DEL ESTADO POR SERIAL
     
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);               //INDICADOR LED RGB EN ROJO
    analogWrite(bluePin, 0);
    
    delay(5);
  }

  delay(5);
}
