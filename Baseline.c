#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double compute_pi(size_t dt){
    double pi = 0.0;
    double delta = 1.0/dt;
    size_t i = 0;
    for(i=0; i<dt; i++){
        double x = (double)i/dt;
        pi += delta/(1.0 + x*x);
    }
    return 4.0 * pi;
}

int main(int argc, char **argv){
    size_t dt = 128 * 1024 * 1024;
    double pi_result = 0.0;
    clock_t start,end;
    double period;

    start = clock();
    pi_result = compute_pi(dt);
    end = clock();
    period = (double)(end - start)/CLOCKS_PER_SEC;
    printf("Pi=%.10f, Time=%f\n",pi_result,period);

    return 0;
}
