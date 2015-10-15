#include<stdio.h>
#include<CL/cl.h>
#include<iostream>

int main(void)
{
	cl_int err;
	cl_uint numPlatforms;

	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (CL_SUCCESS == err)
		printf("\nDetected OpenCL platforms: %d", numPlatforms);
	else
		printf("\nError calling clGetPlatformIDs. Error code: %d", err);
	system("pause");
	return 0;
}