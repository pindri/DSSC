#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "cptimer.h"


double f(const double x) {
  return ( 1./(1.+ x*x) );
}

double compute_pi(const double a, const double b, const unsigned int N) {

  const double h = (b-a)/(double)N;
  double x = a + h/2.;
  double pi = 0;

  for (int i = 0; i < N; i++) {
    pi += f(x);
    x += h;
  }

  return 4*h*pi;
}



double compute_atomic_pi(const double a, const double b, const unsigned int N) {

  double global_result = 0;
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    double h = (double)(b-a)/(double)N;

    int local_n = N/nthreads;
    double local_a = a + tid*local_n*h;
    double local_b = local_a + local_n*h;


    double local_result = compute_pi(local_a, local_b, local_n);


#pragma omp atomic
    global_result += local_result;

  } // Ended parallel section.
  return global_result;
}



double compute_critical_pi(const double a, const double b, const unsigned int N) {

  double global_result = 0;
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    double h = (double)(b-a)/(double)N;

    int local_n = N/nthreads;
    double local_a = a + tid*local_n*h;
    double local_b = local_a + local_n*h;


    double local_result = compute_pi(local_a, local_b, local_n);


#pragma omp critical
    global_result += local_result;

  } // Ended parallel section.
  return global_result;
}



double compute_reduction_pi(const double a, const double b, const unsigned int N) {

  double global_result = 0;
#pragma omp parallel reduction(+:global_result)
  {
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    double h = (double)(b-a)/(double)N;

    int local_n = N/nthreads;
    double local_a = a + tid*local_n*h;
    double local_b = local_a + local_n*h;


    global_result += compute_pi(local_a, local_b, local_n);

  } // Ended parallel section.
  return global_result;
}


int main() {

  const unsigned int N = 100000000;
  const int a = 0;
  const int b = 1;
  double serial_result = 0;
  double atomic_result = 0;
  double critical_result = 0;
  double reduction_result = 0;

  double start;
  double end;

  // Serial computation.
  start = seconds();
  serial_result = compute_pi(a, b, N);
  end = seconds();

#ifndef BENCHMARK
  printf("Serial pi = %.10f. Elapsed time = %fs.\n", serial_result, end-start);
#else
  printf("%f",  end-start);
#endif


  // OMP ATOMIC
  start = seconds();
  atomic_result = compute_atomic_pi(a, b, N);
  end = seconds();

#ifndef BENCHMARK
  printf("Atomic pi = %.10f. Elapsed time = %fs.\n", atomic_result, end - start);
#else
  printf("\t%f",  end-start);
#endif

  // OMP CRITICAL
  start = seconds();
  critical_result = compute_critical_pi(a, b, N);
  end = seconds();

#ifndef BENCHMARK
  printf("Critical pi = %.10f. Elapsed time = %fs.\n", critical_result, end - start);
#else
  printf("\t%f",  end-start);
#endif

  // OMP REDUCTION
  start = seconds();
  reduction_result = compute_reduction_pi(a, b, N);
  end = seconds();

#ifndef BENCHMARK
  printf("Reduction pi = %.10f. Elapsed time = %fs.\n", reduction_result, end - start);
#else
  printf("\t%f\n",  end-start);
#endif


  return 0;

}
