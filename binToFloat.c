#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) 
{

    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the binary number representation of float point number
    char buff;
    unsigned int binary = 0;
    for (int i=EXP_SZ+FRAC_SZ; 0<=i; i--) 
    { // read MSB first as that is what comes first in the file
        /* ... */
       if (fscanf(fp, "%c", &buff) != 1) 
       {
            perror("fscanf failed");
            return EXIT_FAILURE;
        }
        binary = (binary << 1) | (buff - '0');
    }
    // float number = *(float *)&binary; // you are not allowed to do this.
    bool sign = binary >> (EXP_SZ + FRAC_SZ);
    int exponent = (binary >> FRAC_SZ) & ((1 << EXP_SZ) - 1);
    unsigned int mantissa = binary & ((1 << FRAC_SZ) - 1);

    /* ... */
    double value = 0.0;
    if (exponent == 255) 
    {
        // special values: NaN or infinity
        if (mantissa == 0) 
        {
            // infinity
            value = sign ? -INFINITY : INFINITY;
        } else 
        {
            // NaN
            value = NAN;
        }
    } else if (exponent == 0) 
    {
        // subnormal number
        value = ldexp(mantissa, -126);
    } else 
    {
        // normal number
        mantissa |= (1 << FRAC_SZ); // add implicit leading 1
        value = ldexp(mantissa, exponent - 127 - FRAC_SZ);
    }
    value = sign ? -value : value;

    // https://www.tutorialspoint.com/c_standard_library/c_function_ldexp.htm
    //double number = ldexp ( m, e );
   // number = sign ? -number : number;
  //  printf("%e\n", number);

printf("%e\n", value);

    fclose(fp);
    return EXIT_SUCCESS;

}
