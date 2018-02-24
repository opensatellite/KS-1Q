#ifndef FPRINT_H
#define FPRINT_H

void halt(void);
int error(const char *format, ...);
void print_hex(uint8_t *ptr, uint32_t begin, uint32_t size);
void f_printresult(FRESULT rc);


#endif