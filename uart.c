#include "uart.h"

volatile unsigned int *const ttyS0 = (unsigned int *const)0xd4017000;

#define UART8250_RX  0
#define UART8250_TX  0
#define UART8250_LCR 3
#define UART8250_LSR 5

#define UART8250_LCR_DLAB 0x80

#define UART8250_LSR_DATA 0x01
#define UART8250_LSR_TXRDY 0x20

void uart_init() {
    // set DLAB = 0
    ttyS0[UART8250_LCR] &= ~UART8250_LCR_DLAB;
}

void uart_put_c(char c) {
    while ((ttyS0[UART8250_LSR] & UART8250_LSR_TXRDY) == 0);
    ttyS0[UART8250_TX] = c;
}

char uart_check_read() { // 1: data ready, 0: no data
    return (ttyS0[UART8250_LSR] & UART8250_LSR_DATA) != 0;
}

char uart_get_c() {
    while ((ttyS0[UART8250_LSR] & UART8250_LSR_DATA) == 0);
    return ttyS0[UART8250_RX];
}

void print_s(const char *c) {
    while (*c) {
        uart_put_c(*c);
        c ++;
    }
}

void print_long(long x) {
    char buffer[30];
    if (x < 0) {
        uart_put_c('-');
        x = -x;
    }
    int idx = 0;
    while (x) {
        long new_x = x / 10;
        long rem_x = x % 10;
        buffer[idx ++] = '0' + rem_x;
        x = new_x;
    }
    if (idx == 0) uart_put_c('0');
    else while (idx) uart_put_c(buffer[--idx]);
}

void print_digit(unsigned char x) {
    uart_put_c('0'+x);
}

void dump_hex(unsigned long x) {
    uart_put_c('0');
    uart_put_c('x');
    char buffer[16];
    for (int i=0;i<16;i++) {
        unsigned long cur = x & 0xf;
        buffer[i] = cur < 10 ? ('0' + cur) : ('a' + cur - 10);
        x >>= 4;
    }
    for (int i=15;i>=0;i--) uart_put_c(buffer[i]);
    uart_put_c('\r');
    uart_put_c('\n');
}