#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
// https://www.tutorialspoint.com/c_standard_library/limits_h.htm
#include <limits.h>

int main(int argc, char *argv[]) 
{

    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the minuend (number to be subtracted from)
    char buff;
    bool minuend[CHAR_BIT]; // suggested that you store bits as array of bools; minuend[0] is the LSB
    for (int i=CHAR_BIT-1; 0<=i; i--) { // read MSB first as that is what comes first in the file
        /* ... */
        if (fscanf(fp, "%c", &buff) == 1) 
        {
            minuend[i] = (buff == '1');
        } else 
        {
            perror("fscanf failed");
            return EXIT_FAILURE;
        }

    }

    // notice that you are reading two different lines; caution with reading
    /* ... */
    if (fscanf(fp, "%c", &buff) != EOF) 
    { 
        if (buff != '\n') 
        {
            perror("unexpected character");
            return EXIT_FAILURE;
        }
    }

    // second, read the subtrahend (number to subtract)
    bool subtrahend[CHAR_BIT]; // suggested that you store bits as array of bools; subtrahend[0] is the LSB
    for (int i=CHAR_BIT-1; 0<=i; i--) 
    { 
        /* ... */
        if (fscanf(fp, "%c", &buff) == 1) 
        {
            subtrahend[i] = (buff == '1');
        } else 
        {
            perror("fscanf failed");
            return EXIT_FAILURE;
        }
    }

    // WE WILL DO SUBTRACTION BY NEGATING THE SUBTRAHEND AND ADD THAT TO THE MINUEND

    // Negate the subtrahend
    // flip all bits
    /* ... */
        for (int i=0; i<CHAR_BIT; i++) 
        {
        subtrahend[i] = !subtrahend[i];
    }


    // add one
    bool carry = true; // to implement the 'add one' logic, we do binary addition logic with carry set to true at the beginning
    for (int i=0; i<CHAR_BIT; i++) 
    { 
        /* ... */
        bool a = subtrahend[i];
        bool b = carry;
        carry = (a & b);
        subtrahend[i] = (a ^ b);
    }

    // Add the minuend and the negated subtrahend
    bool difference[CHAR_BIT];
    /* ... */
    carry = false;
    for (int i=0; i<CHAR_BIT; i++) 
    { // iterate from LSB to MSB
        bool x = minuend[i];
        bool y = subtrahend[i];
        bool s = (x ^ y ^ carry);
        carry = ((x & y) | (y & carry) | (x & carry));
        difference[i] = s;
    }

    // print the difference bit string
    for (int i=CHAR_BIT-1; 0<=i; i--)
    {
        printf("%d",difference[i]);
    }
        printf("\n");

    return EXIT_SUCCESS;

}

