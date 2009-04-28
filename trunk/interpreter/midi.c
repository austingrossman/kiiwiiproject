/* Structure that represents a MIDI Message */
typedef struct
{
   unsigned char length;
   unsigned char status;
   unsigned char data[3];
} MidiEvent;

/* Function initializes a MIDI Event */
void init_midievent(MidiEvent* event)
{  
   event->length = 0;
   event->status = 0;
   event->data[0] = 0;
   event->data[1] = 0;
   event->data[2] = 0;
}

/* Function outputs a MIDI event */
void output_midievent(MidiEvent event)
{
   unsigned char i;
   
   if(event.length)
   {
      putc(event.status);
   }
   
   for(i = 0; i < event.length - 1; i++)
   {
      putc(event.data[i]);
   }
}

/* Function retrieves the first midi status byte */
unsigned char get_midistatus()
{
   unsigned char rbyte = 0;
   
   while(rbyte < 128)
   {
      rbyte = getchar(midi);
   }
   
   return rbyte;
}

/* Function polls the UART until a properly formatted MIDI message is retrieved */
MidiEvent get_midievent(unsigned char status, unsigned char num_databytes)
{
   unsigned char rbyte, i;
   
   MidiEvent event;
   event.status = status;
   event.length = num_databytes + 1;
   
   for (i = 0; i < num_databytes; i++)
   {
      rbyte = getchar(midi);
   
      if(rbyte < 0x80)
      {
         event.data[i] = rbyte;
      }
      else
      {
         // This should be impossible, return error state
         LED_ON(YELLOW_LED);
      }
   }
   
   return event;
}

// Function returns the number of data bytes based on status byte
unsigned char lookup_mididatabytes(unsigned char status)
{
   if(status < 0xC0) {   //192
      return 2;
   } else if (status < 0xE0) { //224
      return 1;
   } else if (status < 0xF0) { //240
      return 2;
   } else if (status < 0xF1) { //241
      return 3;               // System exclusive: length varies
   } else if (status < 0xF2) { //242
      return 1;
   } else if (status < 0xF3) { //243
      return 2;
   } else if (status < 0xF4) { //244
      return 1;
   } else if (status < 0xF5) { //245 - 0xF4 - Wah
      return 2;
   } else if (status < 0xF6) { //246
      return 1;
   }else {
      return 0;
   }   
}

/* Function creates a MIDI event with the specified status and data bytes */
void set_midievent(MidiEvent* event, unsigned char status)
{
   event->length = 1;
   event->status = status;
}

/* Function creates a MIDI event with the specified status and data bytes */
void set_midievent(MidiEvent* event, unsigned char status, unsigned char data0)
{
   event->length = 2;
   event->status = status;
   event->data[0] = data0;
}

/* Function creates a MIDI event with the specified status and data bytes */
void set_midievent(MidiEvent* event, unsigned char status, unsigned char data0, unsigned char data1)
{
   event->length = 3;
   event->status = status;
   event->data[0] = data0;
   event->data[1] = data1;
}

/* Function creates a MIDI event with the specified status and data bytes */
void set_midievent(MidiEvent* event, unsigned char status, unsigned char data0, unsigned char data1, unsigned char data2)
{
   event->length = 4;
   event->status = status;
   event->data[0] = data0;
   event->data[1] = data1;
   event->data[2] = data2;
}
