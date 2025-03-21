#include <ti/screen.h>
#include <ti/getcsc.h>
#include "pi.h"

#define MAXROWS 10
#define MAXCOLS 26

// get raw key code
uint8_t getkey(void)
{
    uint8_t key = 0;
    while (!key){
        key = os_GetCSC();
    }
    return key;
}

// get the key and see if it could be converted to 0-9
// if not, the default value (sentinel) is 10
uint8_t parseKey(uint8_t key)
{
    switch(key){
        case sk_DecPnt:
        case sk_0:
            return 0;
            break;
        case sk_1:
            return 1;
            break;
        case sk_2:
            return 2;
            break;
        case sk_3:
            return 3;
            break;
        case sk_4:
            return 4;
            break;
        case sk_5:
            return 5;
            break;
        case sk_6:
            return 6;
            break;
        case sk_7:
            return 7;
            break;
        case sk_8:
            return 8;
            break;
        case sk_9:
            return 9;
            break;

        // sentinel, for error key
        // where the key press is not 0-9
        default:
            return 10;
            break;
    }
}

// add spaces for the particular line all the way to the end of the line
void pad_spaces()
{
    unsigned int cc, cr;
    os_GetCursorPos(&cr, &cc);

    for (uint8_t i = cc ; i < MAXCOLS; i++)
    {
        os_PutStrFull(" ");
    }
    os_SetCursorPos(cr, cc);
}

// printing a number at a cursor, which is a direct substitute for
// sprintf(buffer, "%d", num)
void print_num_at_cursor(uint32_t num)
{
    // initialize output
    char output[2];
    output[1] = 0;

    // index and number array
    uint8_t i = 0;
    uint8_t num_array[6];
    do
    {
        num_array[i++] = num %10;
        num /= 10;
    } while (num != 0);

    // print the array in reverse
    while (i != 0)
    {
        output[0] = '0'+num_array[--i];
        os_PutStrFull(output);
    }
}

// gets the i-th value of pi from pi_index
uint8_t fetch_pi_index(uint32_t i)
{
    return i%2 ? (pi_index[i/2] & 0x0f) : ((pi_index[i/2] & 0xf0) >> 4);
}

// prints the values of pi backwards with rightmost digit being points
void print_pi(uint32_t max_digit)
{
    char temp[MAXCOLS+1];

    // index of the temporary variable
    // start is 0 because it a default.
    uint8_t history_index = 0;

    // start (should) be able to go up to 100k
    uint32_t start = 0;

    // Set the start of the loop
    if (max_digit == MAXCOLS)
    {
        start = 1;
    }
    else if (max_digit > MAXCOLS)
    {
        start = max_digit - MAXCOLS;
    }

    // loops through the digits of pi.
    for (uint32_t i = start; i < max_digit; i++)
    {
        // the decimal logic
        if (i == 1 && max_digit <= MAXCOLS)
        {
            temp[history_index++] = '.';
        }
        // otherwise, populating the buffer
        temp[history_index++] = '0' + fetch_pi_index(i);
    }

    // putting a 0 at the end
    temp[history_index] = 0;

    os_PutStrFull(temp);
}

// End screen prompting the user to hit Clear to end
void end_screen()
{
    uint8_t key = 0;
    os_SetCursorPos(5, 0);
    os_PutStrFull("GG! Hit \xc1");
    os_PutStrFull("Clear] to End.");

    while (key != sk_Clear){
        key = getkey();
    }
}

// the code for the game, with a return_on_incorrect parameter
// if the parameter == 0 then it's in practice, else quiz mode
void game(uint8_t return_on_incorrect)
{
    // Waits for a key
    uint8_t key = 0;
    uint8_t parsed = 10;
    uint32_t points = 0;

    // used to check if the correct key is pressed
    uint8_t check;

    // prints one character
    os_SetCursorPos(0, 0);
    os_PutStrLine("Type the next digit!");

    // do loop first, then check if clear button pressed
    do
    {
        // Ensure we have a valid key press. Otherwise
        // disregard, and go back into looping.

        // The sentinel for invalid parsing is 10
        // or the key has been a clear
        while (parsed == 10 && key != sk_Clear)
        {
            key = getkey();
            parsed = parseKey(key);
        }

        // get the bitmasked value from the pi-table
        // for example:
        //  0x31      0x31
        //    &&        &&
        //  0xf0  or  0x0f
        // =0x30     =0x01
        // >>4=3     =1
        check = fetch_pi_index(points);

        os_SetCursorPos(2, 0);
        // check if the correct key has been pressed
        if (check == parsed)
        {
            // increment points due to correct button press
            points++;

            // when it's a correct button press
            // clear out the previous line call
            pad_spaces();
        }

        // incorrect key pressed but we are in practice mode
        else if (!return_on_incorrect)
        {
            os_PutStrFull("Next: ");
            char temp[2];
            temp[0] = '0' + check;
            temp[1] = 0;
            os_PutStrFull(temp);
        }

        // incorrect key pressed but we are in quiz mode
        else
        {
            os_SetCursorPos(1, 0);
            os_PutStrFull("Score: ");
            print_num_at_cursor(points);
            os_SetCursorPos(2, 0);
            os_PutStrFull("You lose.");
            end_screen();
            return;
        }

        // write the string and pad spaces (for the end of the line)
        // display practice mode score:
        os_SetCursorPos(1, 0);
        os_PutStrFull("Score: ");
        print_num_at_cursor(points);

        // check if the points have reached the max digit size:
        if(points == 2*ARRSIZE)
        {
            os_SetCursorPos(2, 0);
            os_PutStrFull("How did you do that?!");
            os_SetCursorPos(3, 0);
            os_PutStrFull("You typed 100,000 digits!");
            pad_spaces();
            os_SetCursorPos(4, 0);
            os_PutStrFull("Hacker perhaps?");
            end_screen();
            return;
        }


        // print the digits of pi at this location
        os_SetCursorPos(3, 0);

        print_pi(points);

        // reset parsed for polling again up top
        parsed = 10;
    } while (key != sk_Clear);
}

