/*
 * File            : spi.c
 * Author          : Ligo George
 * Company         : electroSome
 * Editor          : Cristopher
 * Project         : SPI Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 */

#include "SPI.h"
//función de inicialización SPI
void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{   //se coloca el SDO como output
    TRISC5 = 0;
    if(sType & 0b00000100) //If Slave Mode se hace mascará de bits
    {
        SSPSTAT = sTransmitEdge;//se carga el valor del eje de transmisión 
        TRISC3 = 1;//coloca CLK como input
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge; //se carga una mascara de datos
        TRISC3 = 0;//coloca CLK como output
    }
   //se carga mascara al SSPCON con los valores de configuración
    SSPCON = sType | sClockIdle;
}
//función de verificación de recepción de dato
static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF ); // Wait for Data Transmit/Receipt complete
}

void spiWrite(char dat)  //Write data to SPI bus
{
    SSPBUF = dat; //cargar el valor al buffer
}

char spiRead() //Resad the received data
{
    spiReceiveWait();        // wait until the all bits receive
    return(SSPBUF); // read the received data from the buffer
}