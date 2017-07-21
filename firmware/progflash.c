#include <msp430.h>
#include <stdint.h>
#include "primething.h"

// S25FL127S commands
#define RDSR1 0x05  // Read Status Register 1

void initUart(void)
{
    UCA0BR0 = 8;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_6;
    UCA0CTL1 = UCSSEL_2;
}

void progFlash(void)
{
    uint8_t buffer[256];
    unsigned int i;

    // clear any junk from UART rx buffer
    i = UCA0RXBUF;

    // send "start" command
    UCA0TXBUF = 0xB8;

    // read 256 characters
    for(i=0; i<256; i++) {
        while(!(IFG2 & UCA0RXIFG));
        buffer[i] = UCA0RXBUF;
    }

    // program flash
    IO_OUT &= (~FLASH_CS);
    for(i=0; i<256; i++) {
        UCB0TXBUF = buffer[i];
        while(!(IFG2 & UCB0TXIFG));
    }
    IO_OUT |= FLASH_CS;
}
