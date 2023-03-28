#include "keyboard.h"
//need release scancodes for shift, tab, control, alt, caps, enter
enum uint8_t { //
zero, one, two, three, four, five, six, seven, eight, nine,

A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,

a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,

F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,

dash, equal, backspace, tab, left_bracket, right_bracket, semicolon, single_quote, backtick,

backslash, comma, period, slash, keypad, space,

enter_press, control_press, left_shift_press, right_shift_press, alt_press,caps_press,

left_shift_release, right_shift_release, tab_release, control_release, alt_release, capslock_release, enter_release
};

//key is the var name, value is the ascii value or scancode value corresponding to each keyboard press or release
typedef struct key_value_pair {
    uint8_t key;
    uint8_t value;
} kv_t;

kv_t ascii_table[ASCII_size] = {
    {zero, '0'}, {one, '1'}, {two, '2'}, {three, '3'}, {four, '4'}, {five, '5'}, {six, '6'},
    {seven, '7'}, {eight, '8'}, {nine, '9'},

    {A, 'A'}, {B, 'B'}, {C, 'C'}, {D, 'D'}, {E, 'E'}, {F, 'F'}, {G, 'G'}, {H, 'H'},
    {I, 'I'}, {J, 'J'}, {K, 'K'}, {L, 'L'}, {M, 'M'}, {N, 'N'}, {O, 'O'}, {P, 'P'}, {Q, 'Q'},
    {R, 'R'}, {S, 'S'}, {T, 'T'}, {U, 'U'}, {V, 'V'}, {W, 'W'}, {X, 'X'}, {Y, 'Y'}, {Z, 'Z'},
    
    {a, 'a'}, {b, 'b'}, {c, 'c'}, {d, 'd'}, {e, 'e'}, {f, 'f'}, {g, 'g'}, {h, 'h'},
    {i, 'i'}, {j, 'j'}, {k, 'k'}, {l, 'l'}, {m, 'm'}, {n, 'n'}, {o, 'o'}, {p, 'p'}, {q, 'q'},
    {r, 'r'}, {s, 's'}, {t, 't'}, {u, 'u'}, {v, 'v'}, {w, 'w'}, {x, 'x'}, {y, 'y'}, {z, 'z'},

    {dash, '-'}, {equal, '='} , {backspace, 0x08}, {tab, 0x09} , {left_bracket, '['}, 
    {right_bracket, ']'}, {semicolon, ';'}, {single_quote, '\''},  {backtick, '`'},

    {backslash, '\\'},  {comma,','} , {period, '.'} ,{slash, '/'}, {keypad, '*'}, {space,' '},

    {enter_press, '\n'} // 
};

kv_t scancode_table[scancode_size] = {
{zero, 0x0B}, {one, 0x02}, {two, 0x03}, {three, 0x04}, {four, 0x05}, {five, 0x06}, {six, 0x07},
{seven, 0x08}, {eight, 0x09}, {nine, 0x0A},

{a, 0x1e}, {b, 0x30}, {c, 0x2e}, {d, 0x20}, {e, 0x12}, {f, 0x21}, {g, 0x22}, {h, 0x23},
{i, 0x17}, {j, 0x24}, {k, 0x25}, {l, 0x26}, {m, 0x32}, {n, 0x31}, {o, 0x18}, {p, 0x19}, {q, 0x10},
{r, 0x13}, {s, 0x1f}, {t, 0x14}, {u, 0x16}, {v, 0x2f}, {w, 0x11}, {x, 0x2d}, {y, 0x15}, {z, 0x2c},
    
{F1, 0x3B}, {F2, 0x3C}, {F3, 0x3D}, {F4, 0x3E}, {F5, 0x3F}, {F6, 0x40}, {F7, 0x41}, {F8, 0x42}, 
{F9, 0x43}, {F10, 0x44}, {F11, 0x57}, {F12, 0x58},

{dash, 0x0C}, {equal, 0x0D} , {backspace, 0x0E}, {tab, 0x0F} , {left_bracket, 0x1A}, 
{right_bracket, 0x1B}, {semicolon, 0x27}, {single_quote, 0x28},  {backtick, 0x29},

{backslash, 0x2B},  {comma,0x33} , {period, 0x34} ,{slash, 0x35}, {keypad, 0x37}, {space, 0x39},

{enter_press, 0x1C},

{control_press, 0x1D}, {left_shift_press, 0x2A}, {right_shift_press, 0x36},  {alt_press, 0x38}, {caps_press, 0x3A},

{left_shift_release, 0xAA}, {right_shift_release, 0xB6}, {tab_release, 0x8F}, {control_release, 0x9D}, 
{alt_release, 0xB8}, {capslock_release, 0xBA}, {enter_release, 0x9C}
};





/* Reference: https://wiki.osdev.org/Interrupts#From_the_keyboard.27s_perspective */
/* Reference: https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1 */

 /* void keyboard_init();
 * Inputs: void
 * Return Value: none
 * Function: enables IRQ1 for Keyboard Interrupt*/

