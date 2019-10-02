/*
 *
 * This code implements the sum of two vectors in parallel using a shared memory approach, implemented with OpenMP
 * The parallelization is explicit because the data are manually divided among the threads. 
 * The pre-defined size of the vectors N is expected to be a submultiple of the number of threads spwaned at runtime.
 *
 * Every element of the two vectors a and b is initialized with the threadId of the thread working on the element.
 * resulting vector c is obtained by the parallel sum of the two vectors a 
 *
 * Compile with: 
 * $icc -qopenmp vector_parallel.c
 * 
 * Run with:
 * $export OMP_NUM_THREADS=[NUMBER OF THREADS]; ./a.out
 *
 */

#include <stdlib.h>
#include <stdio.h>

// Header file for compiling code including OpenMP APIs
#include <omp.h>

#define N 8

int main( int argc, char * argv[] ){

  int thread_id = 0.0;
  int * a, * b, * c;
  int i = 0;
  
  a = (int * )malloc( N * sizeof(int) );
  b = (int * )malloc( N * sizeof(int) );
  c = (int * )malloc( N * sizeof(int) );

#pragma omp parallel private( thread_id, i )
  {
    int n_threads = omp_get_num_threads(); 
    thread_id = omp_get_thread_num(); 

    if( ( N % n_threads ) && !thread_id){
      fprintf( stderr, "\nSize N = %d is expected to be a multiple of the number of threads = %d. Program exits...\n", N, n_threads ); 
      exit(1);
    }
    
    int num_ist_per_thread = N / n_threads;
    int start = num_ist_per_thread * thread_id;
    int end = start + num_ist_per_thread;

    for( i = start; i < end; i++ ){
      a[ i ] = b[ i ] = thread_id;
      c[ i ] = a[ i ] + b[ i ];
    }
  }

  for( i = 0; i < N; i++ ){
    fprintf( stdout, "\nc[ %d ] = a[ %d ] + b[ %d ] = %d \n", i, i, i, c[ i ] );
  }

  free( a );
  free( b );
  free( c );

  return 0;
}
