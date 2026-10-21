#include <stdio.h>
#include <stdint.h>

int main() {
    int    i = 10;
    char   c = 'A';
    double d = 3.14;
    short  s = 5;

    printf("Sizes:\n");
    printf("int : %zu bytes\n", sizeof(int));
    printf("char : %zu bytes\n", sizeof(char));
    printf("double : %zu bytes\n", sizeof(double));
    printf("short : %zu bytes\n", sizeof(short));

    printf("\nAddresses:\n");
    printf("int i : %p  (address %% 4 = %lu)\n", (void*)&i, (uintptr_t)&i % 4);
    printf("char c : %p  (address %% 1 = %lu)\n", (void*)&c, (uintptr_t)&c % 1);
    printf("double d : %p  (address %% 8 = %lu)\n", (void*)&d, (uintptr_t)&d % 8);
    printf("short s : %p  (address %% 2 = %lu)\n", (void*)&s, (uintptr_t)&s % 2);

    return 0;
}
