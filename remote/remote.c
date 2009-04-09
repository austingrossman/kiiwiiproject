#include <18f4520.h>
#device ICD=TRUE
#fuses HS,NOLVP,NOWDT
#use delay(clock=20000000)
#use rs232 (baud=9600, STREAM=TERM, xmit=PIN_C6, rcv=PIN_C7)
#use rs232 (baud=9600, STREAM=XBEE, xmit=PIN_B3, rcv=PIN_B2,ERRORS,FORCE_SW,parity=N,bits=8) //A1 goes to TX0, A2 goes to RX1
#include <stdlib.h>
#include <input.c>
//#include <math.h>

#define GREEN_LED       PIN_A5      // define the pins
#define YELLOW_LED      PIN_B4
#define RED_LED         PIN_B5
#define PUSH_BUTTON     PIN_A4
#define BUTTON1         PIN_C0
#define BUTTON2         PIN_C1
#define BUTTON3         PIN_C2
#define VOLUME          0x01        // button values
#define PITCH           0x02
#define WAH             0x03
#define XOFFSET         41          // raw adc offsets
#define YOFFSET         49
#define ZOFFSET         48
#define XMAX            82          // total adc ranges
#define YMAX            85
#define ZMAX            84

#INT_EXT2
void wegetsignal() {                // toggle red when remote receives data (no application)
 ext_int_edge( 2, H_TO_L);
}

char min(char a, char b) {
   if (a < b) return a;
   else return b;
}

void main() {
    char ADC_1, ADC_2, ADC_3, i;
    long buffer;
    enable_interrupts(INT_EXT2);
    enable_interrupts(global);

    setup_timer_0(RTCC_INTERNAL);
    setup_adc_ports (AN0_TO_AN3_ANALOG);
    setup_adc(ADC_CLOCK_INTERNAL);

    while(TRUE){
        if (input(BUTTON1) == 1) {       // volume button pushed
            set_adc_channel(1);              // y axis
            for (i = 0; i < 8; i++) {        // average 8 samples
                buffer += read_adc();
            }
            buffer = buffer >> 3;
            ADC_1 = min((((long)buffer - YOFFSET) * 127) / YMAX, 127) & 0xFF;
            delay_us(25);
            
            output_low(GREEN_LED);
            
            fputc(VOLUME,TERM);
            fputc(ADC_1,TERM);
            
            fputc(VOLUME,XBEE);
            fputc(ADC_1,XBEE);
        }
        else if (input(BUTTON2) == 1) {                        // pitch button pushed
            set_adc_channel(2);              // z axis
            for (i = 0; i < 8; i++) {        // average 8 samples
                buffer += read_adc();
            }
            buffer = buffer >> 3;
            ADC_2 = min((((long)buffer - ZOFFSET) * 127) / ZMAX, 127) & 0xFF;
            delay_us(25);
            
            output_low(RED_LED);
            
            fputc(PITCH,TERM);
            fputc(ADC_2,TERM);
            
            fputc(PITCH,XBEE);
            fputc(ADC_2,XBEE);
        }
        else if (input(BUTTON3) == 1) {                        // wah button pushed
            set_adc_channel(3);              // x axis
            for (i = 0; i < 8; i++) {        // average 8 samples
                buffer += read_adc();
            }
            buffer = buffer >> 3;
            ADC_3 = min((((long)read_adc() - XOFFSET) * 127) / XMAX, 127) & 0xFF;
            delay_us(25);
            
            output_low(YELLOW_LED);
            
            fputc(WAH,TERM);
            fputc(ADC_3,TERM);
            
            fputc(WAH,XBEE);
            fputc(ADC_3,XBEE);
        }
        
        output_high(GREEN_LED);
        output_high(RED_LED);
        output_high(YELLOW_LED);
    }
}
