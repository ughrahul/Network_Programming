#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> // For htons

union
{
    short s;
    char c[sizeof(short)];
} un;

int main(int argc, char **argv)
{
    un.s = 0x0102; // Set the short value to 0x0102 (258 in decimal)

    // Print the original short value in hexadecimal
    printf("Original short value (in hexadecimal): 0x%x\n", un.s);

    // Check endianness by inspecting the first byte of the union
    if (sizeof(short) == 2)
    {
        if (un.c[0] == 1 && un.c[1] == 2)
        {
            printf("Big Endian\n");
        }
        else if (un.c[0] == 2 && un.c[1] == 1)
        {
            printf("Little Endian\n");
        }
        else
        {
            printf("Unknown Endianness\n");
        }
    }
    else
    {
        printf("Size of short is not 2 bytes, unknown behavior.\n");
    }

    // Print the size of short
    printf("sizeof(short) = %lu\n", sizeof(short));

    // Convert the short to network byte order (Big Endian)
    un.s = htons(un.s);
    printf("After htons conversion: 0x%x\n", un.s); // Print after conversion to network byte order

    // Check endianness again after conversion
    if (sizeof(short) == 2)
    {
        if (un.c[0] == 1 && un.c[1] == 2)
        {
            printf("After conversion: Big Endian\n");
        }
        else if (un.c[0] == 2 && un.c[1] == 1)
        {
            printf("After conversion: Little Endian\n");
        }
        else
        {
            printf("Unknown Endianness\n");
        }
    }

    return 0;
}
