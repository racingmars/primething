#ifndef PRIMETHING_H_
#define PRIMETHING_H_

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

// Functions from util.c
void displayString(char *string);

// Functions from progflash.c
void initUart(void);
void progFlash(void);

#endif /* PRIMETHING_H_ */
