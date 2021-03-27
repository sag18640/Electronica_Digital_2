#ifndef I2C
#define I2C
//-------------------------Define i2c pins--------------------------------------
#define SCK     RC3
#define SDA     RC4

#define  ACK  0
#define  NACK 1
//---------------------------DIRECTION------------------------------------------
#define SCK_dir TRISC3			//Clock pin direction
#define SDA_dir TRISC4			//Data pin direction
//------------------------------------------------------------------------------
#endif