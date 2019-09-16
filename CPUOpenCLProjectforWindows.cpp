#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <memory.h>
#include <vector>

#include "CL\cl.h"
#include "utils.h"

#include <Windows.h>

#define OPENCL_VERSION_1_2  1.2f
#define SIZE 30
#define COUNT_STRING 30

struct ocl_args_d_t
{
	ocl_args_d_t();
	~ocl_args_d_t();

	// Regular OpenCL objects:
	cl_context       context;           // hold the context handler
	cl_device_id     device;            // hold the selected device handler
	cl_command_queue commandQueue;      // hold the commands-queue handler
	cl_program       program;           // hold the program handler
	cl_kernel        kernel;            // hold the kernel handler
	float            platformVersion;   // hold the OpenCL platform version (default 1.2)
	float            deviceVersion;     // hold the OpenCL device version (default. 1.2)
	float            compilerVersion;   // hold the device OpenCL C version (default. 1.2)

	// Objects that are specific for algorithm implemented in this sample
	cl_mem           srcA;              // hold first source buffer
	cl_mem           srcB;              // hold second source buffer
	cl_mem           dstMem;            // hold destination buffer
	cl_mem srcBVer;
	cl_mem srcCount;
};

ocl_args_d_t::ocl_args_d_t() :
	context(NULL),
	device(NULL),
	commandQueue(NULL),
	program(NULL),
	kernel(NULL),
	platformVersion(OPENCL_VERSION_1_2),
	deviceVersion(OPENCL_VERSION_1_2),
	compilerVersion(OPENCL_VERSION_1_2),
	srcA(NULL),
	srcB(NULL),
	srcBVer(NULL),
	srcCount(NULL),
	dstMem(NULL)
{
}
ocl_args_d_t::~ocl_args_d_t()
{
	cl_int err = CL_SUCCESS;
}

cl_platform_id FindOpenCLPlatform(const char* preferredPlatform, cl_device_type deviceType)
{
	cl_uint numPlatforms = 0;
	cl_int err = CL_SUCCESS;
	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	std::vector<cl_platform_id> platforms(numPlatforms);
	err = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);

	for (cl_uint i = 0; i < numPlatforms; i++)
	{
		cl_uint numDevices = 0;
		err = clGetDeviceIDs(platforms[i], deviceType, 0, NULL, &numDevices);
		if (0 != numDevices)
		{
			return platforms[i];
		}
	}
	return NULL;
}


int writeInArrayVer(char inputArray[][SIZE], int countOfString)//создаем большой массив строк и суем туда все строки из файла
{
	int i2 = countOfString; int j2 = 0; int p;
	for (int i = 0; i < SIZE; i++) {
		int c = 0;
		for (int j = 0; j < countOfString; j++) {
			if (inputArray[j][i] == '1' || inputArray[j][i] == '0')
			{
				inputArray[i2][j2] = inputArray[j][i]; j2++; c++; p = 0;
			}
			else {
				if (p == 0)
				{
					i2++;
					j2 = 0;
				}p++;
			}

		}
		if (p == 0) {
			i2++; p++;
		} j2 = 0; if (c == 0)return i2;
	}
	return i2;
}

int writeInArray(char inputArray[][SIZE], int countString[])//создаем большой массив строк и суем туда все строки из файла
{
	FILE *f;
	f = fopen("test.txt", "r");

	int count_of_all_file = 0; //char *s = 1;
	int num_string = -1;
	while (num_string == -1 || *inputArray[num_string] != 'М')
	{//идем по всему файлу и записываем в массив строк все строки)
		num_string++;
		fgets(inputArray[num_string], SIZE, f);
		count_of_all_file += strlen(inputArray[num_string]) - 1;
	}
	count_of_all_file -= (strlen(inputArray[num_string]) - 1);
	fclose(f);
	countString[0] = num_string;
	return count_of_all_file;
}

void SetupOpenCL(ocl_args_d_t *ocl, cl_device_type deviceType)
{
	cl_platform_id platformId = FindOpenCLPlatform("Intel", deviceType);
	ocl->context = clCreateContextFromType(0, deviceType, NULL, NULL, NULL);
	clGetContextInfo(ocl->context, CL_CONTEXT_DEVICES, sizeof(cl_device_id), &ocl->device, NULL);
	ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, 0, NULL);
}

void CreateAndBuildProgram(ocl_args_d_t *ocl)
{
	char* source = NULL;
	size_t src_size = 0;
	ReadSourceFromFile("Template.cl", &source, &src_size);
	ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char**)&source, &src_size, NULL);
	clBuildProgram(ocl->program, 1, &ocl->device, "", NULL, NULL);
}

