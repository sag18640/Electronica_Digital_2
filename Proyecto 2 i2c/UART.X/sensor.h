#ifndef sensor
#define	sensor
//------------------------------------------------------------------------------
#define  SENSOR_ADDRESS             0x90     // dirección + bit de lectura
#define  TEMPERATURE_ADDRESS      0x00
//------------------------------------------------------------------------------
void  sensor_init(void);
float read(char regAddress);
float toFloat(signed int tempr);
//------------------------------------------------------------------------------
#endif