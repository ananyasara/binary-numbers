#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

size_t anyToInteger(char* source, int base, int digitCount) 
{
    size_t repr = 0;
    for (int i = 0; i < digitCount; ++i) 
    {
        char c = source[i];
        int numValue;
        if (c >= '0' && c <= '9') 
        {
            numValue = c - '0';
        } else if (c >= 'A' && c <= 'Z') 
        {
            numValue = 10 + (c - 'A');
        }
        repr = repr * base + numValue;
    }
    return repr;
}

void integerToAny(char* result, size_t repr, int base) 
{
    char temp[65];
    int i = 0;
    if (repr == 0) 
    {
        result[0] = '0';
        result[1] = '\0';
        return;
    }
    while (repr > 0) 
    {
        int remainder = repr % base;
        if (remainder < 10) 
        {
            temp[i++] = '0' + remainder;
        } else 
        {
            temp[i++] = 'A' + (remainder - 10);
        }
        repr /= base;
    }
    temp[i] = '\0';

    // Reverse the string
    int k, j;
    for (j = 0, k = i - 1; j < i; j++, k--) 
    {
        result[j] = temp[k];
    }
    result[i] = '\0';
}

int main(int argc, char* argv[]) 
{
    FILE* fp = fopen(argv[1], "r");
    if (!fp) 
    {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    int digitCount;
    int sourceBase;
    int destBase;

    if (!fscanf(fp, "%d\n", &digitCount)) 
    {
      perror("reading the input digit count failed");
      exit(EXIT_FAILURE);
    }

    if (!fscanf(fp, "%d\n", &sourceBase)) 
    {
      perror("reading the source base failed");
      exit(EXIT_FAILURE);
    }

    if (!fscanf(fp, "%d\n", &destBase)) 
    {
      perror("reading the destination base failed");
      exit(EXIT_FAILURE);
    }

    char* sourceNum = calloc(sizeof(char), digitCount+1);

    if (!fscanf(fp, "%s\n", sourceNum)) 
    {
      perror("reading the source number");
      exit(EXIT_FAILURE);
    }

    size_t repr;

    repr = anyToInteger(sourceNum, sourceBase, digitCount);

    char* result = (char*)calloc(sizeof(char), 65); 
    integerToAny(result, repr, destBase);

   printf("%s", result);

    free(sourceNum);
    free(result);

    return EXIT_SUCCESS;
}
