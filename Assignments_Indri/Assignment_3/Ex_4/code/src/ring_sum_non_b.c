#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define SIZE 10000000 // Size of the vectors.
#define LEFT_PROC(rank, npes) (rank - 1 + npes) % npes
#define RIGHT_PROC(rank, npes) (rank + 1 + npes) % npes

void swap(int** A, int** B) {
  int* Tmp = *A;
  *A = *B;
  *B = Tmp;
}

int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;
  int printer = 0; // Rank of the process that will print the elapsed time.


  MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    MPI_Request requestSend; // Request handler, checks communication status.
    MPI_Request requestRecv; // Request handler, checks communication status.

    int* X = (int*)malloc(sizeof(int) * SIZE);
    int* Xright = (int*)malloc(sizeof(int) * SIZE);
    int* sum = (int*)malloc(sizeof(int) * SIZE);
    double t1, t2;

    // Initialize X and sum.
    for(int i = 0; i < SIZE; i++) {
      X[i] = rank;
      sum[i] = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    // Cycle the whole chain.
    for (int j = 0; j < npes; j++) {

      // Sent X to the left processor, receive from the right processor..
      MPI_Isend(X, SIZE, MPI_INT, LEFT_PROC(rank, npes), 101,
                MPI_COMM_WORLD, &requestSend);

      MPI_Irecv(Xright, SIZE, MPI_INT, RIGHT_PROC(rank, npes), 101,
               MPI_COMM_WORLD, &requestRecv);

      // Update sum.
      for (int j = 0; j < SIZE; j++) {
        sum[j] += X[j];
      }

      // Wait for the Isend and Irecv procedures to be completed.
      MPI_Wait(&requestSend, MPI_STATUS_IGNORE);
      MPI_Wait(&requestRecv, MPI_STATUS_IGNORE);

      // The content of a processor is updated.
      swap(&X, &Xright);

    }

    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime();

    printf("After computation: proc %d, sum %d\n", rank, sum[0]);

	  free(X);
	  free(Xright);
	  free(sum);

    MPI_Finalize();

    if (rank == printer) {
      printf("Elapsed time: %f\n", rank, t2 - t1);
    }


}

