#include <msp430.h>
#include <primething.h>

void displayString(char *string)
{
    IO_OUT &= (~LCD_MODE);          // LCD command mode
    IO_OUT &= (~LCD_CS);            // LCD chip select active

    UCB0TXBUF = 0x01;               // Clear display
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(1080);           // 1.08ms delay

    IO_OUT |= LCD_CS;               // LCD chip select inactive
    IO_OUT = LCD_MODE;              // LCD data mode
    IO_OUT &= (~LCD_CS);            // LCD chip select active

    while(*string)
    {
        UCB0TXBUF = *string;
        while(!(IFG2 & UCB0TXIFG));
        while(!(UCB0STAT & UCBUSY));
        __delay_cycles(27);          // 27us delay
        string++;
    }

    IO_OUT |= LCD_CS;               // LCD chip select inactive
    IO_OUT |= LCD_MODE;             // LCD command mode
}
