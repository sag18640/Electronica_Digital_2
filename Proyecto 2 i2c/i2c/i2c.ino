//autor: SAGASTUME 18640  


/************************** Configuration ***********************************/
//se importa la libreria de configuración y se define los pines de RXD2 y TXD2 del esp32
#include "config.h"
#define RXD2 16
#define TXD2 17



/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch


#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;


// set up the 'counter' feed
AdafruitIO_Feed *LED = io.feed("LED1");
AdafruitIO_Feed *LED2 = io.feed("LED2");
AdafruitIO_Feed *temperatura = io.feed("temp");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1, RXD2, TXD2);
  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  LED->onMessage(handleMessage);
  LED2->onMessage(handleMessage2);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  LED->get();
  LED2->get();
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();



  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // save count to the 'counter' feed on Adafruit IO

    if (Serial2.available()) {
      temp=Serial2.read();
      Serial.print("sending -> ");
      Serial.println(temp);
      temperatura->save(temp);
      delay(4000);
    }

  }

}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  if (data->toString() == "ON") {
    Serial.println("ON");
    Serial2.write("+");
  } else if (data->toString() == "OFF") {
    Serial.println("OFF");
    Serial2.write("-");
  }
  delay(2000);
}

void handleMessage2(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  if (data->toString() == "ON") {
    Serial.println("/");
    Serial2.write("/");
  } else if (data->toString() == "OFF") {
    Serial.println("&");
    Serial2.write("&");
  }
  delay(2000); 
}
