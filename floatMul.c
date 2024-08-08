#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define FLOAT_SZ sizeof(float)*CHAR_BIT
#define EXP_SZ (FLOAT_SZ-FLT_MANT_DIG)
#define FRAC_SZ (FLT_MANT_DIG-1)
#define BIAS 127 // Directly use the calculated bias for single precision floating points

unsigned int readBinary(FILE *fp) 
{
    char buffer[FLOAT_SZ + 1]; // Plus null-terminator
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        perror("Failed to read binary number");
        exit(EXIT_FAILURE);
    }
    return strtoul(buffer, NULL, 2);
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    unsigned int multiplier = readBinary(fp);
    unsigned int multiplicand = readBinary(fp);

    unsigned int sign1 = (multiplier >> (EXP_SZ + FRAC_SZ)) & 1;
    unsigned int sign2 = (multiplicand >> (EXP_SZ + FRAC_SZ)) & 1;
    unsigned int sign = sign1 ^ sign2;

    int exp1 = ((multiplier >> FRAC_SZ) & ((1 << EXP_SZ) - 1)) - BIAS;
    int exp2 = ((multiplicand >> FRAC_SZ) & ((1 << EXP_SZ) - 1)) - BIAS;
    int exp = exp1 + exp2 + BIAS;

    unsigned long long frac1 = ((unsigned long long)(multiplier & ((1ULL << FRAC_SZ) - 1)) | (1ULL << FRAC_SZ));
    unsigned long long frac2 = ((unsigned long long)(multiplicand & ((1ULL << FRAC_SZ) - 1)) | (1ULL << FRAC_SZ));
    unsigned long long frac = frac1 * frac2 >> FRAC_SZ; // Normalize early to avoid overflow

    // Normalize if necessary and adjust the exponent
    if (frac >= (1ULL << (FRAC_SZ + 1))) {
        frac >>= 1;
        exp += 1;
    }

    // Handle special cases: zero, overflow, underflow
    if (exp >= (1 << EXP_SZ) - 1) 
    { // Overflow or inf
        exp = (1 << EXP_SZ) - 1;
        frac = 0; // Consider as infinity
    } else if (exp <= 0) 
    { // Underflow or zero
        if (exp <= -FRAC_SZ)
         { // Too small to represent
            exp = 0;
            frac = 0;
        } else 
        { // Denormalized number
            frac >>= (-exp + 1);
            exp = 0;
        }
    }

    // Combine sign, exp, and frac into result
    unsigned int result = (sign << (EXP_SZ + FRAC_SZ)) | ((exp & ((1 << EXP_SZ) - 1)) << FRAC_SZ) | (frac & ((1ULL << FRAC_SZ) - 1));

    // PRINTING
    for (int bit_index = FLOAT_SZ - 1; bit_index >= 0; bit_index--) 
    {
        printf("%d", (result >> bit_index) & 1);
        if (bit_index == EXP_SZ + FRAC_SZ || bit_index == FRAC_SZ) printf("_");
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
