#include <msp430.h> 
#include <primething.h>

/*
 * Pin Assignments:
 *  1 - DVCC
 *  2 - P1.0/CA0 - Comparator input (battery level) (TODO)
 *  3 - P1.1/UCA0RXD - RX
 *  4 - P1.2/UCA0TXD - TX
 *  5 - P1.3 - unused
 *  6 - P1.4 - unused
 *  7 - P1.5/UCB0CLK - SPI Clock
 *  8 - P2.0 - Startup mode "reset" (active low)
 *  9 - P2.1 - Startup mode "program" (active low)
 * 10 - P2.2 - Button "Press For Prime" (active low)
 * 11 - P2.3 - Button LED
 * 12 - P2.4 - Chip Select LCD
 * 13 - P2.5 - Chip Select Flash
 * 14 - P1.6/UCB0SOMI - SPI Data In
 * 15 - P1.7/UCB0SIMO - SPI Data Out
 * 16 - /RST
 * 17 - TEST
 * 18 - P2.7 - LCD Cmd/Data
 * 19 - P2.6 - unused
 * 20 - DVSS
 */

void initDevice(void);
void initLcd(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initDevice();
	initLcd();
	
	if(!(IO_IN & MODE_RESET))
	{
	    // Reset pin is selected (active low)

	    // For now... turn LED on.
	    IO_OUT |= BTN_LED;

	    return 0;
	}

	if(!(IO_IN & MODE_PROG))
	{
	    // Program pin is selected (active low)

	    displayString("ProgMode");
	    initUart();
	    progFlash();
	    displayString("Done");

	    return 0;
	}

	// Normal run mode selected

	return 0;
}

void initDevice(void)
{
    // Initialize clock to 1MHz using factory-programmed calibration values
    BCSCTL1 = CAL_BC1_1MHZ;
    DCOCTL = CAL_DCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);       // SMCLK = DCO = 1MHz

    // Port 3 needs to be terminated since not on physical package
    P3DIR = 0;
    P3OUT = 0;
    P3REN = 0xFF;

    // Port 1 configuration for UART and SPI interfaces, other pins unused
    P1DIR = BIT7 | BIT5 | BIT2;
    P1SEL = BIT7 | BIT6 | BIT5 | BIT2 | BIT1;
    P1SEL2 = BIT7 | BIT6 | BIT5 | BIT2 | BIT1;
    // Unused pins pull-down
    P1REN = BIT4 | BIT3 | BIT0;
    P1OUT = 0;

    // Port 2 is our general IO port.
    P2OUT = FLASH_CS | LCD_CS;  // start with SPI Chip Selects high
    P2DIR = LCD_MODE | FLASH_CS | LCD_CS | BTN_LED;
    P2REN = BIT6 | BUTTON | MODE_PROG | MODE_RESET;

    // Initialize SPI interface on USCI_B0
    UCB0CTL0 = UCCKPH | UCCKPL | UCMSB | UCMST | UCSYNC;
    UCB0BR0 = 0;
    UCB0BR1 = 0;
    UCB0CTL1 = UCSSEL_2;
}

void initLcd(void)
{
    __delay_cycles(40000);          // 40ms startup delay for LCD module

    IO_OUT &= (~LCD_MODE);          // LCD command mode
    IO_OUT &= (~LCD_CS);            // LCD chip select active

    UCB0TXBUF = 0x31;               // Function set
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us

    UCB0TXBUF = 0x14;               // Bias set
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us

    UCB0TXBUF = 0x55;               // Power control
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us delay

    UCB0TXBUF = 0x6D;               // Follower control
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us delay

    UCB0TXBUF = 0x7C;               // Contrast set
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us delay

    UCB0TXBUF = 0x30;               // Function set
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us delay

    UCB0TXBUF = 0x0C;               // Display on
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us delay

    UCB0TXBUF = 0x01;               // Clear display
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(1080);           // 1.08ms delay

    UCB0TXBUF = 0x06;               // Entry mode set
    while(!(IFG2 & UCB0TXIFG));
    while(!(UCB0STAT & UCBUSY));
    __delay_cycles(27);             // 27us delay

    IO_OUT |= LCD_CS;               // LCD chip select inactive
}
