// This program is to clarify the difference between (1ULL << 2) and 0x00000004

#include <stdio.h>

#define STYLE1 (1ULL << 2)
#define STYLE2 0x00000004

int main()
{
    printf("size of (1ULL << 2): %lu\n", sizeof(STYLE1));
    printf("size of 0x00000004: %lu", sizeof(STYLE2));
    return 0;
}
