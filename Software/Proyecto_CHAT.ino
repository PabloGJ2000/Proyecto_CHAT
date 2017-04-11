/* PROYECTO CHAT

    Proyecto CHAT by Pablo García Jaén (https://twitter.com/PabloGarciaJaen)
    is licensed under
    a Creative Commons Reconocimiento-Compartir Igual 4.0 Internacional License.
    Puede hallar permisos más allá de los concedidos con esta licencia en
    https://moaisenergy.com/acerca-de/

    Proyecto publicado en: https://moaisenergy.com/proyecto-chat/
*/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>                    //añadimos la librería para la pantalla LCD
#include <FabricaDigital_MCP23008.h>              //añadimos la librería para los extensores de pines MCP23008
#include <Keypad_MCP23008.h>                      //añadimos la librería para controlar el teclado con el extensor de pines MCP23008

LiquidCrystal_I2C lcd (0x3F, 16, 2);              //declaramos la dirección del I2C de la pantalla (0x3F) y las dimensiones de la pantalla (16,2)


/* ADVERTENCIA TECLADO

   El teclado está formado por 5 columnas y 9 filas y está dividido en 3 partes iguales.
   Todas las partes tienen 5 columnas (comunes para las 3 partes) pero solo tienen 3 filas.
   Cada una de las partes va conectado a un extensor de pines MCP23008 (ver conexiones en https://moaisenergy.com/proyecto-chat/ )
*/

const byte I2C_ADDRESS2 = 0x20;         //dirección del I2C ('0x2' porque es la dirección preestablecidad y '0' porque los pines A0, A1 y A2 están conectados a tierra por lo que crean 000 (en binario 0))
const byte ROWS2 = 3;                   //número de filas de 1/3 del teclado.
const byte COLS2 = 5;                   //número de columnas de 1/3 del teclado.
char keys2[ROWS2][COLS2] = {            //mapa de 1/3 del teclado. Concretamente del teclado superior.
  {'M', 'f', '_', 'k', '_'},            // | MODO | SHIFT |  ON | ARRIBA |  DER  |
  {'H', 'b', 'D', '_', 'p'},            // |  HEX |  BIN  | DEC |   IZQ  | ABAJO |
  {'c', 's', 't', 'L', 'l'}             // |  COS |  SEN  | TAN |   LOG  |   LN  |
};
byte rowPins2[ROWS2] = {5, 6, 7};         //pines del extensor de pines (MCP23008) donde van las filas.
byte colPins2[COLS2] = {4, 3, 2, 1, 0};   //pines del extensor de pines (MCP23008) donde van las columnas.
Keypad_MCP23008 keypad2 = Keypad_MCP23008(rowPins2, colPins2, ROWS2, COLS2);    //enlazamos el mapa de 1/3 del teclado con los pines correspondientes.


const byte I2C_ADDRESS1 = 0x21;         //dirección del I2C ('0x2' porque es la dirección preestablecidad y '1' porque los pines A0 y A1 están conectados a tierra y A2 está conectado a 5V por lo que crean 001 (en binario 1))
const byte ROWS1 = 3;                   //número de filas de 1/3 del teclado.
const byte COLS1 = 5;                   //número de columnas de 1/3 del teclado.
char keys1[ROWS1][COLS1] = {            //mapa de 1/3 del teclado. Concretamente del teclado del medio.
  {'e', 'r', 'R', '(', ')'},            // | potencia | 2RAIZ | xRAIZ |  (  |  )  |
  {'=', 'B', 'C', 'P', '_'},            // |     A    |   B   |   C   |  PI |  e  |
  {'7', '8', '9', 'd', 'a'}             // |     7    |   8   |   9   | DEL |  AC |
};
byte rowPins1[ROWS1] = {5, 6, 7};         //pines del extensor de pines (MCP23008) donde van las filas.
byte colPins1[COLS1] = {0, 1, 2, 3, 4};   //pines del extensor de pines (MCP23008) donde van las columnas.
Keypad_MCP23008 keypad1 = Keypad_MCP23008(rowPins1, colPins1, ROWS1, COLS1);    //enlazamos el mapa de 1/3 del teclado con los pines correspondientes.


