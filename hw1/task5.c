#include <stdio.h>

int main () {
    
    char str[] = "hello";
    char* p = &str[0];
    char* end = NULL;
    int i = 0;
    while (*(p + i) != '\0') {
        printf ("%c", *(p + i));
        ++i;
    }
    end = p + i;

    printf ("\n");

    // I assume this solution was asked "using pointer arithmetic", otherwise I could just print i
    printf ("string contains %d elements \n", (int)((end - p) / sizeof(char)));

    return 0;
}
