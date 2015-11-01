#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "openmp.h"
#include <math.h>

typedef double_t fvec_t __attribute__((vector_size((8)*sizeof(double_t))));

double compute_pi(size_t dt)
{
    double pi = 0.0;
    double delta = 1.0/dt;
    size_t i = 0;
    for(i=0; i<dt; i++) {
        double x = (double)i/dt;
        pi += delta/(1.0 + x*x);
    }
    return 4.0 * pi;
}

double compute_pi_SIMD(double_t dt)
{
    size_t s=0, i=0;
    double_t out = 0.0;
    fvec_t S1 = {(double_t)1.0,(double_t)1.0,(double_t)1.0,(double_t)1.0};
    fvec_t piS = {(double_t)0.0,(double_t)0.0,(double_t)0.0,(double_t)0.0};
    fvec_t dtS = {(double_t)dt,(double_t)dt,(double_t)dt,(double_t)dt};
    fvec_t S4 = {(double_t)4.0,(double_t)4.0,(double_t)4.0,(double_t)4.0};
    fvec_t iS = {(double_t)1.0,(double_t)2.0,(double_t)3.0,(double_t)4.0};

    fvec_t deltaS = S1/dtS;

    for(i=0; i<dt; i+=4) {
        fvec_t x = iS/dtS;
        piS += deltaS/(S1 + x*x);
        iS += S4;
    }
    for(s=0; s<4; s++)
        out += ((double_t *)(&piS))[s];

    return  4.0 * out;
}

int main(int argc, char **argv)
{
    size_t dt = 128 * 1024 * 1024;
    double pi_result = 0.0, pi_resultS;
    float_t start, startS;
    double period,periodS;

    start = get_wtime();
    pi_result = compute_pi(dt);
    period = get_wtime() - start;

    startS = get_wtime();
    pi_resultS = compute_pi_SIMD(dt);
    periodS = get_wtime() - startS;

    printf("BASE Pi=%.10f, Time=%f\n",pi_result,period);
    printf("SIMD Pi=%.10f, Time=%f\n",pi_resultS,periodS);

    return 0;
}
