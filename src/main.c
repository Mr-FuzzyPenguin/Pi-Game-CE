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
    int8_t offset;
    while (!key)
    {
        key = os_GetCSC();
    }

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
                offset = -1*'0'+key;
                break;
        }

        // sprintf is bad so I won't use it for now
        os_SetCursorPos(5, 0);
        for (uint8_t i =0; i < MAXCOLS; i++){
            if (i % 2)
            {
                output[i] = '0'+((pi_index[i] & 0b11110000)>> 4);
            }
            else
            {
                output[i] = '0'+(pi_index[i] & 0x0f);
            }
        }

        // adding a 0 at the end
        output[MAXCOLS] = 0;
        os_PutStrLine(output);

        // still testing stuff to verify bit shifts are correct.
        os_SetCursorPos(2, 0);
        output[0] = '0'+offset;
        output[1] = ' ';
        output[2] = '0'+((pi_index[0] & 0b11110000)>> 4);
        output[3] = '0'+(pi_index[1] & 0x0f);
        output[4] = 0;
        os_PutStrLine(output);
    }

    /* Return 0 for success */
    return 0;
}
