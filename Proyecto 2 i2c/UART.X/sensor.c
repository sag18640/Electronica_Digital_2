/*
 * File:   sensor.c
 * Author: SAGASTUME
 *
 * Created on 26 de marzo de 2021, 08:12 PM
 */


#include <xc.h>
#include "I2C.h"
#include "sensor.h"
//se inicia la comunicación i2c con el sensor

void sensor_init() {
    I2C_init(100000); //inicio a 100Kb/s

}
//se lee los datos del sensor
float read(char regAddress) {
    signed int MSB, LSB;
    //se inicia la comunicación i2c
    I2C_start();
    //se escribe la dirección del sensor y se le indica que se le escribirá
    I2C_write(SENSOR_ADDRESS + 0); // 0 = Write             
    //se le indica la dirección que le se utilizará
    I2C_write(regAddress);
    //se reinicia la comunicación
    I2C_restart();
    //se escribe la dirección del sensor y se le indica que se leera
    I2C_write(SENSOR_ADDRESS + 1); // 1 = Read
    //se guarda el valor de la lectura del i2c en 2 variables
    MSB = I2C_read(ACK);
    LSB = I2C_read(NACK);
    //se detiene la comunicación
    I2C_stop();
    //se convierte el valor recibido a float
    return toFloat((MSB << 8) + LSB);
}
//------------------------------------------------------------------------------

float toFloat(signed int tempr) {
    float result = (float) (tempr >> 8);//Se descarta el byte menos significativo

    return (result); 
}
//------------------------------------------------------------------------------