void ExecuteAddKernel(ocl_args_d_t *ocl, int countOfString)
{
	size_t globalWorkSize = countOfString;
	clEnqueueNDRangeKernel(ocl->commandQueue, ocl->kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
	clFinish(ocl->commandQueue);//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
}

int ReadAndVerify(ocl_args_d_t *ocl, int *Ver)
{
	cl_int err = CL_SUCCESS; 
	cl_int *resultPtr = (cl_int *)clEnqueueMapBuffer(ocl->commandQueue, ocl->srcB, true, CL_MAP_READ, 0, sizeof(int) * SIZE, 0, NULL, NULL, &err); //clFinish(ocl->commandQueue);
	cl_int *resultPtrVer = (cl_int *)clEnqueueMapBuffer(ocl->commandQueue, ocl->srcBVer, true, CL_MAP_READ, 0, sizeof(int) * SIZE, 0, NULL, NULL, &err);
	clFinish(ocl->commandQueue);
	//clFinish(ocl->commandQueue);
	int tired; int sum = 0;
	scanf("%d", &tired); int tired2 = tired;
	for (int i = SIZE - 1; tired > 0; i--)
	{
		while (resultPtr[i] > 0 && tired != 0)
		{
			resultPtr[i]--;
			tired--;
			sum += i;
		}if (i < 0 && tired != 0){sum = -1; break;
	}
	}
	for (int i = SIZE - 1; tired2 > 0; i--)
	{
		while (resultPtrVer[i] > 0 && tired2 != 0)
		{
			resultPtrVer[i]--;
			tired2--;
			*Ver += i;
		}
		if (i < 0 && tired2 != 0)
		{
			*Ver = -1; break;
		}
	}
	return sum;
}

int _tmain(int argc, TCHAR* argv[])
{
	cl_int err;
	ocl_args_d_t ocl;
	cl_device_type deviceType = CL_DEVICE_TYPE_CPU;

	LARGE_INTEGER perfFrequency;//time
	LARGE_INTEGER performanceCountNDRangeStart;
	LARGE_INTEGER performanceCountNDRangeStop;

	SetupOpenCL(&ocl, deviceType);//информация о платфоре тыры-пыры все дела
	char inputAllFile[COUNT_STRING][SIZE];
	int Res[30]; memset(Res, 0, 30 * sizeof(int));
	int ResVer[SIZE]; memset(ResVer, 0, SIZE * sizeof(int));
	int countOfString = 0;

	int countOfsymhor = writeInArray(inputAllFile, &countOfString);
	int countOfStringVer = writeInArrayVer(inputAllFile, countOfString);

	//Create Buffer Arguments 
	ocl.srcA = clCreateBuffer(ocl.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(char) * COUNT_STRING * SIZE, inputAllFile, NULL);
	ocl.srcB = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int) * SIZE, Res, NULL);
	ocl.srcBVer = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int) * SIZE, ResVer, NULL);
	ocl.srcCount = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &countOfString, NULL);

	CreateAndBuildProgram(&ocl);
	ocl.kernel = clCreateKernel(ocl.program, "Add", &err);

	//Set Kernel Arguments
	clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), (void *)&ocl.srcA);
	clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), (void *)&ocl.srcB);
	clSetKernelArg(ocl.kernel, 2, sizeof(cl_mem), (void *)&ocl.srcBVer);
	clSetKernelArg(ocl.kernel, 3, sizeof(cl_mem), (void *)&ocl.srcCount);

	QueryPerformanceCounter(&performanceCountNDRangeStart);
	ExecuteAddKernel(&ocl, countOfStringVer);
	QueryPerformanceCounter(&performanceCountNDRangeStop);
	QueryPerformanceFrequency(&perfFrequency);
	LogInfo("NDRange performance counter time %f ms.\n",
		1000.0f*(float)(performanceCountNDRangeStop.QuadPart - performanceCountNDRangeStart.QuadPart) / (float)perfFrequency.QuadPart);


	int Ver = 0; int Hor;
int mag = ReadAndVerify(&ocl, &Ver); 
	if (mag == -1)
		Hor = 0;
	else Hor=countOfsymhor - mag;
	if (Ver == -1) 
		Ver = 0;
	else 
		Ver = countOfsymhor - Ver;
	printf("Hor %d\n", Hor);
	printf("Ver %d\n", Ver);
	if (Hor < Ver)
		printf("min %d\n", Hor);
	else
		printf("min %d\n", Ver);

	system("pause");
	return 0;
}
