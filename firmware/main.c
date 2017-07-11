#include <msp430.h> 

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


#define IO_DIR      P2DIR
#define IO_OUT      P2OUT
#define IO_IN       P2IN
#define IO_IE       P2IE
#define IO_IES      P2IES
#define IO_IFG      P2IFG
#define BUTTON      BIT2
#define MODE_RESET  BIT0
#define MODE_PROG   BIT1
#define BTN_LED     BIT3
#define LCD_CS      BIT4
#define FLASH_CS    BIT5
#define LCD_MODE    BIT7

void initDevice(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initDevice();
	
	if(!(IO_IN & MODE_RESET))
	{
	    // Reset pin is selected (active low)
	    return 0;
	}

	if(!(IO_IN & MODE_PROG))
	{
	    // Program pin is selected (active low)
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
    P2DIR = LCD_MODE | FLASH_CS | LCD_CS | BTN_LED;
    P2REN = BIT6 | BUTTON | MODE_PROG | MODE_RESET;
    P2OUT = FLASH_CS | LCD_CS;  // start with SPI Chip Selects high

    // Initialize SPI interface on USCI_B0
    UCB0CTL0 = UCMST | UCSYNC;
    UCB0CTL1 = UCSSEL_2;

}