const byte I2C_ADDRESS = 0x23;         //dirección del I2C ('0x2' porque es la dirección preestablecidad y '3' porque los pines A0 está conectado a tierra y A1 y A2 están conectado a 5V por lo que crean 011 (en binario 3))
const byte ROWS = 3;                   //número de filas de 1/3 del teclado.
const byte COLS = 5;                   //número de columnas de 1/3 del teclado.
char keys[ROWS][COLS] = {              //mapa de 1/3 del teclado. Concretamente del teclado inferior.
  {'4', '5', '6', 'x', '/'},           // | 4 | 5 |  6  |  x  | / |
  {'1', '2', '3', '+', '-'},           // | 1 | 2 |  3  |  +  | - |
  {'0', '.', 'E', 'y', '='}            // | 0 | . | EXP | ANS | = |
};
byte rowPins[ROWS] = {5, 6, 7};          //pines del extensor de pines (MCP23008) donde van las filas.
byte colPins[COLS] = {0, 1, 2, 3, 4};    //pines del extensor de pines (MCP23008) donde van las columnas.
Keypad_MCP23008 keypad = Keypad_MCP23008(rowPins, colPins, ROWS, COLS);    //enlazamos el mapa de 1/3 del teclado con los pines correspondientes.


//VARIABLES & CONSTANTES GLOBALES PARA CAMBIAR DE MODO.
byte modo = 1;                                  //variable donde almacenamos el modo en el que estamos. Siempre que se inicia la calculadora empezamos en el modo 1, es decir, en el modo calculadora.
//byte modoAnterior = 1;                          //variable donde almacenamos el modo en el que estabamos. De esta forma borramos la pantalla cada vez que cambiamos de modo.
//const int NUMERO_MODOS = 4;                     //cantidad de modos que tiene la calculadora.


//VARIABLES & CONSTANTES GLOBALES DEL MODO CALCULADORA CIENTÍFICA.
char botones;                                   //variable donde almacenamos el último botón leido.
//byte contadorNumeros = 0;                       //variable donde almacenamos la posición de cada número, es decir, en la operación 10+21, el número 10 está la posición 1 y el 21 está en la posición 2.

String stringOperacion = "(0+8)+(9+5)";
//String stringOperacion = "((4+7)+5)+(9+8)";
//String stringOperacion = "9+8+5";
//String stringOperacion = "(5+7)+(9+8)";
//String stringOperacion = "5+(((4+8)+7)+5)";

//String stringOperacionNV;                       //string donde se almacenan los caracteres pulsados. Esta variable es a partir de la cual se resuelve la operación.

float resultado;                                //variable tipo float (porque acepta decimales y almacena 4bytes). Aquí se almacenará el resultado una vez pulsamos '='.
//float resultadoMostrar;                         //variable tipo float (porque acepta decimales y almacena 4bytes). Una vez tengamos el resultado, esta variable será la que se muestre en la pantalla LCD.
//float ans;                                      //variable tipo float (porque acepta decimales y almacena 4bytes). Esta variable será la que llamaremos cuando pulsemos "Ans" que valdrá el último resultado obtenido.

unsigned long tiempoParpadeo;                   //variable donde almacenar el tiempo (en millis) transcurrido. Será utilizado para medir el tiempo de parpadeo que se produce en la posición donde vamos a escribir.
const int TIEMPO_PARPADEO = 1000;               //tiempo que tiene que pasar entre cada parpadeo.


//VARIABLES & CONSTANTES GLOBALES DEL MODO CALCULADORA MULTÍMETRO.
const byte PIN_OHMETRO = A0;                    //pin analógico donde medimos la resistencia.
unsigned long ohmios;                           //variable donde almacenar los ohmios de la resistencia.
boolean activadorOhmetro = 0;                   //variable para comprobar si el hay una resistencia en el ohmetro. Si hay resistencia esta variable valdrá 1. Si no hay, valdrá 0.

unsigned long tiempoReinicioOhms;               //variable donde almacenar el tiempo (en millis) transcurrido. Será utilizado para reiniciar los ohmios leídos.
const int TIEMPO_REINICIO_OHMS = 10000;         //tiempo que tiene que pasar entre cada lectura del ohmetro.


//VARIABLES & CONSTANTES GLOBALES DEL MODO CONFIGURACIÓN.
const int LUZ_LCD = 3;                          //pin digital PWM con el que vamos a poder cambiar el brillo del fondo de la pantalla LCD.
int luz_fondoLCD = 0;                           //variable donde almacenar el valor del pin PWM "LUZ_LCD". Este valor va desde 0 hasta 255, siendo (en este caso), 0 el maximo brillo y 255 el menor brillo.
int porcentaje_brillo = 100;                    //variable donde almacenar el porcentaje de brillo. Este porcentaje será mostrado en la pantalla LCD.


