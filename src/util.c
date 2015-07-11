#include "util.h"

char* itoa(int i, char b[])
{
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    } while(i);
    return b;
}

void memcpy(uint8_t *destination, const uint8_t *src, uint32_t len)
{
    const uint8_t *start_pointer = (const uint8_t *)src;
    uint8_t *destination_pointer = (uint8_t *)destination;
    while(len-- != 0) *destination_pointer++ = *start_pointer++;
}

void memset(uint8_t *destination, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)destination;
    while (len-- != 0) *temp++ = val;
}