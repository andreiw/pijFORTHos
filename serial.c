/*
 * serial.c -- Raspberry Pi serial i/o (UART) routines written in C
 *
 * Some of this code was inspired by bare-metal examples
 * from David Welch at https://github.com/dwelch67/raspberrypi
 */
#include "serial.h"

//#define USE_SERIAL_UART0    /* select full UART for serial i/o */
#define USE_SERIAL_UART1    /* select mini UART for serial i/o */

#define GPFSEL1         0x20200004
#define GPSET0          0x2020001c
#define GPCLR0          0x20200028
#define GPPUD           0x20200094
#define GPPUDCLK0       0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504c
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505c
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

/*
 * Initialize serial UART
 */
void
serial_init()
{
#ifdef USE_SERIAL_UART0
#error NOT IMPLEMENTED!
#endif /* USE_SERIAL_UART0 */
#ifdef USE_SERIAL_UART1
    u32 r0;
    int n;

    PUT_32(AUX_ENABLES, 1);
    PUT_32(AUX_MU_IER_REG, 0);
    PUT_32(AUX_MU_CNTL_REG, 0);
    PUT_32(AUX_MU_LCR_REG, 3);
    PUT_32(AUX_MU_MCR_REG, 0);
    PUT_32(AUX_MU_IER_REG, 0);
    PUT_32(AUX_MU_IIR_REG, 0xc6);
    PUT_32(AUX_MU_BAUD_REG, 270);  // ((250,000,000/115200)/8)-1 = 270

    r0 = GET_32(GPFSEL1);
    r0 &= ~(7 << 12); // gpio pin 14
    r0 |= 2 << 12;    //   alt5 = mini UART transmit (TX)
    r0 &= ~(7 << 15); // gpio pin 15
    r0 |= 2 << 15;    //   alt5 = mini UART receive (RX)
    PUT_32(GPFSEL1, r0);

    PUT_32(GPPUD, 0);
    n = 150;
    while (n-- > 0) {  // wait for (at least) 150 clock cycles
        NO_OP();
    }

    r0 = (1 << 14) | (1 << 15);
    PUT_32(GPPUDCLK0, r0);
    n = 150;
    while (n-- > 0) {  // wait for (at least) 150 clock cycles
        NO_OP();
    }

    PUT_32(GPPUDCLK0, 0);

    PUT_32(AUX_MU_CNTL_REG, 3);
#endif /* USE_SERIAL_UART1 */
}

/*
 * Serial input ready != 0, wait == 0
 */
int
serial_in_ready()
{
#ifdef USE_SERIAL_UART0
#error NOT IMPLEMENTED!
#endif /* USE_SERIAL_UART0 */
#ifdef USE_SERIAL_UART1
    return GET_32(AUX_MU_LSR_REG) & 0x01;
#endif /* USE_SERIAL_UART1 */
}

/*
 * Raw input from serial port
 */
int
serial_in()
{
#ifdef USE_SERIAL_UART0
#error NOT IMPLEMENTED!
#endif /* USE_SERIAL_UART0 */
#ifdef USE_SERIAL_UART1
    return GET_32(AUX_MU_IO_REG);
#endif /* USE_SERIAL_UART1 */
}

/*
 * Serial output ready != 0, wait == 0
 */
int
serial_out_ready()
{
#ifdef USE_SERIAL_UART0
#error NOT IMPLEMENTED!
#endif /* USE_SERIAL_UART0 */
#ifdef USE_SERIAL_UART1
    return GET_32(AUX_MU_LSR_REG) & 0x20;
#endif /* USE_SERIAL_UART1 */
}

/*
 * Raw output to serial port
 */
int
serial_out(u8 data)
{
#ifdef USE_SERIAL_UART0
#error NOT IMPLEMENTED!
#endif /* USE_SERIAL_UART0 */
#ifdef USE_SERIAL_UART1
    PUT_32(AUX_MU_IO_REG, (u32)data);
    return (int)data;
#endif /* USE_SERIAL_UART1 */
}

/*
 * Blocking read from serial port
 */
int
serial_read()
{
    while (!serial_in_ready())
        ;
    return serial_in();
}

/*
 * Blocking write to serial port
 */
int
serial_write(u8 data)
{
    while (!serial_out_ready())
        ;
    return serial_out(data);
}

/*
 * Print a C-string, character-by-character
 */
void
serial_puts(char* s)
{
    int c;

    while ((c = *s++) != '\0') {
        serial_write((u8)c);
    }
}

/*
 * Print n repetitions of character c
 */
void
serial_rep(int c, int n)
{
    while (n-- > 0) {
        serial_write((u8)c);
    }
}
