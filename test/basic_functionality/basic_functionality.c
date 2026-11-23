#include <stdio.h>

#define UCAVX_COMPILE
#include "ucavx.h"

void vecd_print(const double* in, size_t len) {
    printf("{");
    for (size_t i = 0; i < len; i++) {
        if (i > 0) { printf(", "); }
        printf("%.1f", in[i]);
    }
    printf("}\n");
}

int vecd_eq(const double* v1, const double* v2, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        if (v1[i] != v2[i])
        {
            return 0;
        }
    }
    return 1;
}

int main() {
    for (size_t i = 1; i < 100; i++) {
        double* dst1 = malloc(i * sizeof(double));
        double* a1 = malloc(i * sizeof(double));
        double* b1 = malloc(i * sizeof(double));
        double* dst2 = malloc(i * sizeof(double));
        double* a2 = malloc(i * sizeof(double));
        double* b2 = malloc(i * sizeof(double));
        for (size_t j = 0; j < i; j++) {
            a1[j] = (double)j;
            b1[j] = (double)j * 2;
            a2[j] = (double)j;
            b2[j] = (double)j * 2;
        }
        printf("---%zu", i);
        printf("\na1:  ");
        vecd_print(a1, i);
        printf("\nb1:  ");
        vecd_print(b1, i);
        ucavx256_add_d_arblen(dst1, a1, b1, i);
        printf("\ndst256_add_d:");
        vecd_print(dst1, i);
        printf("\na2:  ");
        vecd_print(a2, i);
        printf("\nb2:  ");
        vecd_print(b2, i);
        ucavx128_add_d_arblen(dst2, a2, b2, i);
        printf("\ndst128_add_d:");
        vecd_print(dst2, i);
        if (!vecd_eq(dst1, dst2, i))
        {
            printf("UNEXPECTEDLY NOT EQUAL\n");
            return 1;
        }
    }
    return 0;
}
