#include <stdio.h>
#include <stdlib.h>

extern long add_with_bias(long left, long right);

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s left right\n", argv[0]);
        return 2;
    }
    long left = strtol(argv[1], NULL, 10);
    long right = strtol(argv[2], NULL, 10);
    printf("%ld\n", add_with_bias(left, right));
    return 0;
}
