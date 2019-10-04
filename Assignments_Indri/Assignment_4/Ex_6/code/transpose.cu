#include <stdio.h>

#define N 8192 // Number of rows/columns of the matrix.
#define TILE_DIM 32
#define SIZE N*N // Total size of a matrix.


// Compares two matrices element by element.
int isTransposed (const int* a, const int* b, const int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      if(b[j + i*dim] != a[i + j*dim]) return 0;
    }
  }
  return 1;
}

// Gpu naive transposition.
__global__ void gpuNaiveTrans (const int* a, int* b, const int dim) {
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;
  b[row + col*dim] =  a[row*dim + col];
}

// Cpu optimised transposition.
__global__ void gpuOptTrans (const int* a, int* b, const int dim) {
  __shared__ double tmp[TILE_DIM][TILE_DIM];
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;
  tmp[threadIdx.x][threadIdx.y] = a[row*dim + col];
  __syncthreads();
  b[row + col*dim] =  tmp[threadIdx.x][threadIdx.y];
}


void matrixFill (int* a, const int dim) {
  for(int i = 0; i < dim; i++) {
    a[i] = i + 1;
  }
}


int main(int argc, char* argv[]) {


  int* hostInput, * hostOutput;
  int* devInput, * devOutput;

  // Allocate host memory.
  hostInput = (int* )malloc(SIZE * sizeof(int));
  hostOutput = (int* )malloc(SIZE * sizeof(int));

  // Allocate device memory.
  cudaMalloc((void**)&devInput, SIZE * sizeof(int));
  cudaMalloc((void**)&devOutput, SIZE * sizeof(int));

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
  cudaMemcpy(devInput, hostInput, SIZE * sizeof(int), cudaMemcpyHostToDevice);

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

  // Copy output to host.
  cudaMemcpy(hostOutput, devOutput, SIZE * sizeof(int), cudaMemcpyDeviceToHost);
  
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
