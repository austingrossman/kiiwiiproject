#define BUFFER_SIZE 3 // = data bytes + 1

BYTE buffer[BUFFER_SIZE];
BYTE next_in = 0;
BYTE next_out = 0;
BYTE strt_rc = FALSE;
BYTE stop_rc = FALSE;

#define bkbhit (next_in!=next_out)

BYTE bgetc() {
   BYTE c;

   while(!bkbhit) ;
   c=buffer[next_out];
   next_out=(next_out+1) % BUFFER_SIZE;
   return(c);
}

void bsetc(BYTE b) {
   int t;
   
   buffer[next_in] = b;
   t=next_in;
   next_in=(next_in+1) % BUFFER_SIZE;
   if(next_in==next_out)
     next_in=t;           // Buffer full !!
}

void reset_buffer(){
   next_in = 0;
   next_out = 0;
   strt_rc = FALSE;
   stop_rc = FALSE;
}

