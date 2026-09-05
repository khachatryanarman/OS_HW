task1 demonstrates basic address extraction using the address-of operator &. Printing px and &x produces the same output because px stores the address of x directly

task2 demonstrates pointer arithmetic on arrays. Using *(parr + i) allows iterating, reading, and modifying array elements, performing identically to standard arr[i] indexin

task3 demonstrates pass-by-reference. Passing memory addresses (&x, &y) into swap allows the function to dereference those pointers and directly swap the original variable values in main

task4 demonstrates double pointers and multiple levels of indirection. Dereferencing ppx navigates through px to retrieve the underlying value of x

task5 demonstrates string traversal and pointer distance. Iterating until the null terminator '\0' and subtracting two pointers (end - p) calculates the exact number of elements between them
