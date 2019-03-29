#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

void print_usage( int * a, int N, int nthreads ) {

  int tid, i;
  for( tid = 0; tid < nthreads; ++tid ) {

    fprintf( stdout, "%d: ", tid );

    for( i = 0; i < N; ++i ) {

      if( a[ i ] == tid) fprintf( stdout, "*" );
      else fprintf( stdout, " ");
    }
    printf("\n");
  }
}

int main( int argc, char * argv[] ) {

  const int N = 100;
  int a[N];
  int thread_id = 0;
  int nthreads = 1;

  for (int i = 0; i < N; ++i) {
    a[i] = thread_id;
  }

  printf("serial:\n");
  print_usage(a, N, nthreads);


#pragma omp parallel
  {
    nthreads = omp_get_num_threads();

#pragma omp for schedule(static, 5)
    for (int i = 0; i < N; ++i) {
      a[i] = omp_get_thread_num();
    }

#pragma omp single
  {
    printf("\nstatic, chunk size = 1\n");
    print_usage(a, N, nthreads);
  }

#pragma omp for schedule(dynamic, 5)
    for (int i = 0; i < N; ++i) {
      a[i] = omp_get_thread_num();
    }

#pragma omp single
  {
    printf("\ndynamic, chunk size = 1\n");
    print_usage(a, N, nthreads);
  }

  }
  return 0;

}