//VARIABLES GLOABLES PARA TODOS LOS MODOS.
float boton_A;                                  //variable donde podemos almacenar el número de ohmios medidos o cualquier otra cosa pulsando el bóton 'A'.
float boton_B;                                  //variable donde podemos almacenar el número de ohmios medidos o cualquier otra cosa pulsando el bóton 'B'.
float boton_C;                                  //variable donde podemos almacenar el número de ohmios medidos o cualquier otra cosa pulsando el bóton 'C'.

//FUNCIÓN PARA LEER LOS BOTONES.
void leerBotones() {
  char botonesA;                                //declaramos 1/3 del teclado
  char botonesB;                                //declaramos otro tercio del teclado
  char botonesC;                                //declaramos el último tercio del teclado
  botones = 0;                                  //ponemos la variable global de botones en 0. Así no nos interfiere la lectura anterior de los botones
  botonesA = keypad.getKey();
  botonesB = keypad1.getKey();
  botonesC = keypad2.getKey();
  if (botonesA) {
    botones = botonesA;
  }
  if (botonesB) {
    botones = botonesB;
  }
  if (botonesC) {
    botones = botonesC;
  }
}

float deRadianesAGrados(float a) {
  a = (a / 360) * 2 * PI;
  return a;
}



void GENERAL_distribucionBotones() {

}

void CALC_deBotonesAString() {
  stringOperacion = stringOperacion + botones;

  if (botones == '1') {
    stringOperacion = "((4+7)+5)+(9+8)";
  }
  if (botones == '2') {
    stringOperacion = "(9+7)+((4+7)+5)";
  }
  if (botones == '3') {
    stringOperacion = "(9+7)+(4+7)";
  }
  if (botones == '4') {
    stringOperacion = "4+(((5+8)+6)+3)";
  }

  //Serial.println(stringOperacion);
}





//boolean activadorMaximoContadorParentesis = 0;


const byte TAMANO_INICIO_FINAL_PARENTESIS = 10;
//byte inicioParentesis = 0;
//byte finalParentesis = 0;
byte generalInicioParentesis[TAMANO_INICIO_FINAL_PARENTESIS] = {0};
byte generalFinalParentesis[TAMANO_INICIO_FINAL_PARENTESIS] = {0};
//byte maximoContadorParentesisFinal = 0;
//byte maximoContadorParentesisInicio = 0;
boolean contadorInicioFinalParentesis = 0;
boolean activadorGeneralFinalParentesis = 0;
boolean activadorInicioParentesis = 0;
boolean activadorParentesis = 0;

byte contadorParentesis = 0;

const byte TAMANO_SUB_OPERACION = 2;
String subOperacion[TAMANO_SUB_OPERACION];
//byte contadorSubOperacion = 0;

//boolean activadorSustituirParentesis = 0;
//boolean comprobadorParentesis = 0;

//byte pruebaInicio;
//byte pruebaFinal;

//boolean activadorPrueba = 0;

boolean activadorTEST = 1;

void CALC_distribucionBotones() {
  if (botones == '=') {
    subOperacion[0] = stringOperacion;
    CALC_solucionarTotalOperacion();

  }
  else {
    CALC_deBotonesAString();
  }
}

void CALC_solucionarTotalOperacion() {
  Serial.println("[INICIO]CALC_solucionarTotalOperacion");
  //Serial.println(subOperacion[0]);
  subOperacion[1] = subOperacion[0];
  while (activadorTEST == 1) {
    if (CALC_detectarParentesis(subOperacion[1]) == 1) {
      //Serial.println ("ENTRA");
      while ((CALC_detectarParentesis(subOperacion[1])) == 1) {
        CALC_separarParentesis(subOperacion[1]);
      }
      if (CALC_detectarParentesis(subOperacion[1]) == 0) {
        //Serial.println("R");
        CALC_solucionarOperacion();
        CALC_sustituirParentesis();
        //Serial.println(CALC_detectarParentesis(subOperacion[0]));
        //Serial.println(subOperacion[1]);
        if (CALC_detectarParentesis(subOperacion[0]) == 0) {
          Serial.println("////");
          CALC_solucionarOperacion();
          activadorTEST = 0;
        }
        else {
          generalInicioParentesis[0] = 0;
          generalFinalParentesis[1] = 0;
          generalFinalParentesis[2] = 0;
          generalFinalParentesis[0] = 0;
          generalFinalParentesis[1] = 0;
          generalFinalParentesis[2] = 0;
          contadorInicioFinalParentesis = 0;
          activadorGeneralFinalParentesis = 0;
          activadorInicioParentesis = 0;
          activadorParentesis = 0;
          contadorParentesis = 0;
        }
      }
    }
    else {
      CALC_solucionarOperacion();
      activadorTEST = 0;
      Serial.println("NO ENTRA");
    }
  }

  Serial.println("[FINAL]CALC_solucionarTotalOperacion");
  return;
}

