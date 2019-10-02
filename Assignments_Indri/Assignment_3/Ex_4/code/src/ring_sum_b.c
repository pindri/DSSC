#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define SIZE 10000000 // Size of the vectors.
#define LEFT_PROC(rank, npes) (rank - 1 + npes) % npes
#define RIGHT_PROC(rank, npes) (rank + 1 + npes) % npes

void swap (int** A, int** B) {
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

    for (int j = 0; j < npes; j++) {

      // Sent X to the left processor, receive from the right processor..
      if (rank != 0) {

        MPI_Recv(Xright, SIZE, MPI_INT, RIGHT_PROC(rank, npes), 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(X, SIZE, MPI_INT, LEFT_PROC(rank, npes), 101, MPI_COMM_WORLD);

        //printf("proc %d sends %d and receives %d\n", rank, X[0], Xright[0]);

        // Sum received content.
        for (int j = 0; j < SIZE; j++) {
          sum[j] += Xright[j];
        }

        // The content of a processor is updated.
        swap(&X, &Xright);

        //printf("Now proc %d holds %d\n", rank, X[0]);

      } else {

        MPI_Send(X, SIZE, MPI_INT, LEFT_PROC(rank, npes), 101, MPI_COMM_WORLD);
        MPI_Recv(Xright, SIZE, MPI_INT, RIGHT_PROC(rank, npes), 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //printf("proc %d sends %d and receives %d\n", rank, X[0], Xright[0]);

        // Sum received content.
        for (int j = 0; j < SIZE; j++) {
          sum[j] += Xright[j];
        }

        // The content of a processor is updated.
        swap(&X, &Xright);

      }

    }

    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime();

    printf("After computation: proc %d, sum %d\n", rank, sum[0]);

    MPI_Finalize();

  if (rank == printer) {
    printf("I am processor %d, the elapsed time is %f\n", rank, t2 - t1);
  }


  free(X);
  free(Xright);
  free(sum);

}

