#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>


void print_matrix(int* M, int rows, int cols) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      printf("%d  ", M[i * cols + j]);
    }
    printf("\n");
  }
}

void print_matrix_file(int* M, int rows, int cols, FILE* file) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      fprintf(file, "%d  ", M[i * cols + j]);
    }
    fprintf(file, "\n");
  }
}



int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;
  const int N = atoi(argv[1]); // Size of the matrix, taken as argument.
  MPI_Request request;

  MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    int local_N = N / npes;
    int rest = N % npes;

    // If the matrix size N is not a multiple of npes, handle rest.
    if (rank < rest) {
      local_N += 1;
    }

    // Allocate local matrix.
    int* M = (int* )malloc(local_N * N * sizeof(int));


    // Correction to take rest into account.
    int corr = 0;
    if (rank >= rest && rest != 0) corr = 1;


    // Fill partial identities.
    for (size_t i = 0; i < local_N; i++) {
      for (size_t j = 0; j < N; j++) {
        if (j == i + rank * local_N + corr * rest) {
          M[i * N + j] = 1;
        } else {
          M[i * N + j] = 0;
        }
      }
    }


    if (rank != 0) {

      // Send partial matricess to process 0.
      MPI_Send(M, local_N * N, MPI_INT, 0, 101, MPI_COMM_WORLD);

    } else { // In processor 0.

      if (N < 10) { // Print to console.

        print_matrix(M, local_N, N);
        for (int pes = 1; pes < npes; pes++) {
          //printf("pes: %d\n", pes);
          if (pes >= rest && rest != 0) corr = 1; // Correct printing in case of rest.
          MPI_Recv(M, local_N * N, MPI_INT, pes, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          print_matrix(M, local_N - corr, N);
        }

      } else { // N > 10, print to file.
        FILE* file = fopen("output.dat", "w");

        print_matrix_file(M, local_N, N, file);
        for (int pes = 1; pes < npes; pes++) {
          if (pes >= rest && rest != 0) corr = 1; // Correct printing in case of rest.
          MPI_Recv(M, local_N * N, MPI_INT, pes, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          print_matrix_file(M, local_N - corr, N, file);
        }
        fclose(file);

      }

    }

    free(M);


  MPI_Finalize();

  return 0;

}
