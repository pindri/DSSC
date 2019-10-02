#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

double seconds() {

  struct timeval tmp;
  double sec;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
  return sec;
}

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

int main() {

  const unsigned int N = 1000000000;
  const int a = 0;
  const int b = 1;
  double serial_result = 0;
  double global_result = 0;

  double start;
  double end;

  //start = seconds();
  //serial_result = compute_pi(a,b,N);
  //end = seconds();
//
  //printf("%f",  end-start);

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


#pragma omp atomic
    global_result += local_result;

  } // Ended parallel section.

  end = seconds();

  printf("\t%f\n",  end-start);

  return 0;

}
