#include <stdint.h>
#include <stdio.h>
uint8_t cmd[11];

uint16_t get_crc16(uint8_t * buffer, uint8_t len){
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (unsigned char i = 0; i < len; i++) 
    {
        temp = temp ^ buffer[i];
        for (unsigned char j = 1; j <= 8; j++) 
        {
            flag = temp & 0x0001;
            temp >>= 1;
            if (flag)   temp ^= 0xA001;
        }
    }
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    return temp;
}

int main() {
    cmd[0] = 0x02;
    cmd[1] = 0x10;
    cmd[2] = 0x00;
    cmd[3] = 0x00;
    cmd[4] = 0x00;
    cmd[5] = 0x02;
    
  
    uint16_t crc = get_crc16(cmd, 6);

    printf("%d",crc);
    //while(1);
    return 0;
}