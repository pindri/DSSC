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
__global__ void gpuNaiveTrans (const double* a, double* b, const int dim) {
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;
  b[row + col*dim] =  a[row*dim + col];
}

// Cpu optimised transposition.
__global__ void gpuOptTrans (const double* a, double* b, const int dim) {
  __shared__ double tmp[TILE_DIM][TILE_DIM];
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;
  tmp[threadIdx.x][threadIdx.y] = a[row*dim + col];
  __syncthreads();
  b[row + col*dim] =  tmp[threadIdx.x][threadIdx.y];
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

  // Dimension.
  dim3 grid, block;
  block.x = atoi(argv[1]);
  block.y = atoi(argv[2]);
  grid.x = N/block.x;
  grid.y = N/block.y;
  printf("\n--------------------------\n");
  printf("--------------------------\n\n");
  printf("Dimensions: block.x = %d, block.y = %d\n", block.x, block.y);


  // Initialise matricx.
  matrixFill(hostInput, SIZE);


  // Copy input to device.
  cudaMemcpy(devInput, hostInput, SIZE * sizeof(double), cudaMemcpyHostToDevice);

  //// Timing.
  float elapsedTime = 0.0;
  cudaEvent_t tStart, tEnd;
  cudaEventCreate(&tStart);
  cudaEventCreate(&tEnd);


  cudaEventRecord(tStart);
    gpuNaiveTrans<<< grid, block >>>(devInput, devOutput, N);
  cudaEventRecord(tEnd);

  cudaEventSynchronize(tEnd);
  cudaEventElapsedTime(&elapsedTime, tStart, tEnd);
  printf("Elapsed time: %fms\n", elapsedTime);
  printf("Bandwidth: %f GB/s\n", 2 * SIZE * sizeof(double) / elapsedTime / 1000000);

  // Copy output to host.
  cudaMemcpy(hostOutput, devOutput, SIZE * sizeof(double), cudaMemcpyDeviceToHost);
  
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
