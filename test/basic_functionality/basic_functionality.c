#include <stdio.h>

#include "ucavx.h"

void print_vecd(const double* in, size_t len) {
    printf("{");
    for (size_t i = 0; i < len; i++) {
        if (i > 0) { printf(", "); }
        printf("%.1f", in[i]);
    }
    printf("}\n");
}

int main() {
    for (size_t i = 1; i < 100; i++) {
        double* dst = malloc(i * sizeof(double));
        double* a = malloc(i * sizeof(double));
        double* b = malloc(i * sizeof(double));
        for (size_t j = 0; j < i; j++) {
            a[j] = (double)j;
            b[j] = (double)j * 2;
        }
        printf("---%zu", i);
        printf("\na:  ");
        print_vecd(a, i);
        printf("\nb:  ");
        print_vecd(b, i);
        ucavx256_add_d_arblen(dst, a, b, i);
        printf("\ndst:");
        print_vecd(dst, i);
    }
    return 0;
}
