#ifndef I2C_H
#define I2C_H

void I2CInit(void);
void I2CStart(void);
void I2CStop(void);

unsigned char I2CWriteByte(unsigned char data);
unsigned char I2CReadByte(unsigned char *data,unsigned char ack);


#endif
