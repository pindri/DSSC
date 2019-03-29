#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "cptimer.h"


double f(const double x) {
  return ( 1./(1.+ x*x) );
}

double compute_pi(const double a, const double b, const int N) {

  double h = (b-a)/(double)N;
  double x = a + h/2.;
  double pi = 0;

  for (int i = 0; i < N; i++) {
    pi += f(x);
    x += h;
  }

  return 4*h*pi;


}

int main(int argc, char* argv[]) {

  int N = 100000000;
  int a = 0;
  int b = 1;
  double global_result = 0.;

  double start;
  double end;

  start = seconds();
  double result = compute_pi(a,b,N);
  end = seconds();

 // printf("Serial pi = %.10f. Elapsed time = %fs.\n", result, end-start);
  printf("%f",  end-start);


  start = seconds();

#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    double h = (double)(b-a)/(double)N;

    int local_n = N/nthreads;
    double local_a = a + tid*local_n*h;
    double local_b = local_a + local_n*h;


    double local_result = compute_pi(local_a, local_b, local_n);

    //printf("Local a = %f\tLocal b = %f\tLocal n = %d\n", local_a, local_b, local_n);

#pragma omp atomic
    global_result += local_result;

  } // Ended parallel section.

  end = seconds();


  //printf("Parallel pi = %.10f. Elapsed time = %fs.\n", global_result, end - start);
  printf("\t%f\n",  end-start);


  return 0;

}
