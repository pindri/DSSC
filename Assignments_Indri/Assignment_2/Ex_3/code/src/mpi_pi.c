#include <stdio.h>
#include <mpi.h>


int main(int argc, char* argv[]) {

  const unsigned int N = 1000000000;
  const int a = 0;
  const int b = 1;
  const double h = 1./N;

  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes

  MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    
    const int printer = npes - 1;
    const int reducer = 0;

    const unsigned int local_N = N/npes;
    double local_pi, start_time, end_time;
    const double start = rank * local_N;  // start point for the computation
    const double end = start + local_N;  // end point for the computation


    start_time = MPI_Wtime();

      for (int i = start; i < end; i++) {
        local_pi += 4.0 / (1.0 + (h * (i + 0.5)) * (h * (i + 0.5)));
      }
      local_pi *= h;
  
      // printf("partial result: start = %f,\tend = %f,\tpi = %f \n", start, end, local_pi);
  
      double global_pi;
      MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, reducer, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    
    if (rank == reducer) {
      MPI_Send(&global_pi, 1, MPI_DOUBLE, printer, 101, MPI_COMM_WORLD);
    }

    if (rank == printer) {
      MPI_Recv(&global_pi, 1, MPI_DOUBLE, reducer, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("pi =  %f\t elapsed time = %f\n", global_pi, end_time - start_time);
    }


  MPI_Finalize();



  return 0;

}
