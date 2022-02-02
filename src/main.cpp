#include <Arduino.h>
// ESTA VERSION DEL NANO ES GENERAL PARA TODOS LOS ESP DE KOMATSU

#include <DMD2.h>
#include <SPI.h>
#include <Separador.h> // liberia para separa string (se usa para separar valores en la trama de datos de comunicación con el UDP)

#include <fonts/SystemFont5x7.h>
#include <fonts/Droid_Sans_12.h>

#include <fonts/Arial_Black_16.h>
#include <fonts/Arial14.h>
// #include <fonts/digit_16.h>

// const int PIRPin = 2; ///////// PIR

Separador s; // instancia para separar string

// Set Width to the number of displays wide you have
const int WIDTH = 3;
// const int WIDTH = 1;

// You can change to a smaller font (two lines) by commenting this line,
// and uncommenting the line after it:
const uint8_t *FONT1 = Arial14;
const uint8_t *FONT2 = Arial_Black_16;
const uint8_t *FONT3 = SystemFont5x7;
const uint8_t *FONT4 = Droid_Sans_12;
// const uint8_t *FONT5 = digit_16;

SoftDMD dmd(WIDTH, 1);      // DMD controls the entire display
DMD_TextBox box(dmd, 0, 2); // "box" provides a text box to automatically write to/scroll the display

//**********************************
int PtoBuzzer = 12;
//**********************************
int cont_disp = 0;
const char *next = "connecting";

// variables para el parpadeo del display
bool parpadeo;
bool parpadeo_aux;
unsigned long int tiempo_millis;
unsigned long int tiempo_aux;

// variables a mostrar en display
int month = 0;
int accidentesMes = 0;
String monthDisplay = "";

String mensaje;

void Pantalla()
{
  // Serial.println("Pantalla 1");
  if (parpadeo && !parpadeo_aux)
  {
    box.clear();
    box.print((String) "Accidentes");
    // Serial.println("A:" + String(aforo) + " T:" + String(total));
  }
  else if (parpadeo && parpadeo_aux)
  {
    box.clear();
    box.print((String)monthDisplay + ":_");
    box.print(accidentesMes);
    // Serial.println("LLENO");
  }
}

/////////////////// FUNCION PARA ACTUALIZAR EL MES ////////////////////////
void updateMonth(int month)
{
  switch (month)
  {
  case 1:
    monthDisplay = "Enero";
    break;
  case 2:
    monthDisplay = "Febrero";
    break;
  case 3:
    monthDisplay = "Marzo";
    break;
  case 4:
    monthDisplay = "Abril";
    break;
  case 5:
    monthDisplay = "Mayo";
    break;
  case 6:
    monthDisplay = "Junio";
    break;
  case 7:
    monthDisplay = "Julio";
    break;
  case 8:
    monthDisplay = "Agosto";
    break;
  case 9:
    monthDisplay = "Sept. ";
    break;
  case 10:
    monthDisplay = "Octubre";
    break;
  case 11:
    monthDisplay = "Nov. ";
    break;
  case 12:
    monthDisplay = "Dic. ";
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(FONT2);
  dmd.begin();
}

char inicio = 1;

void loop()
{
  // put your main code here, to run repeatedly:
  while (cont_disp <= 10) // mientras el esp8266 no se conecta a la red
  {
    box.print(*next);
    delay(350);
    next++;
    cont_disp++; // pantalla de connecting ...
  }

  if (Serial.available()) // vereficamos que llege comunicación serial ESP8266
  {
    // delay(10);s
    if (inicio == 1) // limpiar pantalla cuando el sisteam inicie
    {
      box.clear();
      inicio = 0;
    }

    mensaje = Serial.readStringUntil('\n');
    if (mensaje.startsWith("/", 0)) // si es que el string empieza con / aforo y total recibidos por el esp
    {
      month = s.separa(mensaje, '/', 1).toInt();
      accidentesMes = s.separa(mensaje, '/', 2).toInt();

      updateMonth(month); // actualizamos el mes si es necesario
    }
  }

  if(month != 0) Pantalla(); // mostramos información en pantalla

  // condicionales para el parpadeo del display
  /////////// rutinas para parpadeo de display //////////
  parpadeo = false;
  tiempo_millis = millis();
  if (tiempo_millis < 500)
  {                 // cuando millis desborde despues de 50d aprox.
    tiempo_aux = 0; // volvemos el tiempo_aux a 0;
  }
  if ((tiempo_millis - tiempo_aux) > 2000)
  {
    tiempo_aux = tiempo_millis;
    parpadeo = true;
    parpadeo_aux = !parpadeo_aux;
  }
}