boolean CALC_detectarParentesis(String operacion) {
  Serial.println("CALC_detectarParentesis");
  Serial.println(operacion);
  byte longitudOperacion = operacion.length();
  //Serial.println("[INICIO]CALC_detectarParentesis");
  //Serial.println(operacion);

  for (byte i = 0; i < longitudOperacion; i++) {
    char caracter = operacion.charAt(i);
    if ((caracter == '(') or (caracter == ')')) {
      //Serial.println("[FINAL]CALC_detectarParentesis[1]");
      return 1;
    }
  }
  //Serial.println("[FINAL]CALC_detectarParentesis[0]");
  return 0;
}

void CALC_separarParentesis(String operacion) {
  Serial.println("[INICIO]CALC_separarParentesis");
  Serial.println(operacion);
  byte longitudOperacion = operacion.length();

  subOperacion[1] = operacion;
  for (byte i = 0; i < longitudOperacion; i++) {
    char caracter = operacion.charAt(i);
    if (caracter == '(') {
      if (activadorInicioParentesis == 0) {
        generalInicioParentesis[contadorInicioFinalParentesis] = i + 1;
        activadorInicioParentesis = 1;
        activadorParentesis = 1;
      }
      if (activadorGeneralFinalParentesis == 0) {
        generalInicioParentesis[2] = i;
      }
      activadorParentesis = 1;
      contadorParentesis++;
    }

    else if (caracter == ')') {
      generalFinalParentesis[contadorParentesis] = i;
      contadorParentesis--;
      if (activadorGeneralFinalParentesis == 0) {
        generalFinalParentesis[2] = i;
        activadorGeneralFinalParentesis = 1;
      }
    }
    if ((contadorParentesis == 0) && (activadorParentesis == 1)) {
      generalFinalParentesis[contadorInicioFinalParentesis] = i;
      activadorInicioParentesis = 0;
      activadorParentesis = 0;
      Serial.println("generalInicioParentesis[0]");
      Serial.println(generalInicioParentesis[0]);
      Serial.println(generalInicioParentesis[1]);
      Serial.println(generalInicioParentesis[2]);
      Serial.println("generalFinalParentesis[0]");
      Serial.println(generalFinalParentesis[0]);
      Serial.println(generalFinalParentesis[1]);
      Serial.println(generalFinalParentesis[2]);
      
      subOperacion[1] =  operacion.substring(generalInicioParentesis[contadorInicioFinalParentesis], generalFinalParentesis[contadorInicioFinalParentesis]);
      Serial.println(subOperacion[1]);
      contadorInicioFinalParentesis = 1;
      return;


    }
  }
  Serial.println("[FINAL]CALC_separarParentesis");
}

void CALC_solucionarOperacion() {
  byte longitudOperacion = subOperacion[1].length();
  float numeroActual = 0;
  float numeroAnterior = 0;
  byte contadorValores = 0;
  const byte TAMANOVALORES = 20;
  float valorNumerico[TAMANOVALORES] = {0};
  boolean activadorMenos = 0;
  
  for (byte i = 0; i < longitudOperacion; i++) {
    char caracter = subOperacion[1].charAt(i);
    if ((caracter >= 48) && (caracter <= 57)) {
      numeroActual = caracter - 48 + numeroAnterior * 10;
      if (activadorMenos == 1) {
        //numeroActual = 0 - numeroActual;
      }
      numeroAnterior = numeroActual;
      
      valorNumerico[contadorValores] = numeroActual;
    }
    else if (caracter == '-') {
      activadorMenos = 1;
    }
    else {
      numeroAnterior = 0;
      contadorValores++;
    }
  }
  for (byte i = 0; i < 10; i++) {
    Serial.print("numero =  ");
    Serial.println(valorNumerico[i]);
  }

  resultado = 0;
  for (int i = 0; i <= contadorValores; i++) {
    resultado += valorNumerico[i];
  }
  Serial.println(resultado);
  Serial.println("[FINAL]CALC_solucionarOperacion");
}




