#pragma once
#include <settings.h>
//EXAMPLE FOR SINGLE WRITE REGISTER
/* 8
 * id
 * function               0x06
 * ADDRESS                HIGH
 * ADDRESS                LOW
 * VALUE                  HIGH
 * VALUE                  LOW
 * CRC                    HIGH
 * CRC                    LOW
*/

//EXAMPLE FOR GROUP WRITE REGISTER
/* 13
 * id
 * function               0x10
 * START_REGISTER         HIGH
 * START_REGISTER         LOW
 * COUNT_REGISTER         HIGH
 * COUNT_REGISTER         LOW
 * COUNT DATA BYTES       EQUALS 2*COUNT_REGISTER
 * FIRST_GROUP_DATA       HIGH
 * FIRST_GROUP_DATA       LOW
 * SECOND_GROUP_DATA      HIGH
 * SECOND_GROUP_DATA      LOW
 * CRC                    HIGH
 * CRC                    LOW
*/

extern const uint8_t  START_ALL_CW[8] = {
  ALL,  //id
  0x06, //write single register
  0x00, //hi address
  0x00, //lo address
  0x00, //hi value
  0x01, //low value
  0x49, //hi crc16
  0xDB  //lo crc16
};

extern const uint8_t  START_ALL_CCW[8] = {
  ALL,  //id
  0x06, //write single register
  0x00, //hi address
  0x00, //lo address
  0x00, //hi value
  0x02, //low value
  0x09, //hi crc16
  0xDA  //lo crc16
};

extern const uint8_t  STOP_ALL[8] = {
  ALL,  //id
  0x06, //write single register
  0x00, //hi address
  0x00, //lo address
  0x00, //hi value
  0x00, //low value
  0x88, //hi crc16
  0x1B  //lo crc16
};

extern const uint8_t  LAYER_START_MAX_CW[13] = {
  LAYER_ID, //id
  0x10,     //write group registers
  0x00,     //start reg hi 
  0x00,     //start reg lo
  0x00,     //count reg hi
  0x02,     //count reg lo
  0x04,     //count bytes
  0x00,     //first data hi
  0x01,     //first data lo
  0x13,     //second data hi
  0x88,     //second data lo
  0xA0,     //crc hi
  0x7D      //crc lo
};

extern const uint8_t  LAYER_START_MAX_CCW[13] = {
  LAYER_ID, //id
  0x10,     //write group registers
  0x00,     //start reg hi 
  0x00,     //start reg lo
  0x00,     //count reg hi
  0x02,     //count reg lo
  0x04,     //count bytes
  0x00,     //first data hi
  0x02,     //first data lo
  0x13,     //second data hi
  0x88,     //second data lo
  0x50,     //crc hi
  0x7D      //crc lo
};

uint8_t  LAYER_STOP_SET_SPEED[13] = {
  LAYER_ID, //id
  0x10,     //write group registers
  0x00,     //start reg hi 
  0x00,     //start reg lo
  0x00,     //count reg hi
  0x02,     //count reg lo
  0x04,     //count bytes
  0x00,     //first data hi
  0x00,     //first data lo
  0x00,     //second data hi
  0x00,     //second data lo
  0x00,     //crc hi
  0x00      //crc lo
};

uint8_t  LAYER_SET_SPEED[8] = {
  LAYER_ID,  //id
  0x06,      //write single register
  0x00,      //hi address
  0x01,      //lo address
  0x00,      //hi value
  0x00,      //low value
  0x00,      //hi crc16
  0x00       //lo crc16
};

uint8_t  REEL_SET_SPEED[8] = {
  REEL_ID,  //id
  0x06,     //write single register
  0x00,     //hi address
  0x01,     //lo address
  0x00,     //hi value
  0x00,     //low value
  0x00,     //hi crc16
  0x00      //lo crc16
};