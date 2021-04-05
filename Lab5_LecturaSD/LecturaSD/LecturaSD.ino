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

  root = SD.open("/");

  menu();

  Serial.println("done!");
}

void loop()
{
  if (Serial.available()){
    dato = Serial.read();
  }
  if (dato=='1') {
    File dataFile = SD.open("PIKA.txt");

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening PIKA.txt");
    }
    menu();
  }
  else if (dato=='2') {
    File dataFile = SD.open("KIRBYE.txt");

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening KIRBYE.txt");
    }
    menu();
  }
  else if (dato=='3') {
    File dataFile = SD.open("CALAMA~1.txt");

    // if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening CALAMARDO.txt");
    }
    menu();
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    Serial.print(entry.name());
    Serial.println("\t\t");

    entry.close();
  }
}
void menu(void){
  root = SD.open("/");
  root.rewindDirectory();
  Serial.println("");
  Serial.println("Bienvenido al menú de lectura de SD");
  Serial.println("__________________________________");
  Serial.println("ingresar opciones entre 1,2 y 3 dependiendo el archivo que quiera leer excluyendo SYSTEM");
  printDirectory(root,0);
  
  
}
