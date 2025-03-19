#include <stdint.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <debug.h>
#include "pi.h"

#define MAXROWS 10
#define MAXCOLS 26


uint8_t getkey(void)
{
    uint8_t key = 0;
    while (!key){
        key = os_GetCSC();
    }
    return key;
}

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
        default:
            return 10;
            break;
    }
}

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


void print_num_at_cursor(uint32_t num)
{
    // initialize output
    char output[2];
    output[1] = 0;

    // index and number array
    uint8_t i = 0;
    uint8_t num_array[6];
    while(num != 0)
    {
        num_array[i++] = num %10;
        num /= 10;
    }

    // print the array in reverse
    while (i != 0)
    {
        output[0] = '0'+num_array[--i];
        os_PutStrFull(output);
    }
}

uint8_t fetch_pi_index(uint32_t i)
{
    return i%2 ? (pi_index[i/2] & 0x0f) : ((pi_index[i/2] & 0xf0) >> 4);
}

void game(uint8_t return_on_incorrect)
{
    // Waits for a key
    uint8_t key = 0;
    uint8_t parsed;
    uint32_t points = 0;

    // prints one character
    char temp[MAXCOLS+1];
    os_SetCursorPos(0, 0);
    os_PutStrLine("Type the next digit!");

    while (key != sk_Clear)
    {
        key = getkey();
        parsed = parseKey(key);

        // a valid 0-9 key press
        if (parsed >= 0 && parsed < 10)
        {
            uint8_t check;

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
            else
            {
                if (!return_on_incorrect)
                {
                    os_PutStrFull("Next: ");
                    temp[0] = '0' + check;
                    temp[1] = 0;
                    os_PutStrFull(temp);
                }
                else
                {
                    os_PutStrFull("You lose.");
                    getkey();
                    return;
                }
            }

            // write the string and pad spaces (for the end of the line)
            // display practice mode score:
            os_SetCursorPos(1, 0);
            os_PutStrFull("Score: ");
            print_num_at_cursor(points);

        }

        // print the digits of pi at this location
        os_SetCursorPos(3, 0);

        // index of the temporary variable
        // start is 0 because it a default.
        uint8_t history_index = 0;
        uint8_t start = 0;

        // Set the start of the loop
        if (points == MAXCOLS)
        {
            start = 1;
        }
        else if (points > MAXCOLS)
        {
            start = points - MAXCOLS;
        }

        // loops through the digits of pi.
        for (uint32_t i = start; i < points; i++)
        {
            // the decimal logic
            if (i == 1 && points <= MAXCOLS)
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
}

/* Main function, called first */
int main(void)
{
    /* Clear the homescreen */
    os_ClrHome();

    uint8_t mode = 2;
    uint8_t key = 0;


    os_PutStrLine("100,000 digits of \xc4!");
    os_SetCursorPos(1, 0);
    os_PutStrLine("Type the first digit of \xc4");


    while (key != sk_Enter && key != sk_Clear && key != sk_Right){
        os_SetCursorPos(3, 0);
        os_PutStrLine("Select mode:");

        if (key == sk_Mode || key == sk_Down)
        {
            mode = (mode + 1) % 3;
        }
        else if (key == sk_Up)
        {
            if (mode == 0)
            {
                mode = 2;
            }
            else
            {
                mode = (mode - 1) % 3;
            }
        }


        os_SetCursorPos(4, 0);
        os_PutStrLine(" Quiz mode");
        os_SetCursorPos(5, 0);
        os_PutStrLine(" Practice mode");
        os_SetCursorPos(6, 0);
        os_PutStrLine(" Pi-Digits explorer");

        os_SetCursorPos(mode+4, 0);
        os_PutStrFull(">");

        // get the next key
        key = getkey();
    }

    if (key == sk_Clear)
    {
        return 0;
    }

    os_ClrLCDFull();
    switch(mode)
    {
        case 0:
            game(1);
            break;
        case 1:
            game(0);
            break;
        case 2:
            os_PutStrFull("Not implemented.");
            break;
    }



    /* Return 0 for success */
    return 0;
}
