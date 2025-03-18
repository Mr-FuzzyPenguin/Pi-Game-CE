#include <stdio.h>
#include <stdint.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include "pi.h"
// #include <stdlib.h>

#define MAXROWS 10
#define MAXCOLS 26

/* Main function, called first */
int main(void)
{

    /* Clear the homescreen */
    os_ClrHome();

    os_PutStrLine("100,000 digits of \xc4!");
    os_SetCursorPos(1, 0);
    os_PutStrLine("Type the first digit of \xc4");
    /* Waits for a key */
    uint8_t key = 0;
    uint8_t offset;
    volatile uint32_t points = 0;
    char output[MAXCOLS+1];
    while (key != sk_Clear)
    {
        key = os_GetCSC();
        switch(key){
            case sk_DecPnt:
            case sk_0:
                offset = 0;
                break;
            case sk_1:
                offset = 1;
                break;
            case sk_2:
                offset = 2;
                break;
            case sk_3:
                offset = 3;
                break;
            case sk_4:
                offset = 4;
                break;
            case sk_5:
                offset = 5;
                break;
            case sk_6:
                offset = 6;
                break;
            case sk_7:
                offset = 7;
                break;
            case sk_8:
                offset = 8;
                break;
            case sk_9:
                offset = 9;
                break;
            default:
                offset = 10;
                break;
        }

        // a valid key press. Judge if it was a correct press.
        if (offset >= 0 && offset < 10)
        {
            sprintf(output, "Pressed: %d", offset);
            os_SetCursorPos(2, 0);
            os_PutStrLine(output);

            uint8_t check;
            os_SetCursorPos(3, 0);
            if (points % 2)
            {
                check = (pi_index[points/2] & 0x0f);
            }
            else
            {
                check = ((pi_index[points/2] & 0xf0) >> 4);
            }
            sprintf(output, "Next: %d", check);

            if (check == offset)
            {
                os_SetCursorPos(3, 0);
                os_PutStrLine("         ");


                os_SetCursorPos(4, 0);
                points++;
                sprintf(output, "Points: %lu", points);
            }
            os_PutStrLine(output);
        }
    }

    /* Return 0 for success */
    return 0;
}
