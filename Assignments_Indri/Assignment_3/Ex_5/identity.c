#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>



int** allocate_matrix(const size_t rows, const size_t cols) {
  int** M = (int**)malloc(rows * sizeof(int*));
  for (size_t i = 0; i < rows; i++) {
    M[i] = (int*)malloc(cols * sizeof(int));
  }
  return M;
}

void print_matrix(int** M, const size_t rows, const size_t cols) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      printf("%d  ", M[i][j]);
    }
    printf("\n");
  }
}


void deallocate_matrix(int** M, const size_t rows) {
  for (size_t i = 0; i < rows; i++) {
    free(M[i]);
  }
  free(M);
}




int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;
  const int N = atoi(argv[1]); // Size of the matrix, taken as argument.

  MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    int local_N = N/npes;
    int rest = N%npes;

    // If the matrix size N is not a multiple of npes, handle rest.
    if(rank < rest) {
      local_N += 1;
    }

    // Allocate local matrix.
    int** M = allocate_matrix(local_N, N);

    // Correction for the rank>=rest case.
    int corr = 0;
    if (rank >= rest && rest != 0) corr = 1;

    // Fill partial identities.
    for (size_t i = 0; i < local_N; i++) {
      for (size_t j = 0; j < N; j++) {
        if (j == i + rank * local_N + corr * rest) M[i][j] = 1;
        else M[i][j] = 0;
      }
    }

    // Open output binary file.
    MPI_File file;
    MPI_Status status;
    MPI_Offset offset;
    MPI_File_open(MPI_COMM_WORLD, "output.dat", MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &file);


    if (rank != 0) {

      // Send partial matricess to process 0.
      for (int row = 0; row < local_N; row++) {
        MPI_Send(M[row], N, MPI_INT, 0, 101, MPI_COMM_WORLD);
      }

    } else {

      if (N < 10) {

        print_matrix(M, local_N, N);
        for (int pes = 1; pes < npes; pes++) {
          // Corrects number of rows in case of rest.
          if (pes >= rest && rest != 0) corr = 1;
          for (int row = 0; row < local_N - corr; row++) {
            MPI_Recv(M[row], N, MPI_INT, pes, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          }
          print_matrix(M, local_N - corr, N);
        }

      } else { //if N >= 10.

        // Writing content of pes 0.
        for (int row = 0; row < local_N; row++) {
          offset = row * N * sizeof(int);
          MPI_File_seek(file, offset, MPI_SEEK_SET);
          MPI_File_write(file, M[row], N, MPI_INT, &status);
        }

        // Writing content of other pes.
        for (int pes = 1; pes < npes; pes++) {

          // Corrects number of rows in case of rest.
          if (pes >= rest && rest != 0) corr = 1;

          // Receive from each processor and write with the correct offset.
          for (int row = 0; row < local_N - corr; row++) {
            MPI_Recv(M[row], N, MPI_INT, pes, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            offset = ( corr * (rest * local_N + (pes - rest) * (local_N - 1)) +
                (1 - corr) * (local_N * pes) + row ) * N * sizeof(int);
                // Can probably be written more clearly.
            MPI_File_seek(file, offset, MPI_SEEK_SET);
            MPI_File_write(file, M[row], N, MPI_INT, &status);
          }
        }
      }
    }

    deallocate_matrix(M, local_N);

    // Close binary file.
    MPI_File_close(&file);


  MPI_Finalize();

  return 0;

}