// get an input from the user, and give a number
// it's a (very) rough equivalent to the python's int(input(prompt))
uint32_t input_func(char* prompt){
    os_ClrLCDFull();
    // input buffer
    char input[7];

    // converting string to number
    uint32_t parsed_number = 0;

    // set cursor position to top left
    os_SetCursorPos(0, 0);

    // prompt
    os_GetStringInput(prompt,input, 7);

    // read input (string) and convert to number
    // also make sure they're all numbers
    uint8_t i = 0;
    while (input[i] != 0)
    {
        parsed_number *= 10;
        parsed_number += (input[i] - '0');
        // make sure they're all numbers
        if (input[i] < '0' || input[i] > '9')
        {
            os_SetCursorPos(1, 0);
            os_PutStrFull("The input you provided");
            os_SetCursorPos(2, 0);
            os_PutStrFull("Is not a valid number.");
            getkey();
            // larger than max, which is invalid.
            return 0;
        }
        i++;
    }

    os_SetCursorPos(7, 0);
    os_PutStrLine("explore mode!");
    os_SetCursorPos(8, 0);
    os_PutStrLine("You can try to guess the");
    os_SetCursorPos(9, 0);
    os_PutStrLine("next number as well!");

    return parsed_number;
}

// exploring the digits of pi
void pi_explorer()
{
    uint32_t number_result = input_func("Jump to \xc4 digit: ");

    // scroll through the digits of pi.
    uint8_t key = 0;
    while (key != sk_Clear) {
        if (key == sk_Left && number_result != 0)
        {
            number_result--;
        }
        else if (key == sk_Right && number_result < 2*ARRSIZE)
        {
            number_result++;
        }
        else if (key == sk_Enter)
        {
            number_result = input_func("Jump to \xc4 digit: ");
        }

        // to prevent the number from exceeding physical array size
        // go to last index (for number_result)
        if (number_result >= 2*ARRSIZE)
        {
            number_result = 2*ARRSIZE;
        }

        // pressed numpad key matches next digit and number result is not the last digit
        if (parseKey(key) == fetch_pi_index(number_result) && number_result != 2*ARRSIZE)
        {
            number_result++;
        }

        // display it
        os_SetCursorPos(0, 0);
        os_PutStrFull("Rightmost digit: ");
        print_num_at_cursor(number_result);
        pad_spaces();
        os_SetCursorPos(1, 0);
        print_pi(number_result);
        pad_spaces();

        // get the next key press
        key = getkey();
    }
}

int main(void)
{
    // Clear the homescreen
    os_ClrHome();

    // initializes the mode and key
    int8_t mode = 0;
    uint8_t key = 0;

    // splash screen
    os_PutStrLine("100,000 digits of \xc4!");
    os_SetCursorPos(1, 0);
    os_PutStrLine("Type the first digit of \xc4");
    os_SetCursorPos(8, 0);
    os_PutStrFull("\xc1");
    os_PutStrFull("Clear] to end any time!");

    // select menu
    while (key != sk_Enter &&
           key != sk_Clear &&
           key != sk_Right &&
           key != sk_1 &&
           key != sk_2 &&
           key != sk_3)
    {
        os_SetCursorPos(3, 0);
        os_PutStrLine("Select mode:");

        if (key == sk_Mode || key == sk_Down)
        {
            mode++;
        }
        else if (key == sk_Up)
        {
            mode--;
        }

        // if mode is less than 0
        // roll over logic, back to 2
        if (mode < 0)
        {
            mode = 2;
        }

        // take the remainder of 3
        mode = mode % 3;

        os_SetCursorPos(4, 0);
        os_PutStrLine("  1. Quiz mode");
        os_SetCursorPos(5, 0);
        os_PutStrLine("  2. Practice mode");
        os_SetCursorPos(6, 0);
        os_PutStrLine("  3. Pi-Digits explorer");

        os_SetCursorPos(mode+4, 0);
        os_PutStrFull(">");

        // get the next key
        key = getkey();
    }

    // see if the game is meant to end
    if (key == sk_Clear)
    {
        return 0;
    }
    // game mode has been shortcut-selected
    else if (1 <= parseKey(key) && parseKey(key) <= 3)
    {
        mode = parseKey(key)-1;
    }

    // game mode selector
    os_ClrLCDFull();
    os_SetCursorPos(7, 0);
    switch(mode)
    {
        case 0:
            os_PutStrLine("quiz mode!");
            game(1);
            break;
        case 1:
            os_PutStrLine("practiCE mode!");
            game(0);
            break;
        case 2:
            pi_explorer();
            break;
    }

    return 0;
}
