/** Compile: nvcc example4.cu -lGL -lGLU -lglut **/

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "utils/cheader.h"
#include "utils/cpu_bitmap.h"

#define WIDTH	1024
#define HEIGHT	768
#define SCALEX	1.500
#define SCALEY	1.500

struct cuComplex {
	float real, img;
	
	__device__ cuComplex(float a, float b) : real(a), img(b) {}
	__device__ float magnitude2() {
		return (real * real) + (img * img);
	}
	__device__ cuComplex operator*(const cuComplex &a) {
		return cuComplex( ((real * a.real) - (img * a.img)) ,
						  ((img * a.real) + (real * a.img)) );
	}
	__device__ cuComplex operator+(const cuComplex &a) {
		return cuComplex( (real + a.real), (img + a.img) );
	}
};

enum color {RED, GREEN, BLUE, ALPHA};

__device__ int julia_value(int x, int y, int width, int height) {
	int k;
	float jx = SCALEX * (float) (width / 2 - x) / (width / 2);
	float jy = SCALEY * (float) (height / 2 - y) / (height / 2);
	cuComplex c(-0.8, 0.156);
	cuComplex a(jx, jy);
	
	for (k = 0; k < 200; k++) {
		a = a*a + c;
		if (a.magnitude2() > 1000) {
			return 0;
		}
	}
	return 1;
}

__global__ void julia_set(unsigned char *ptr, int width, int height) {
	int x = blockIdx.x;
	int y = blockIdx.y;
	int offset = x + y * gridDim.x;
	
	int value = julia_value(x, y, width, height);
	ptr[offset*4 + RED]   = (unsigned char) (255 * (0.4 * value));
	ptr[offset*4 + GREEN] = (unsigned char) (255 * (0.5 * value));
	ptr[offset*4 + BLUE]  = (unsigned char) (255 * (0.7 * value));
	ptr[offset*4 + ALPHA] = 255;
}

int main(int argc, char* argv[]) {
	CPUBitmap bitmap(WIDTH, HEIGHT);
	unsigned char *dev_bitmap;
	dim3 grid(WIDTH, HEIGHT);
	double ms;
	
	cudaMalloc((void**) &dev_bitmap, bitmap.image_size());
	
	printf("Starting...\n");
	ms = 0;
	for (int i = 0; i < N; i++) {
		start_timer();
		julia_set<<<grid, 1>>>(dev_bitmap, WIDTH, HEIGHT);
		ms += stop_timer();
	}
	
	cudaMemcpy(bitmap.get_ptr(), dev_bitmap, bitmap.image_size(), cudaMemcpyDeviceToHost);
	cudaFree(dev_bitmap);
	
	printf("avg time = %.5lf ms\n", (ms/N));
	bitmap.display_and_exit();
	return 0;
}
	
	
	
	
