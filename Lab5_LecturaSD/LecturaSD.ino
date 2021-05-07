/*  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 2 Feb 2014
  by Scott Fitzgerald

  USED and MODIFIED BY Cristopher Sagastume 18640
  DATE 05/04/2021
*/
#include <SPI.h>
#include <SD.h>
int dato;

File root;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SPI.setModule(0);

  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/"); //se abre el directorio de la SD

  menu();//se llama al menú

  Serial.println("done!");
}

void loop()
{
  if (Serial.available()){ //si existe comunicación Serial
    dato = Serial.read(); //se guarda el dato que se recibio
  }
  if (dato=='3') { //si el dato igual a 3 se abre el archivo .txt de pikachu
    File dataFile = SD.open("PIKA.txt");

    // if the file is available, write to it:
    if (dataFile) { 
      while (dataFile.available()) {
        Serial.write(dataFile.read());//se escribe en el monitor la lectura del .txt
      }
      dataFile.close(); // se cierra el archivo
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening PIKA.txt");
    }
    menu();//se llama al menú
  }
  else if (dato=='2') {//si el dato recibido es igual a 2 se abre el archivo .txt de kirby enojado 
    File dataFile = SD.open("KIRBYE.txt");

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read()); //se escribe en el monitor la lectura del .txt
      }
      dataFile.close();//se cierra el archivo
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening KIRBYE.txt");
    }
    menu();
  }
  else if (dato=='1') { //si el dato es igual a 1 se abre el archivo .txt de calamardo
    File dataFile = SD.open("CALAMA~1.txt");

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) { //se escribe en el monitor la lectura del .txt
        Serial.write(dataFile.read());
      }
      dataFile.close();//se cierra el archivo
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening CALAMARDO.txt");
    }
    menu();
  }
}

void printDirectory(File dir) {
  while (true) {
    File entry =  dir.openNextFile(); //entry guardará los archivos del directorio indicado
    if (! entry) {
      // no more files
      break;
    }
    Serial.print(entry.name()); //imprimimos los nombres de los archivos del directorio indicado
    Serial.println("\t\t");

    entry.close();//cerramos el directorio 
  }
}
void menu(void){
  root = SD.open("/");//volvemos a abrir la SD en el directorio principal
  root.rewindDirectory();//regresamos el "cursor" al primer archivo del directorio
  Serial.println("");
  Serial.println("Bienvenido al menú de lectura de SD");
  Serial.println("______________________________________");
  Serial.println("ingresar opciones entre 1,2 y 3 dependiendo el archivo que quiera leer excluyendo SYSTEM");
  Serial.println("_____________________________________________________________________________________________");
  printDirectory(root); //llamamos la funcion que imprime los nombres de los archivos 
}
