//Cristopher Sagastume 18640

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

volatile uint32_t i;

int main (void){
    //Se configura el reloj a 40Mhz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Se habilita el boton izquierdo de la tiva como input pull-up a 2mA
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //Se habilitan los led como salidas (Puerto F)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    while(1){
        //Se verifica si el botón fue presionado
        if (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)){
            //empieza el ciclo encendiendo el led verde con la mascara 0x08
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x08);
            //Se realiza una iteracion como sustituto a un delay
            for (i=0;i<2000000;i++){}

            //se apaga el led verde
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x00);
            for (i=0;i<2000000;i++){}

            //se enciende el led verde para que parpadee
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x08);
            for (i=0;i<1000000;i++){}

            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x00);
            for (i=0;i<1000000;i++){}

            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x08);
            for (i=0;i<2000000;i++){}

            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x00);
            for (i=0;i<2000000;i++){}

            //Se enciende el led amarillo con la mascara 0x0A
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x0A);
            for (i=0;i<2000000;i++){}

            //Se apaga el led amarillo
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x00);
            for (i=0;i<2000000;i++){}

            //Se enciende el led rojo con la mascara 0x02
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0x02);
            for (i=0;i<2000000;i++){}


        }

    }
}
