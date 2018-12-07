#include <lpc214x.h>
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header


#define bit(x) (1<<x)
 
void lcd_init(void);
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay(void);

