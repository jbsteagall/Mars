
#include <project.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *array;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = (int *)malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}


const char welcome_string[] = "Hello World!\n\r";
const char new_line[] = "\n\r";
const char input_pwm[] = "Please input number between 1000 and 11000 followed by '+' \n\r";
const char pwm_value[] = "The PWM value has changed\n\r";

volatile uint8_t inc_flag = 1;
volatile uint16_t compare_val = 0;

void PWM_changed(){
    uint8_t i;
	for(i=0; i<strlen(pwm_value); i++) { // display the welcome string
		UART_UartPutChar(pwm_value[i]);
	}    
    
}


void enter(){
    uint8_t i;
	for(i=0; i<strlen(new_line); i++) { // display the welcome string
		UART_UartPutChar(new_line[i]);
	}    
}

void PWM_enter(){
    uint8_t i;
	for(i=0; i<strlen(input_pwm); i++) { // display the welcome string
		UART_UartPutChar(input_pwm[i]);
	}    
}



uint16_t getpwm_val(Array a)
{
    int one   = a.array[0] - '0';
    //int two   = a.array[1] - '0';
    //int three = a.array[2] - '0';
    //int four  = a.array[3] - '0';
    
    uint16_t val = 0;
    //val = one*1000 + two*100 + three*10 + four;
    if(one == 1) val = 1000;
    else if(one == 2) val = 2000;
    else if(one == 3) val = 3000;
    else if(one == 4) val = 4000;
    else if(one == 5) val = 5000;
    else if(one == 6) val = 6000;
    else if(one == 7) val = 7000;
    else if(one == 8) val = 8000;
    else if(one == 9) val = 9000;
    else if(one == 0) val = 0;
    else val = 5555;
    return val;   
}

int main()
{
	uint8_t i;
	char c;
    Array a;
    int g = 5;
    uint16_t pwm_val = 0;
    initArray(&a, g);
    
    UART_Start();                             // start the UART peripheral
	
    
	while(SW1_Read());                        // wait for button press to continue (gives you time to switch to a terminal window)

	for(i=0; i<strlen(welcome_string); i++) { // display the welcome string
		UART_UartPutChar(welcome_string[i]);
	}
    for(;;) {
        
		c = UART_UartGetChar();               // get typed character

        
        if(c) {                               // if it's a valid character
            //g += 1;
            insertArray(&a, c);   
            UART_UartPutChar(c);              // echo it in the terminal window
		}
        
        if(c == ';') {
            enter();
            for(i = 0; i < a.used; i++){
                UART_UartPutChar(a.array[i]);
            }
            enter();
            freeArray(&a);     
        }
        
        if(c == 'p')
        {
            PWM_Start();                                  // start the PWM peripheral
	        enter();
            PWM_enter();
            enter();
            freeArray(&a);
        }    
        
        if(c == '?')
        {

            pwm_val = getpwm_val(a);
            
            UART_UartPutChar(a.array[0]);
            UART_UartPutChar(a.array[1]);
            UART_UartPutChar(a.array[2]);
            UART_UartPutChar(a.array[3]);
            enter();
            PWM_changed();
            freeArray(&a);
            PWM_WriteCompare(pwm_val);
            enter();
        }
        
        
        	/*if(compare_val > 9000) { inc_flag = 0; } // set increment flag to false
		    if(compare_val < 1000) { inc_flag = 1; }  // set increment flag to true
		    if(inc_flag) {                            // if increment flag is true
		    	compare_val += 500;                   // increment compare value
		    }else{                                    // else if increment flag is false
			    compare_val -= 500;                   // decrement compare value
		    }
		    PWM_WriteCompare(compare_val);            // write new compare value to PWM
		    CyDelay(500);                              // wait 25ms before looping again
    */
        
        //else{
        //    insertArray(&a, c);   
        //}
        
    }
}