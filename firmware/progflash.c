#include <msp430.h>
#include <primething.h>

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

}
