#include <stdio.h>

#define N 8192 // Number of rows/columns of the matrix.
#define TILE_DIM 32
#define SIZE N*N // Total size of a matrix.


// Compares two matrices element by element.
int isTransposed (const double* a, const double* b, const int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      if(b[j + i*dim] != a[i + j*dim]) return 0;
    }
  }
  return 1;
}



// Gpu naive transposition.
__global__ void gpuNaiveTrans (double* a, double* b, const int size,
                               const int brows) {

  int col = blockIdx.x * TILE_DIM + threadIdx.x;
  int row = blockIdx.y * TILE_DIM + threadIdx.y;
  int width = gridDim.x * TILE_DIM;

  for(int i = 0; i < TILE_DIM; i += brows) {
    b[col * width + (row + i)] = a[(row + i) * width + col];
  }
}



// Gpu optimised transposition.
__global__ void gpuOptTrans (double* a, double* b, const int size,
                             const int brows) {

  // Buffer on the shared memory.
  __shared__ double tmp[TILE_DIM][TILE_DIM];

  int col = blockIdx.x * TILE_DIM + threadIdx.x;
  int row = blockIdx.y * TILE_DIM + threadIdx.y;
  int width = gridDim.x * TILE_DIM;

  // Transposition on buffer.
  for(int i = 0; i < TILE_DIM; i += brows) {
    tmp[threadIdx.y + i][threadIdx.x] = a[(row + i) * width + col];
  }

  __syncthreads();

  col = blockIdx.y * TILE_DIM + threadIdx.x;
  row = blockIdx.x * TILE_DIM + threadIdx.y;

  // Writing to output.
  for(int i = 0; i < TILE_DIM; i += brows) {
    b[col + width * (row + i)] = tmp[threadIdx.x][threadIdx.y + i];
  }
}



void matrixFill (double* a, const int dim) {
  for(int i = 0; i < dim; i++) {
    a[i] = (double) i;
  }
}



int main(int argc, char* argv[]) {

  double* hostInput, * hostOutput;
  double* devInput, * devOutput;

  // Allocate host memory.
  hostInput = (double* )malloc(SIZE * sizeof(double));
  hostOutput = (double* )malloc(SIZE * sizeof(double));

  // Allocate device memory.
  cudaMalloc((void**)&devInput, SIZE * sizeof(double));
  cudaMalloc((void**)&devOutput, SIZE * sizeof(double));

  // Dimensions.
  dim3 grid, block;
  block.x = TILE_DIM;
  block.y = atoi(argv[1])/TILE_DIM; // Threads-per-block readed as input.
  grid.x = N / TILE_DIM;
  grid.y = N / TILE_DIM;

  printf("\n--------------------------\n");
  printf("--------------------------\n\n");

  printf("Threads per block = %d\n\n", atoi(argv[1]));


  /// NAIVE TRANSPOSE //

  // Initialise matricx.
  matrixFill(hostInput, SIZE);

  // Copy input to device.
  cudaMemcpy(devInput, hostInput, SIZE * sizeof(double),
             cudaMemcpyHostToDevice);

  // Timing.
  float elapsedTime = 0.0;
  cudaEvent_t tStart, tEnd;
  cudaEventCreate(&tStart);
  cudaEventCreate(&tEnd);

  cudaEventRecord(tStart);
    gpuNaiveTrans<<< grid, block >>>(devInput, devOutput, N, block.y);
  cudaEventRecord(tEnd);

  cudaEventSynchronize(tEnd);
  cudaEventElapsedTime(&elapsedTime, tStart, tEnd);
  printf("NAIVE TRANSPOSE: Elapsed time: %fms\n", elapsedTime);
  printf("Bandwidth: %f GB/s\n",
         2 * SIZE * sizeof(double) / elapsedTime / 1000000);

  // Copy output to host.
  cudaMemcpy(hostOutput, devOutput, SIZE * sizeof(double),
             cudaMemcpyDeviceToHost);
  
  printf("Is the tranposition correct? %s\n",
         isTransposed(hostOutput, hostInput, N) ? "CORRECT" : "ERROR!" );



  /// OPTIMISED TRANSPOSE //

  printf("\n\n");

  // Initialise matricx.
  matrixFill(hostInput, SIZE);

  // Copy input to device.
  cudaMemcpy(devInput, hostInput, SIZE * sizeof(double),
             cudaMemcpyHostToDevice);

  // Timing.
  elapsedTime = 0.0;

  cudaEventRecord(tStart);
    gpuOptTrans<<< grid, block >>>(devInput, devOutput, N, block.y);
  cudaEventRecord(tEnd);

  cudaEventSynchronize(tEnd);
  cudaEventElapsedTime(&elapsedTime, tStart, tEnd);
  printf("OPTIMISED TRANSPOSE: Elapsed time: %fms\n", elapsedTime);
  printf("Bandwidth: %f GB/s\n",
         2 * SIZE * sizeof(double) / elapsedTime / 1000000);

  // Copy output to host.
  cudaMemcpy(hostOutput, devOutput, SIZE * sizeof(double),
             cudaMemcpyDeviceToHost);
  
  printf("Is the tranposition correct? %s\n",
         isTransposed(hostOutput, hostInput, N) ? "CORRECT" : "ERROR!" );



  // Freeing resources.
  free(hostInput);
  free(hostOutput);
  cudaFree(devInput);
  cudaFree(devOutput);
  cudaEventDestroy(tStart);
  cudaEventDestroy(tEnd);

}
