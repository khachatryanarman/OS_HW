#include <stdio.h>
#include <stddef.h>

struct NormalStruct {
    char   a;
    int    b;
    double c;
};

#pragma pack(push, 1)
struct PackedStruct {
    char   a;
    int    b;
    double c;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Packed2 {
    char   a;
    int    b;
    double c;
};
#pragma pack(pop)


#define SHOW(type, var, title)                                                   \
    printf("%s\n", title);                                                       \
    printf("sizeof = %zu bytes\n", sizeof(type));                                \
    printf("&a = %p  (offset %zu)\n", (void*)&var.a, offsetof(type, a));         \
    printf("&b = %p  (offset %zu)\n", (void*)&var.b, offsetof(type, b));         \
    printf("&c = %p  (offset %zu)\n\n", (void*)&var.c, offsetof(type, c));

int main() {
    struct NormalStruct n;
    struct PackedStruct p1;
    struct Packed2 p2;

    SHOW(struct NormalStruct, n,  "Without #pragma pack");
    SHOW(struct PackedStruct, p1, "#pragma pack(1)");
    SHOW(struct Packed2,      p2, "#pragma pack(2)");

    return 0;
}