void keyboard_init(){
    ctrl_flag  = 0;
    caps_flag  = 0;
    shift_flag = 0;
    enable_irq(1); //enables the keyboard interrupt line on the pic
}


 /* void keyboard_handler();
 * Inputs: void
 * Return Value: none
 * Function: Handler in order to read data port from the keyboard controller and prints the corresponding character 
 * to the screen based on what key is pressed and released which is then ended by send_eoi to indicate end of 
 * interrupt */


void keyboard_handler(){
    // x60 is a post for the keyboard data - 0x58 is the number of keys that could be interpreted
    cli();
    uint8_t input;
    uint8_t output;
    uint32_t i;
    uint8_t buttons = inb(0x60);   // grab data from the press, 0x60 represents the keyboard data port
    for(i = 0; i < scancode_size; i++){
        if(scancode_table[i].value == buttons){
            input = scancode_table[i].key; //find key by parsing through scancode table to match keyboard data
        }
    }
    for(i = 0; i < ASCII_size; i++){
        if(input == ascii_table[i].key){
            output = ascii_table[i].value; //based on the key name above, find and store the corresponding ascii value 
            break;
        }
    }
    /*if (buttons == 0x3A) //caps lock pressed
    {
        caps_flag = (caps_flag == 1) ? 0 : 1;
        send_eoi(1);
    }
    else if (buttons == 0xBA) //caps lock released
    {
        send_eoi(1);
    }
    else if (buttons == 0x1D) //ctrl pressed
    {
        ctrl_flag = 1;
        send_eoi(1);
    }
    else if (buttons == 0x9D) //ctrl released
    {
        ctrl_flag = 0;
        send_eoi(1);
    }
    else if (buttons == 0x2A) //shift pressed
    {
        shift_flag = 1;
        send_eoi(1);
    }
    else if (buttons == 0xAA) //shift released
    {
        shift_flag = 0;
        send_eoi(1);
    }*/
    //if(buttons < 0x58)     // range check 
    //{

    switch (buttons)
    {
        case 0x3A : //caps lock pressed

            caps_flag = (caps_flag == 1) ? 0 : 1; //checking caps_lock toggle
            send_eoi(1);  //end of interrupt;
            sti();
            return;

        case 0xBA : //caps lock released

            send_eoi(1);  //end of interrupt;
            break;

        case 0x1D : //ctrl pressed

            ctrl_flag = 1; //determines if ctrl is currently active
            send_eoi(1);  //end of interrupt;
            sti();
            return;

        case 0x9D : //ctrl released
            
            ctrl_flag = 0;
            send_eoi(1);  //end of interrupt;
            break;

        case 0x2A : //shift pressed

            shift_flag = 1; //determines if shift is currently active
            send_eoi(1);  //end of interrupt;
            sti();
            return;

        case 0xAA : //shift released

            shift_flag = 0;
            send_eoi(1);  //end of interrupt;
            break;

        default:
            break;
    }
    //}
    if(buttons < 0x58)     // range check 
    {
        //keyboard_handler();
        if(ctrl_flag && !shift_flag && (output == 'l' || output == 'L'))
        {
            clear();
            reset_cursor();
            send_eoi(1);  //end of interrupt;
            sti();
            return;
        }

        //checking for caps lock or shift
        if((shift_flag && output <= 'z' && output >= 'a') != (caps_flag && output <= 'z' && output >= 'a')){
            putc(output - 0x20); // print to terminal with caps offset(32)
            keyboard_buffer(output - 0x20);
        }
        else if(shift_flag && output == '1'){
            output = '!';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '2'){
            output = '@';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '3'){
            output = '#';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '4'){
            output = '$';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '5'){
            output = '%';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '6'){
            output = '^';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '7'){
            output = '&';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '8'){
            output = '*';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '9'){
            output = '(';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '0'){
            output = ')';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '-'){
            output = '_';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '='){
            output = '+';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '['){
            output = '{';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == ']'){
            output = '}';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == ';'){
            output = ':';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '\''){
            output = '\"';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == ','){
            output = '<';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '.'){
            output = '>';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '\\'){
            output = '|';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '/'){
            output = '?';
            putc(output);
            keyboard_buffer(output);
        }
        else if(shift_flag && output == '`'){
            output = '~';
            putc(output);
            keyboard_buffer(output);
        }
        else if(buttons == 0x0F){ //aka tab
            output = ' ';
            int j;
            for(j = 0; j < 4; j++){ //want 4 spaces
                putc(output);
                keyboard_buffer(output);
            }
            /*output = '    ';
            uint32_t tabbuf[4] = "    ";
            int j;
            for (j = 0; j < 4; j++){
                putc(tabbuf[j]);
            }
            keyboard_buffer(output);*/
        }
        else{
            putc(output);
            keyboard_buffer(output); //function call to keyboard buffer in terminal.c to store output char into a keyboard buffer
                             //for terminal read and write
        }
    }
    send_eoi(1);     // end of interrupt
    sti();
}



