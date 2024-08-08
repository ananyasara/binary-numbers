#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

int main(int argc, char *argv[]) {
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    double value;
    fscanf(fp, "%lf", &value);
    fclose(fp);

    bool sign = value < 0.0;
    printf("%d_", sign);

    double fraction = fabs(value);
    signed short trial_exp = 0;
    while (fraction >= 2.0) 
    {
        fraction /= 2.0;
        trial_exp++;
    }
    while (fraction < 1.0 && trial_exp > -1023) 
    {
        fraction *= 2.0;
        trial_exp--;
    }

    unsigned short bias = (1 << (EXP_SZ - 1)) - 1;
    signed short exp = (trial_exp >= -1022) ? trial_exp + bias : 0;

    // Print exponent bits
    for (int exp_index = EXP_SZ - 1; exp_index >= 0; exp_index--) 
    {
        bool exp_bit = (exp >> exp_index) & 1;
        printf("%d", exp_bit);
    }
    printf("_");

    // Adjust the fraction for normalized numbers
    if (trial_exp >= -1022) 
    {
        fraction -= 1.0; // Remove the implicit 1 for the mantissa
    } else 
    {
        // For denormalized numbers, adjust the fraction as the number itself
        fraction *= pow(2, FRAC_SZ + trial_exp + 1); // Adjust for denormalized scaling
    }

    // Calculate fraction bits
    unsigned long long frac_bits = (unsigned long long)(fraction * (1ULL << FRAC_SZ));

    // Print fraction bits
    for (int frac_index = FRAC_SZ - 1; frac_index >= 0; frac_index--) 
    {
        bool frac_bit = (frac_bits >> frac_index) & 1;
        printf("%d", frac_bit);
        if ((frac_index % 4 == 0) && frac_index != 0) printf("_"); // Optional: readability
    }

    return EXIT_SUCCESS;
}