// Convierte un float en una cadena.
// n -> número a convertir.
// l -> longitud total de la cadena, por defecto 8.
// d -> decimales, por defecto 2.
// z -> si se desea rellenar con ceros a la izquierda, por defecto true.
String floatToString( float n, int l, int d, boolean z) {
  char c[l + 1];
  String s;

  dtostrf(n, l, d, c);
  s = String(c);

  if (z) {
    s.replace(" ", "0");
  }

  return s;
}





void CALC_sustituirParentesis() {
  Serial.println("[INICIO]CALC_sustituirParentesis");
  Serial.println(subOperacion[0]);
  String anteriorResultado;
  String posteriorResultado;
  anteriorResultado = subOperacion[0].substring(0, generalInicioParentesis[2]);
  posteriorResultado = subOperacion[0].substring(generalFinalParentesis[2] + 1);
  subOperacion[0] = anteriorResultado + floatToString(resultado, 5, 0, 1) + posteriorResultado;
  subOperacion[1] = subOperacion[0];
  Serial.println(subOperacion[0]);
  Serial.println("[FINAL]CALC_sustituirParentesis");
  return;
}

void CALC_variablesParentesisACero() {

}




void parpadeoLCD(byte x) {
  lcd.setCursor(x, 0);
  if (millis() - tiempoParpadeo <= TIEMPO_PARPADEO / 2) {
    lcd.print("_");
  }
  else if (millis() - tiempoParpadeo <= TIEMPO_PARPADEO) {
    lcd.print(" ");
  }
  else {
    tiempoParpadeo = millis();
  }
  if (botones == 'd') {
    lcd.print(" ");
  }
}

void CALC_escribirLCD() {
  byte valorx0;
  String stringOperacionVisual;
  stringOperacionVisual = stringOperacion;
  valorx0 = stringOperacionVisual.length();
  if (valorx0 >= 16) {
    stringOperacionVisual = stringOperacion.substring(valorx0 - 15);
  }

  lcd.setCursor(0, 0);
  lcd.print(stringOperacionVisual);
  lcd.setCursor(0, 1);
  lcd.print(resultado);

  byte xparpadeo;
  xparpadeo = stringOperacionVisual.length();
  parpadeoLCD(xparpadeo);
}

void leerResistencia() {

}

void escribirOhmetroLCD() {

}

void escribirConfiguracionLCD() {

}

void escribirBluetoothLCD() {

}

void setup() {
  Serial.begin(9600);
  keypad.begin(I2C_ADDRESS, makeKeymap(keys));
  keypad1.begin(I2C_ADDRESS1, makeKeymap(keys1));
  keypad2.begin(I2C_ADDRESS2, makeKeymap(keys2));

  pinMode(LUZ_LCD, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("MoaisEnergy");
  delay(1000);
  lcd.clear();

  tiempoParpadeo = millis();
  tiempoReinicioOhms = millis();


  //////////////////////////////////////////////////////


  subOperacion[0] = stringOperacion;
  CALC_solucionarTotalOperacion();//////////////////////////////////////////////////////////////////////////////



}

void loop() {
  analogWrite(LUZ_LCD, luz_fondoLCD);
  //recogemos datos
  leerBotones();

  //procesamos los datos
  GENERAL_distribucionBotones();
  switch (modo) {
    case 1: //MODO CALCULADORA
      if (botones) {
        CALC_distribucionBotones();
      }
      break;
    case 2:  //MODO OHMETRO
      leerResistencia();
      break;
    case 3:  //MODO BLUETOOTH

      break;
    case 4:  //MODO CONFIGURACIÓN

      break;
  }

  //sacamos los datos al exterior
  // if (modoAnterior != modo) {
  // lcd.clear();
  // }
  switch (modo) {
    case 1: //MODO CALCULADORA
      CALC_escribirLCD();
      break;
    case 2: //MODO OHMETRO
      escribirOhmetroLCD();
      break;
    case 3:  //MODO BLUETOOTH
      escribirBluetoothLCD();
      break;
    case 4:  //MODO CONFIGURACIÓN
      escribirConfiguracionLCD();
      break;
  }
}
