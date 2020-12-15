/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/i2c.h>

#define I2C_DEV DT_LABEL(DT_ALIAS(i2c_0))
#define I2C_DEV1 DT_LABEL(DT_ALIAS(i2c_1))
//#define I2C_DEV1 DT_LABEL(DT_NODELABEL(i2c1))

#define LM75A_DEFAULT_ADDRESS   0x48
#define BMP280_DEFAULT_ADDRESS  0x76

/**
 * @file Sample app using the I2C
 */


const float LM75A_DEGREES_RESOLUTION = 0.125;
const int LM75A_REG_ADDR_TEMP = 0;

void main(void)
{   
  
	const struct device *i2c_dev;
	const struct device *i2c_dev1;
	
	uint8_t cmp_data[16];
	uint8_t data[16];
	int i, ret;
	uint16_t temp = 0x00;
    uint8_t pointer = 0x00;

	i2c_dev = device_get_binding(I2C_DEV);
	i2c_dev1 = device_get_binding(I2C_DEV1);
	if (!i2c_dev1) {
		printk("I2C: Device driver not found.\n");
		return;
	}
	printk("----------->> -------------------- <<---------------\n");
    printk("---------->>   *** I2C LM75A ***   <<--------------\n\n");
	printk("I2C LM75A - Pruebas.\n");
	/* ----- */
	
	pointer = 0x03; //Tos register pointer - 5000h por defecto
	i2c_write(i2c_dev, &pointer, 1, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	ret = i2c_read(i2c_dev, &data[0], 2, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	printk("----->> SetPoint - Tos Register: %x|%x\n\n", data[0], data[1]);
	
	pointer = 0x02; //Thyst register pointer - 4B00h por defecto
	i2c_write(i2c_dev, &pointer, 1, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	ret = i2c_read(i2c_dev, &data[0], 2, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	printk("----->> Hysteresis - Thyst Register: %x|%x\n\n", data[0], data[1]);
	
	pointer = 0x01; //Configuration register pointer - 00h por defecto
	i2c_write(i2c_dev, &pointer, 1, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	ret = i2c_read(i2c_dev, &data[0], 1, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	printk("----->>  Configuration - Register %x|%x\n\n", data[0], data[1]);
	
	
	//only 9 bits of the two bytes are used to store the set-point data in 2’s complement format with the resolution of 0.5 °C
	printk("----->>  Writing - Tos Register %x|%x\n", data[0], data[1]);
	pointer = 0x03;
	data[0] = pointer;
	temp = 24.5; //valor de setpoint que será enviado - resolución de 0.5°C solo si es float
	temp = temp*2; //dividir por 0.5 <=> multiplicar por 2
	data[1] = temp >> 8;//MSByte
	data[2] = (uint16_t)temp & 0x0F;//LSByte
	i2c_write(i2c_dev, &data[0], 3, LM75A_DEFAULT_ADDRESS);
	printk("----->>  Tos Register wrote temp: <%x>, MSByte <%x>, LSByte <%x>\n\n", temp, data[1], data[2]);
	k_msleep(5);
	
	//verificando el registro de Tos
	pointer = 0x03; //Tos register pointer - 5000h por defecto
	i2c_write(i2c_dev, &pointer, 1, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	ret = i2c_read(i2c_dev, &data[0], 2, LM75A_DEFAULT_ADDRESS);
	k_msleep(5);
	printk("----->> SetPoint - Tos Register: %x|%x\n\n", data[0], data[1]);
	
	pointer = 0x00; // Temp register pointer
    i2c_write(i2c_dev, &pointer, 1, LM75A_DEFAULT_ADDRESS);
    // the pointer doesn't need to be set every i2c read. Once set, pointer is latched and the next readings will be made form the register pointed by pointer
    ret = 0;
    
    printk("----------->> -------------------- <<---------------\n");
    printk("---------->>   *** I2C BMP280 ***   <<--------------\n\n");
    
        
	pointer = 0xF7; //Tos register pointer - 5000h por defecto
	i2c_write(i2c_dev1, &pointer, 1, BMP280_DEFAULT_ADDRESS);
	
	k_msleep(5);
	ret = i2c_read(i2c_dev1, &data[0], 4, BMP280_DEFAULT_ADDRESS);
	k_msleep(5);
	printk("----->> presion %x temp %x\n\n", data[0], data[3]); // data[0] = 0xF7 reg info -  data[3] = 0xFA red info.
	
    pointer = 0xD0; //Tos register pointer - 5000h por defecto
	i2c_write(i2c_dev1, &pointer, 1, BMP280_DEFAULT_ADDRESS);
    
    k_msleep(5);
	ret = i2c_read(i2c_dev1, &data[0], 1, BMP280_DEFAULT_ADDRESS);
	k_msleep(5);
	printk("----->> ID BMP280 %x\n\n", data[0]);
	
    ret = 0;
    while(1) {
        //int i2c_write(conststructdevice *dev, const uint8_t *buf, uint32_t num_bytes, uint16_t addr)
        
        // int i2c_read(const structdevice *dev, uint8_t *buf, uint32_t num_bytes, uint16_t addr)
        ret = i2c_read(i2c_dev, &data[0], 2, LM75A_DEFAULT_ADDRESS);
        
        //int i2c_reg_read_byte(conststructdevice *dev, uint16_t dev_addr, uint8_t reg_addr, uint8_t *value)
        //ret = i2c_reg_read_byte(i2c_dev, LM75A_DEFAULT_ADDRESS, 0x00, &data[0]);
        
        if(~ret) {
            printk("Succesful read\n");
        }
        else {
            printk("Error read\n");
        }    
        
        printk("data[0]: %X data[1] %X \n", data[0], data[1]);
        
        temp = data[0] << 8 | (data[1] & 0x80 );
        temp = temp >> 5; //toma 11 MSBits. Como son 16 bits en temp, se debe hacer el corrimiento.
        temp = temp * LM75A_DEGREES_RESOLUTION;
        //temp = data[0] << 8 | data[1];
        printk("Temperatura en Celsius: %d\n", temp); //float not supported by printk
        k_msleep(2500);
    }
    
    
	/* Do one-byte read/write */
    /*
	data[0] = 0xAE;
	ret = write_bytes(i2c_dev, 0x00, &data[0], 1);
	if (ret) {
		printk("Error writing to FRAM! error code (%d)\n", ret);
		return;
	} else {
		printk("Wrote 0xAE to address 0x00.\n");
	}

	data[0] = 0x86;
	ret = write_bytes(i2c_dev, 0x01, &data[0], 1);
	if (ret) {
		printk("Error writing to FRAM! error code (%d)\n", ret);
		return;
	} else {
		printk("Wrote 0x86 to address 0x01.\n");
	}

	data[0] = 0x00;
	ret = read_bytes(i2c_dev, 0x00, &data[0], 1);
	if (ret) {
		printk("Error reading from FRAM! error code (%d)\n", ret);
		return;
	} else {
		printk("Read 0x%X from address 0x00.\n", data[0]);
	}

	data[1] = 0x00;
	ret = read_bytes(i2c_dev, 0x01, &data[0], 1);
	if (ret) {
		printk("Error reading from FRAM! error code (%d)\n", ret);
		return;
	} else {
		printk("Read 0x%X from address 0x01.\n", data[0]);
	}
    */
	
	
}
