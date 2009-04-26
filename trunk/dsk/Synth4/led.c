#include "common.h"

unsigned char bcd = 0;

void U1(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	UART_EnableLed1(toggle);
}
void U2(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	UART_EnableLed2(toggle);
}
void M1(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	MIDI_EnableLed1(toggle);
}
void M2(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	MIDI_EnableLed2(toggle);
}
void L0(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(0,toggle);
}
void L1(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(1,toggle);
}
void L2(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(2,toggle);
}
void L3(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(3,toggle);
}
void L4(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(4,toggle);
}
void L5(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(5,toggle);
}
void L6(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(6,toggle);
}
void L7(int i) {
	static unsigned int toggle = 0;
	if(i==-1) toggle^=1;
	else if(i==0) toggle=0;
	else toggle=1;
	GPIO_EnableLed(7,toggle);
}
