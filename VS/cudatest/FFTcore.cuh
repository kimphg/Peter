#ifndef FFT_CORE
#define FFT_CORE
#pragma once
// includes for FFT
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cufft.h>
#include <cufftXt.h>
#include <stdio.h>

__global__ void complexMulKernel(cufftComplex *res, const cufftComplex *v1, const cufftComplex *v2)
{
	int i = threadIdx.x;
	res[i].x = v1[i].x * v2[i].x - v1[i].y * v2[i].y;
	res[i].y = v1[i].x * v2[i].y + v1[i].y * v2[i].x;
}
class coreFFT
{
public:


	cufftHandle planTL;
	cufftHandle planNenTH;
	//cufftHandle planImageFFT;
	cufftComplex *dSignalTL;
	cufftComplex *dSignalNen;
	cufftComplex *dImageNen;
	int mMemSizeTL;
	int mMemSizeNen;
	int mMemSizeImage;
	int mTichLuySize;//16
	int mFrameLen;
	coreFFT(int frameLen, int ntichluy)
	{
		mFrameLen = frameLen;
		mTichLuySize = ntichluy;
		mMemSizeTL = sizeof(cufftComplex)* mTichLuySize*frameLen;
		mMemSizeNen = sizeof(cufftComplex)*frameLen;
		mMemSizeImage = sizeof(cufftComplex)*frameLen;

		if (cufftPlan1d(&planTL, mTichLuySize, CUFFT_C2C, frameLen) != CUFFT_SUCCESS)printf("\nFFT planTL failed to init");
		// Allocate device memory for signal tich luy
		cudaMalloc((void **)&dSignalTL, mMemSizeTL);

		if (cufftPlan1d(&planNenTH, frameLen, CUFFT_C2C, 1) != CUFFT_SUCCESS)printf("\nFFT planTL failed to init");
		// Allocate device memory for signal nen
		cudaMalloc((void **)&dSignalNen, mMemSizeNen);

		//if (cufftPlan1d(&planImageFFT, frameLen, CUFFT_C2C, 1) != CUFFT_SUCCESS)printf("\nFFT planTL failed to init");
		// Allocate device memory for image nen
		cudaMalloc((void **)&dSignalNen, mMemSizeNen);
	}
	void exeFFTTL(cufftComplex *h_signal)
	{
		cudaMemcpy(dSignalTL, h_signal, mMemSizeTL, cudaMemcpyHostToDevice);
		cufftExecC2C(planTL, dSignalTL, dSignalTL, CUFFT_FORWARD);
		cudaMemcpy(h_signal, dSignalTL, mMemSizeTL, cudaMemcpyDeviceToHost);
	}
	void exeFFTNen(cufftComplex *h_signal, cufftComplex* h_image)
	{
		//move signal to gpu and process fft forward
		cudaMemcpy(dSignalNen, h_signal, mMemSizeNen, cudaMemcpyHostToDevice);
		cufftExecC2C(planNenTH, dSignalNen, dSignalNen, CUFFT_FORWARD);
		//move image to gpu and process fft forward
		cudaMemcpy(dImageNen, h_image, mMemSizeNen, cudaMemcpyHostToDevice);
		cufftExecC2C(planNenTH, dImageNen, dImageNen, CUFFT_FORWARD);
		// Element wise complext multiplication
		complexMulKernel << <1, mFrameLen >> >(dSignalNen, dSignalNen, dImageNen);
		//
		cufftExecC2C(planNenTH, dSignalNen, dSignalNen, CUFFT_INVERSE);
		cudaMemcpy(h_signal, dSignalNen, mMemSizeNen, cudaMemcpyDeviceToHost);
	}
	~coreFFT()
	{
		cufftDestroy(planTL);
		cufftDestroy(planNenTH);
		// cleanup memory
		cudaFree(dSignalTL);
		cudaFree(dSignalNen);
		cudaFree(dImageNen);
	}
};

#endif


