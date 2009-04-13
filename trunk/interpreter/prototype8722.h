#include <18F8722.h>
#device icd=true
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)

#define GREEN_LED PIN_A5
#define YELLOW_LED PIN_B4
#define RED_LED PIN_B5

#define delay_s(x) delay_ms(x*1000);
#define led_on(x) output_low(x);
#define led_off(x) output_high(x);
#define toggle_led(x) output_toggle(x);
#define led_toggle(x, y) output_toggle(x); delay_ms(y*1000);
