#include <stdio.h>
#include <stddef.h>

struct Original { 
    char   a;
    int    b;
    double c;
};

struct Reordered {
    double c;
    int    b;
    char   a;
};

struct Worst { 
    char   a;
    double c;
    int    b;
};

int main() {
    struct Original  s1;
    struct Reordered s2;
    struct Worst     s3;


    // char 4 byte 3 pad, int 4 byte, double 8 byte
    printf("sizeof = %zu bytes\n", sizeof(s1));
    printf("&s1   = %p\n", (void*)&s1);
    printf("&s1.a = %p\n", (void*)&s1.a);
    printf("&s1.b = %p\n", (void*)&s1.b);
    printf("&s1.c = %p\n", (void*)&s1.c);

    // double 8 byte int 4 byte char 4 byte 3 pad
    printf("sizeof = %zu bytes\n", sizeof(s2));
    printf("&s2   = %p\n", (void*)&s2);
    printf("&s2.c = %p\n", (void*)&s2.c);
    printf("&s2.b = %p\n", (void*)&s2.b);
    printf("&s2.a = %p\n", (void*)&s2.a);

    // char 8 byte 7 pad, double 8 byte int 8 byte 4 pad
    printf("sizeof = %zu bytes\n", sizeof(s3));
    printf("&s3   = %p\n", (void*)&s3);
    printf("&s3.a = %p\n", (void*)&s3.a);
    printf("&s3.c = %p\n", (void*)&s3.c);
    printf("&s3.b = %p\n", (void*)&s3.b);

    return 0;
}
