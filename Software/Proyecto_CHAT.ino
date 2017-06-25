/* PROYECTO CHAT
    Hecho por: Pablo García Jaén - https://twitter.com/PabloGarciaJaen -

    **Última modificación: 20/06/2017
    **Última modificación hecha por: Pablo García Jaén

    Proyecto CHAT is licensed under
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


/* INFORMACIÓN SOBRE EL TECLADO

   El teclado está formado por 5 columnas y 9 filas y está dividido en 3 partes iguales.
   Todas las partes tienen 5 columnas (comunes para las 3 partes) pero solo tienen 3 filas.
   Cada una de las partes va conectado a un extensor de pines MCP23008 (ver conexiones en https://moaisenergy.com/proyecto-chat/ )
*/

const byte I2C_ADDRESS2 = 0x20;         //dirección del I2C ('0x2' porque es la dirección preestablecidad y '0' porque los pines A0, A1 y A2 están conectados a tierra por lo que crean 000 (en binario 0))
const byte ROWS2 = 3;                   //número de filas de 1/3 del teclado.
const byte COLS2 = 5;                   //número de columnas de 1/3 del teclado.
char keys2[ROWS2][COLS2] = {            //mapa de 1/3 del teclado. Concretamente del teclado superior.
  {'M', 'f', '_', 'k', 'v'},            // | MODO | SHIFT |  ON | ARRIBA |  DER  |
  {'H', 'b', 'D', 'u', 'p'},            // |  HEX |  BIN  | DEC |   IZQ  | ABAJO |
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
  {'A', 'B', 'C', 'P', 'n'},            // |     A    |   B   |   C   |  PI |  e  |
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

String stringOperacion; // = "s-60xc-120";
//String stringOperacionLCD;
//String stringOperacion = "((4+7)+5)+(9+8)";
//String stringOperacion = "9+8+5";
//String stringOperacion = "(5+7)+(9+8)";
//String stringOperacion = "5+(((4+8)+7)+5)";

//String stringOperacionNV;                       //string donde se almacenan los caracteres pulsados. Esta variable es a partir de la cual se resuelve la operación.

float resultado;                                //variable tipo float (porque acepta decimales y almacena 4bytes). Aquí se almacenará el resultado una vez pulsamos '='.
//float resultadoMostrar;                         //variable tipo float (porque acepta decimales y almacena 4bytes). Una vez tengamos el resultado, esta variable será la que se muestre en la pantalla LCD.
//float ans;                                      //variable tipo float (porque acepta decimales y almacena 4bytes). Esta variable será la que llamaremos cuando pulsemos "Ans" que valdrá el último resultado obtenido.




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


boolean activadorRadianes = 0;


//SIGNOS Y OPERACION
  const char MULTIPLICACION = 'x';
  const char DIVISION = '/';
  const char SUMA = '+';
  const char RESTA = '-';
  const char POTENCIA = 'e';
  const char RAIZ_CUADRADA = 'r';
  const char RAIZ_X = 'R';
  const char LOGARITMO_BASE_10 = 'L';
  const char LOGARITMO_NEPERIANO = 'l';

  const char SEN = 's';
  const char COS = 'c';
  const char TAN = 't';

  const char ARCOSENO = 'S';
  const char ARCOCOSENO = 'F';
  const char ARCOTANGENTE = 'T';

  const char NUMERO_PI = 'P';
  const char NUMERO_e = 'n';

  const char PARENTESIS_ABRIR = '(';
  const char PARENTESIS_CERRAR = ')';

  const char ARRIBA = 'k';
  const char ABAJO = 'p';
  const char DERECHA = 'v';
  const char IZQUIERDA = 'u';

  const char SHIFT = 'f';

  const char BORRAR_AC = 'a';


byte RAIZ_CARACTER[8] = {                  
  0b00111,
  0b00100,
  0b00100,
  0b00100,   
  0b10100, 
  0b01100,   
  0b00100,   
  0b00000   
};

byte RAIZ_DE_X_CARACTER[8] = {                  
  0b11001,
  0b00110,
  0b01100,
  0b10011,   
  0b00000, 
  0b00000,   
  0b00000,   
  0b00000   
};

byte NUMERO_PI_CARACTER[8] = {                  
  0b00000,
  0b11111,
  0b01010,
  0b01010,   
  0b01010, 
  0b01011,   
  0b10010,   
  0b00000   
};

byte NUMERO_E_CARACTER[8] = {         
  0b00000,
  0b00110,
  0b01001,
  0b11010,
  0b11100,   
  0b11001, 
  0b01110,     
  0b00000   
};

byte ELEVADO_MENOS_UNO_CARACTER[8] = {
  0b00011,
  0b00001,
  0b11101,
  0b00001,
  0b00001,   
  0b00000, 
  0b00000,     
  0b00000  
};

//FUNCIÓN PARA LEER LOS BOTONES.
void leerBotones() {
  char botonesA;                                //declaramos 1/3 del teclado
  char botonesB;                                //declaramos otro tercio del teclado
  char botonesC;                                //declaramos el último tercio del teclado
  botones = 0;                                  //ponemos la variable global de botones en 0. Así no nos interfiere la lectura anterior de los botones
  botonesA = keypad.getKey();                   //asociamos el primer tercio del teclado con la variable botonesA
  botonesB = keypad1.getKey();                  //asociamos el segundo tercio del teclado con la variable botonesB
  botonesC = keypad2.getKey();                  //asociamos el tercer tercio del teclado con la variable botonesC
  if (botonesA) {                               //si botonesA tiene algún dato disponible... es decir, si hemos pulsado un botón del primer tercio del teclado
    botones = botonesA;                         //guardamos en la variable global de los botones el valor obtenido del primer tercio del teclado (en caso de que haya algún dato disponible)
  }
  if (botonesB) {                               //lo mismo pero con el segundo tercio del teclado
    botones = botonesB;
  }
  if (botonesC) {                               //lo mismo pero con el tercer tercio del teclado
    botones = botonesC;
  }
}

float deRadianesAGrados(float a) {              //función para pasar de radianes a grados
  a = (a / 360) * 2 * PI;
  return a;
}



void GENERAL_distribucionBotones() {             //función por si uno de los botones que hemos pulsado es para cambiar de modo

}


byte xparpadeo = 0;
byte xparpadeoLCD = 0;

void CALC_deBotonesAString() {                   //función para guardar el caracter del botón pulsado (por ejemplo '2') en un string. De esta forma podremos borrar un carácter si nos confundimos
  String a = stringOperacion;
  String b = stringOperacion;
  Serial.println("WIIG");
  if ((botones != IZQUIERDA) && (botones != DERECHA)) {
    //stringOperacion = a.substring(0,xparpadeo) + prueba + b.substring(xparpadeo+1);
    //stringOperacion = a.substring(0,xparpadeo) + botones;// + b.substring(xparpadeo+1);  /////////////////////// P R E G U N T A R ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    stringOperacion = a.substring(0,xparpadeo);
    stringOperacion += botones;
    stringOperacion += b.substring(xparpadeo+1);
    //stringOperacion +=
  }
  if ((botones >= 48) && (botones <= 57)) {
    xparpadeo++;
    xparpadeoLCD++;
  }

  else if ((botones == SUMA) or (botones == RESTA) or (botones == MULTIPLICACION) or (botones == DIVISION) or (botones == RAIZ_CUADRADA) or (botones == POTENCIA) or (botones == PARENTESIS_ABRIR) or (botones == PARENTESIS_CERRAR) or (botones == '.') or (botones == NUMERO_PI) or (botones == NUMERO_e)) {
    xparpadeo++;
    xparpadeoLCD++;
  }
  else if ((botones == ARCOSENO) or (botones == ARCOCOSENO) or (botones == ARCOTANGENTE)) {
    xparpadeo++;
    xparpadeoLCD += 5;
  }
  else if ((botones == SEN) or (botones == COS) or (botones == TAN) or (botones == LOGARITMO_BASE_10)) {
    xparpadeo++;
    xparpadeoLCD += 3;
  }
  else if ((botones == LOGARITMO_NEPERIANO) or (botones == RAIZ_X)) {
    xparpadeo++;
    xparpadeoLCD += 2;
  }
  else if (botones == BORRAR_AC) {
    if (xparpadeo == stringOperacion.length()-1) {
      lcd.setCursor(xparpadeoLCD, 0);
      lcd.print("   ");
      if ((stringOperacion.charAt(xparpadeo-1) == ARCOSENO) or (stringOperacion.charAt(xparpadeo-1) == ARCOCOSENO) or (stringOperacion.charAt(xparpadeo-1) == ARCOTANGENTE)) {
        xparpadeoLCD -= 5;
      }
      else if ((stringOperacion.charAt(xparpadeo-1) == COS) or (stringOperacion.charAt(xparpadeo-1) == SEN) or (stringOperacion.charAt(xparpadeo-1) == TAN)) { //error al borrar seno, coseno, tangente
        xparpadeoLCD -=3;
      }
      else if ((stringOperacion.charAt(xparpadeo-1) == LOGARITMO_NEPERIANO) or (stringOperacion.charAt(xparpadeo-1) == RAIZ_X)) {
        xparpadeoLCD -=2;
      }

      else {
        xparpadeoLCD--;
      }
      stringOperacion = a.substring(0,xparpadeo-1);
      
      
      xparpadeo--;
      
    }
    else {
      stringOperacion = a.substring(0,xparpadeo) + b.substring(xparpadeo+1);
    }
  }

  else if ((botones == IZQUIERDA) && (xparpadeo > 0))  {
    lcd.setCursor(xparpadeoLCD, 0);
    lcd.print(" ");
    if ((stringOperacion.charAt(xparpadeo-1) == ARCOSENO) or (stringOperacion.charAt(xparpadeo-1) == ARCOCOSENO) or (stringOperacion.charAt(xparpadeo-1) == ARCOTANGENTE)) {
      xparpadeoLCD -= 5;
    }
    if ((stringOperacion.charAt(xparpadeo-1) == COS) or (stringOperacion.charAt(xparpadeo-1) == SEN) or (stringOperacion.charAt(xparpadeo-1) == TAN)) { //error al borrar seno, coseno, tangente
      xparpadeoLCD -=3;
    }
    else if ((stringOperacion.charAt(xparpadeo-1) == LOGARITMO_NEPERIANO) or (stringOperacion.charAt(xparpadeo-1) == RAIZ_X)) {
      xparpadeoLCD -=2;
    }
    else {
      xparpadeoLCD--;
    }
    xparpadeo--;
  }
  else if ((botones == DERECHA) && (xparpadeo < stringOperacion.length())) {
    if ((stringOperacion.charAt(xparpadeo) == ARCOSENO) or (stringOperacion.charAt(xparpadeo) == ARCOCOSENO) or (stringOperacion.charAt(xparpadeo) == ARCOTANGENTE)) {
      xparpadeoLCD += 5;
    }
    if ((stringOperacion.charAt(xparpadeo) == COS) or (stringOperacion.charAt(xparpadeo) == SEN) or (stringOperacion.charAt(xparpadeo) == TAN)) { //error al borrar seno, coseno, tangente
      xparpadeoLCD +=3;
    }
    else if ((stringOperacion.charAt(xparpadeo) == LOGARITMO_NEPERIANO) or (stringOperacion.charAt(xparpadeo) == RAIZ_X)) {
      xparpadeoLCD +=2;
    }
    else {
      xparpadeoLCD++;
    }
    xparpadeo++;
  }
}

//VARIABLES PARA PODER RESOLVER LOS PARENTESIS
byte generalInicioParentesis[3] = {0};           //variable donde guardaremos la posicion del primer parentesis de inicio y el último. EXPLICACION: el paréntesis de inicio es: '('. Por ejemplo en la operacion 5*(4*(6+7)) el primer parentesis inicio tiene la posicion 2 y el último 5 (empieza a contar desde el 0)
byte generalFinalParentesis[3] = {0};            //variable donde haremos lo mismo que en la anterior pero en vez de con el paréntesis inicio lo haremos con el paréntesis final ')'. 
boolean contadorInicioFinalParentesis = 0;       //variable donde guardaremos el número de pares de paréntesis. EXPLICACIÓN: en la operación (7*(2+8))/9  hay un total de 2 pares de paréntesis
boolean activadorGeneralFinalParentesis = 0;     //variable donde almacenaremos un 1 cuando nos encontremos el primer ')'. De esta forma en la operación ((2+8)*3)/(7-2), cuando nos encontremos el primer ')', sabremos que no tendremos que sumar más números al contador de paréntesis (solo nos quedará restar cada vez que encontremos ')') pues la operación que nos interesa sacar es ((2+8)*3) para resorverla.
boolean activadorInicioParentesis = 0;
boolean activadorParentesis = 0;
byte contadorParentesis = 0;
const byte TAMANO_SUB_OPERACION = 2;
String subOperacion[TAMANO_SUB_OPERACION];

boolean activadorTEST = 1;

boolean activadorxParpadeoFinal = 0;
boolean activadorShift = 0;

void CALC_distribucionBotones() {
  Serial.println(botones);
  if (botones == '=') {
    subOperacion[0] = stringOperacion;
    xparpadeo = stringOperacion.length() + 1;
    activadorxParpadeoFinal = 1;
    CALC_solucionarTotalOperacion();

  }
  if(activadorxParpadeoFinal == 0) {
    if (botones == SHIFT) {
      activadorShift = 1;
    }
    if (activadorShift == 1) {
      switch (botones) {
        case SEN:
          botones = ARCOSENO;
          activadorShift = 0;
        break;
        case COS:
          botones = ARCOCOSENO;
          activadorShift = 0;
        break;
        case TAN:
          botones = ARCOTANGENTE;
          activadorShift = 0;
        break;
      }
    }
    CALC_deBotonesAString();
  }
}

void CALC_solucionarTotalOperacion() {
  Serial.println("[INICIO]CALC_solucionarTotalOperacion");
  subOperacion[1] = subOperacion[0];
  while (activadorTEST == 1) {
    if (CALC_detectarParentesis(subOperacion[1]) == 1) {
      while ((CALC_detectarParentesis(subOperacion[1])) == 1) {
        CALC_separarParentesis(subOperacion[1]);
      }
      if (CALC_detectarParentesis(subOperacion[1]) == 0) {
        CALC_solucionarOperacion();
        CALC_sustituirParentesis();
        if (CALC_detectarParentesis(subOperacion[0]) == 0) {
          CALC_solucionarOperacion();
          activadorTEST = 0;
        }
        else {
          generalInicioParentesis[0] = 0;
          generalInicioParentesis[1] = 0;
          generalInicioParentesis[2] = 0;
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
    }
  }

  Serial.println("[FINAL]CALC_solucionarTotalOperacion");
  return;
}

boolean CALC_detectarParentesis(String operacion) {
  Serial.println("CALC_detectarParentesis");
  Serial.println(operacion);
  byte longitudOperacion = operacion.length();

  for (byte i = 0; i < longitudOperacion; i++) {
    char caracter = operacion.charAt(i);
    if ((caracter == '(') or (caracter == ')')) {
      return 1;
    }
  }
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
      subOperacion[1] =  operacion.substring(generalInicioParentesis[contadorInicioFinalParentesis], generalFinalParentesis[contadorInicioFinalParentesis]);
      Serial.println(subOperacion[1]);
      contadorInicioFinalParentesis = 1;
      return;


    }
  }
  Serial.println("[FINAL]CALC_separarParentesis");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CALC_solucionarOperacion() {
  byte longitudOperacion = subOperacion[1].length();
  float numeroActual = 0;
  float numeroAnterior = 0;
  byte contadorValores = 0;
  const byte TAMANOVALORES = 20;
  float valorNumerico[TAMANOVALORES] = {0};
  char posicionSignos[TAMANOVALORES] = {0};
  bool activadorMenos = 0;
  byte contadorActivadorMenos = 0;
  bool activadorNumeros = 0;
  bool activadorDecimales = 0;
  byte contadorDecimales = 0;





  for (byte i = 0; i <= longitudOperacion; i++) {
    char caracter = subOperacion[1].charAt(i);
    //Serial.println("Caracter");
    //Serial.println(caracter);
    if (caracter == NUMERO_PI) {
      valorNumerico[contadorValores] = PI;
      activadorNumeros = 1;
      if (activadorMenos == 1) {
        valorNumerico[contadorValores] = valorNumerico[contadorValores] * -1;
      }
    }

    if (caracter == NUMERO_e) {
      valorNumerico[contadorValores] = exp (1);
      activadorNumeros = 1;
      if (activadorMenos == 1) {
        valorNumerico[contadorValores] = valorNumerico[contadorValores] * -1;
      }
    }

    if ((caracter >= 48) && (caracter <= 57)) {
      numeroActual = caracter - 48 + numeroAnterior * 10;
      numeroAnterior = numeroActual;

      valorNumerico[contadorValores] = numeroActual;

      activadorNumeros = 1;
      if (activadorDecimales == 1) {
        contadorDecimales++;
        valorNumerico[contadorValores] = valorNumerico[contadorValores] / (pow(10, contadorDecimales ));
      }
      if (activadorMenos == 1) {
        valorNumerico[contadorValores] = valorNumerico[contadorValores] * -1;
      }
    }
    
    if ((caracter < 48) or (caracter > 57)) {
      if (activadorDecimales == 1) {
        if(valorNumerico[contadorValores-1] < 0) {
          valorNumerico[contadorValores-1] = valorNumerico[contadorValores-1] * -1;
          valorNumerico[contadorValores-1] = valorNumerico[contadorValores-1] + valorNumerico[contadorValores];
          valorNumerico[contadorValores-1] = valorNumerico[contadorValores-1] * -1;
        }
        else {
          valorNumerico[contadorValores-1] = valorNumerico[contadorValores-1] + valorNumerico[contadorValores];
        }
        valorNumerico[contadorValores] = 0;
        contadorValores--;
        Serial.println(valorNumerico[contadorValores]);
      }
      if (caracter == '.') {
        activadorDecimales = 1;
      }

      else {
        contadorDecimales = 0;
        activadorDecimales = 0;
      }

      
      if ((caracter == MULTIPLICACION) or (caracter == DIVISION) or (caracter == POTENCIA) or (caracter == RAIZ_CUADRADA) or (caracter == RAIZ_X) or (caracter == LOGARITMO_BASE_10) or (caracter == LOGARITMO_NEPERIANO) or (caracter == SEN) or (caracter == COS) or (caracter == TAN) or (caracter ==ARCOSENO) or (caracter == ARCOCOSENO) or (caracter == ARCOTANGENTE)) {
        posicionSignos[contadorValores] = caracter;
        if ((caracter == RAIZ_CUADRADA) or (caracter == LOGARITMO_BASE_10) or (caracter == LOGARITMO_NEPERIANO) or (caracter == SEN) or (caracter == COS) or (caracter == TAN) or (caracter == ARCOSENO) or (caracter == ARCOCOSENO) or (caracter == ARCOTANGENTE)) {
          contadorValores++;
        }
      }


      if (activadorNumeros == 1) {
        contadorActivadorMenos = 0;
        activadorMenos = 0;
        contadorValores++;
        numeroAnterior = 0;
      }
      activadorNumeros = 0;

      if (caracter == '-') {
        contadorActivadorMenos++;
        if ((contadorActivadorMenos % 2) == 1) {
          activadorMenos = 1;
        }
        else {
          activadorMenos = 0;
        }
      }
    }
  }

  if (activadorDecimales == 1) {
    valorNumerico[contadorValores-1] = valorNumerico[contadorValores-1] + valorNumerico[contadorValores];
    valorNumerico[contadorValores] = 0;
    contadorValores--;
  }



  for (byte i = 0; i < 5; i++) {
    Serial.print("numero =  ");
    Serial.println(valorNumerico[i]);
    Serial.println(posicionSignos[i]);
  }
  
  
  contadorValores++; //para las raices que tienen el signo delante del número


  // T R I G O N O M E T R I A       SENO COSENO TANGENTE
  for (byte i = 0; i < contadorValores+1; i++) {
    if (posicionSignos[i] == SEN) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      if (activadorRadianes == 1) {
        valorNumerico[i] = sin(valorNumerico[i]);
      }
      else {
        valorNumerico[i] = sin(deRadianesAGrados(valorNumerico[i]));
      }
      
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
    if (posicionSignos[i] == COS) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      if (activadorRadianes == 1) {
        valorNumerico[i] = cos(valorNumerico[i]);
      }
      else {
        valorNumerico[i] = cos(deRadianesAGrados(valorNumerico[i]));
      }
      
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
    if (posicionSignos[i] == TAN) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      if (activadorRadianes == 1) {
        valorNumerico[i] = tan(valorNumerico[i]);
      }
      else {
        valorNumerico[i] = tan(deRadianesAGrados(valorNumerico[i]));
      }
      
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
  }


  // T R I G O N O M E T R I A       ARCOSENO   ARCOCOSENO   ARCOTANGENTE
  for (byte i = 0; i < contadorValores+1; i++) {
    if (posicionSignos[i] == ARCOSENO) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      if (activadorRadianes == 1) {
        valorNumerico[i] = asin(valorNumerico[i]);
      }
      else {
        valorNumerico[i] = ((asin(valorNumerico[i])) * 360) / (2 * PI);
      }
      
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
    if (posicionSignos[i] == ARCOCOSENO) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      if (activadorRadianes == 1) {
        valorNumerico[i] = acos(valorNumerico[i]);
      }
      else {
        valorNumerico[i] = ((acos(valorNumerico[i])) * 360) / (2 * PI);
      }
      
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
    if (posicionSignos[i] == ARCOTANGENTE) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      if (activadorRadianes == 1) {
        valorNumerico[i] = atan(valorNumerico[i]);
      }
      else {
        valorNumerico[i] = ((atan(valorNumerico[i])) * 360) / (2 * PI);
      }
      
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
  }




  // L O G A R I T M O
  for (byte i = 0; i < contadorValores+1; i++) {
    if (posicionSignos[i] == LOGARITMO_BASE_10) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      valorNumerico[i] = log10(valorNumerico[i]);
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }

    if (posicionSignos[i] == LOGARITMO_NEPERIANO) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      valorNumerico[i] = log(valorNumerico[i]);
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
  }

  // P O T E N C I A      Y      R A I Z
  for (byte i = 0; i < contadorValores+1; i++) {

    if (posicionSignos[i] == POTENCIA) {
      valorNumerico[i] = pow(valorNumerico[i], valorNumerico[i+1]);
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }

    if (posicionSignos[i] == RAIZ_CUADRADA) {
      for (int a = i; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
      }
      valorNumerico[i] = pow(valorNumerico[i], 0.5);
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }

    if (posicionSignos[i] == RAIZ_X) {
      valorNumerico[i] = pow(valorNumerico[i+1], 1/valorNumerico[i]);
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
  }

 for (byte i = 0; i < 5; i++) {
    Serial.print("[fd]numero =  ");
    Serial.println(valorNumerico[i]);
    Serial.println(posicionSignos[i]);
  }



  // M U L T I P L I C A C I O N       Y       D I V I S O R
  for (byte i = 0; i < contadorValores+1; i++) {
    if (posicionSignos[i] == MULTIPLICACION) {
      valorNumerico[i] = valorNumerico[i] * valorNumerico[i+1];
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
    if (posicionSignos[i] == DIVISION) {
      valorNumerico[i] = valorNumerico[i] / valorNumerico[i+1];
      posicionSignos[i] = 0;
      for (int a = i+1; a <= contadorValores+1; a++) {
        valorNumerico[a] = valorNumerico[a+1];
        posicionSignos[a-1] = posicionSignos[a];
      }
      i--;
    }
  }



  resultado = 0;
  for (int i = 0; i <= contadorValores; i++) {
    resultado += valorNumerico[i];
  }
  Serial.println("RESULTADO");
  Serial.println(resultado);
  Serial.println("[FINAL]CALC_solucionarOperacion");
}


String floatToString( float n, int l, int d, boolean z) {
  // Convierte un float en una cadena.
  // n -> número a convertir.
  // l -> longitud total de la cadena, por defecto 8.
  // d -> decimales, por defecto 2.
  // z -> si se desea rellenar con ceros a la izquierda, por defecto true.
  char c[l + 1];
  String s;

  dtostrf(n, l, d, c);
  s = String(c);

  if (z) {
    s.replace(" ", "0");
  }

  int posMenos = s.indexOf('-');
  
  if (posMenos > 0) {
    s = s.substring(posMenos);
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
  subOperacion[0] = anteriorResultado;         // + floatToString(resultado, 5, 4, 1) + posteriorResultado;
  subOperacion[0] += floatToString(resultado, 5, 4, 1);
  subOperacion[0] += posteriorResultado;
  subOperacion[1] = subOperacion[0];
  Serial.println(anteriorResultado);
  Serial.println(floatToString(resultado, 5, 4, 1));
  Serial.println(posteriorResultado);
  Serial.println(subOperacion[0]);
  Serial.println("[FINAL]CALC_sustituirParentesis");
  return;
}




unsigned long tiempoParpadeo;                   //variable donde almacenar el tiempo (en millis) transcurrido. Será utilizado para medir el tiempo de parpadeo que se produce en la posición donde vamos a escribir.
const int TIEMPO_PARPADEO = 1800;               //tiempo que tiene que pasar entre cada parpadeo.


//void parpadeoLCD() {
  
//}




void CALC_escribirLCD() {
  byte valorx0;
  String stringOperacionVisual;
  String stringOperacionVisualLCD;
  valorx0 = stringOperacion.length();
  for (byte i = 0; i <= valorx0; i++) {
    char caracter = stringOperacion.charAt(i);
    if ((caracter >= 48) && (caracter <= 57)) {
      stringOperacionVisual += caracter;
    }
    switch (caracter) {

      case '.':
        stringOperacionVisual += '.';
      break;
      case COS:
        stringOperacionVisual += "cos";
      break;
      case ARCOCOSENO:
        stringOperacionVisual += "cosU ";
      break;
      case SEN:
        stringOperacionVisual += "sen";
      break;
      case ARCOSENO:
        stringOperacionVisual += "senU ";
      break;
      case TAN:
        stringOperacionVisual += "tan";
      break;
      case ARCOTANGENTE:
        stringOperacionVisual += "tanU ";
      break;
      case LOGARITMO_BASE_10:
        stringOperacionVisual += "log";
      break;
      case LOGARITMO_NEPERIANO:
        stringOperacionVisual += "ln";
      break;
      case SUMA:
        stringOperacionVisual += "+";
      break;
      case RESTA:
        stringOperacionVisual += "-";
      break;
      case MULTIPLICACION:
        stringOperacionVisual += "*";
      break;
      case DIVISION:
        stringOperacionVisual += "/";
      break;
      case RAIZ_CUADRADA:
        stringOperacionVisual += "K";
      break;
      case RAIZ_X:
        stringOperacionVisual += "qK";
      break;
      case POTENCIA:
        stringOperacionVisual += "^";
      break;
      case '(':
        stringOperacionVisual += "(";
      break;
      case ')':
        stringOperacionVisual += ")";
      break;
      case NUMERO_PI:
        stringOperacionVisual += "P";
      break;
      case NUMERO_e:
        stringOperacionVisual += 'N';
      break;

    }
  }
  stringOperacionVisualLCD =  stringOperacionVisual;
  byte valorx = stringOperacionVisualLCD.length();
  if (valorx >= 16) {
    stringOperacionVisualLCD = stringOperacionVisual.substring(valorx - 15);
  }
  if (activadorxParpadeoFinal == 1) {
    xparpadeo = stringOperacion.length();
    xparpadeoLCD = stringOperacionVisualLCD.length();

    activadorxParpadeoFinal = 0;
  }
  if (xparpadeoLCD < stringOperacionVisualLCD.length()) {
    lcd.setCursor(stringOperacionVisualLCD.length(), 0);
    lcd.print("   ");
  }

  lcd.setCursor(0, 0);
  for (byte i = 0; i < valorx; i++) {
    if (stringOperacionVisualLCD.charAt(i) == 'K') {
      lcd.write(byte(0));
    }
    if (stringOperacionVisualLCD.charAt(i) == 'q') {
      lcd.write(byte(1));
    }
    if (stringOperacionVisualLCD.charAt(i) == 'P') {
      lcd.write(byte(2));
    }
    if (stringOperacionVisualLCD.charAt(i) == 'N') {
      lcd.write(byte(3));
    }
    if (stringOperacionVisualLCD.charAt(i) == 'U') {
      lcd.write(byte(4));
    }
    else if ((stringOperacionVisualLCD.charAt(i) != 'K') && (stringOperacionVisualLCD.charAt(i) != 'q') && (stringOperacionVisualLCD.charAt(i) != 'P') && (stringOperacionVisualLCD.charAt(i) != 'N') && (stringOperacionVisualLCD.charAt(i) != 'U')) {
      lcd.print(stringOperacionVisualLCD.charAt(i));
    }
  }
  
  lcd.setCursor(stringOperacionVisualLCD.length()+1, 0);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print(resultado);

  lcd.setCursor(xparpadeoLCD, 0);
  if (millis() - tiempoParpadeo <= TIEMPO_PARPADEO / 2) {
    lcd.print("_");
  }
  else if ((millis() - tiempoParpadeo <= TIEMPO_PARPADEO) && (xparpadeoLCD == stringOperacionVisual.length())) {
    lcd.print(" ");
  }
  else if (millis() - tiempoParpadeo <= TIEMPO_PARPADEO) {

  }
  else {
    tiempoParpadeo = millis();
  }
 
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

  lcd.createChar(0, RAIZ_CARACTER);
  lcd.createChar(1, RAIZ_DE_X_CARACTER);
  lcd.createChar(2, NUMERO_PI_CARACTER);
  lcd.createChar(3, NUMERO_E_CARACTER);
  lcd.createChar(4, ELEVADO_MENOS_UNO_CARACTER);

  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("MoaisEnergyDEF");
  delay(1000);
  lcd.clear();
  tiempoParpadeo = millis();
  tiempoReinicioOhms = millis();


  //////////////////////////////////////////////////////


  subOperacion[0] = stringOperacion;
  //CALC_solucionarTotalOperacion();//////////////////////////////////////////////////////////////////////////////



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
