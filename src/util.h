#ifndef UTIL_H
#define	UTIL_H

#include <stdint.h>

char* itoa(int i, char b[]);
void memcpy(uint8_t *destination, const uint8_t *src, uint32_t len);
void memset(uint8_t *destination, uint8_t val, uint32_t len);

#endif	/* UTIL_H */