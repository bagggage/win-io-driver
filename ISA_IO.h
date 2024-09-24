#pragma once

#ifndef KERNEL_SPACE
#include <windows.h>
#endif

#define IOCTL_ISA_READ_32 CTL_CODE(FILE_DEVICE_UNKNOWN, 0xA30B | 0xCB0 | 0xB0BA, METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)
#define IOCTL_ISA_WRITE_32 CTL_CODE(FILE_DEVICE_UNKNOWN, 0xDEAD | 0xC00L , METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)
#define IOCTL_MMAP_MMIO CTL_CODE(FILE_DEVICE_UNKNOWN, 0xB0BADEAD , METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)
#define IOCTL_UMAP_MMIO CTL_CODE(FILE_DEVICE_UNKNOWN, 0xCB0228 , METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)
#define IOCTL_MMIO_READ_32 CTL_CODE(FILE_DEVICE_UNKNOWN, 0xDEADBEAF , METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)
#define IOCTL_MMIO_WRITE_32 CTL_CODE(FILE_DEVICE_UNKNOWN, 0xDEADBAD , METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)

struct IsaIoRequestRead {
	unsigned short port;
};

struct IsaIoRequestWrite {
	unsigned short port;
	unsigned int value;
};

struct IsaIoResponse {
	unsigned int value;
};

typedef struct IsaIoResponse IoResponse;

struct IoRequestMmap {
	void* phys;
	unsigned int size;
};

struct IoRequestWrite {
	void* virt;
	unsigned int value;
};

struct IoRequestRead {
	void* virt;
};

struct IoRequestUnmap {
	void* virt;
	unsigned int size;
};

struct IoResponseMmap {
	void* virt;
};

#ifndef KERNEL_SPACE

unsigned int IsaIoRead(HANDLE hDevice, unsigned short port) {
	struct IsaIoRequestRead request;
	struct IsaIoResponse response;
	request.port = port;

	if (!DeviceIoControl(
		hDevice, IOCTL_ISA_READ_32,
		&request, sizeof(request),
		&response, sizeof(response),
		NULL, NULL
	)) return 0xFFFFFFFF;

	return response.value;
}

bool IsaIoWrite(HANDLE hDevice, unsigned short port, unsigned int value) {
	struct IsaIoRequestWrite request;
	request.port = port;
	request.value = value;

	return DeviceIoControl(hDevice, IOCTL_ISA_WRITE_32, &request, sizeof(request), NULL, 0, NULL, NULL);
}

unsigned int MmIoRead(HANDLE hDevice, void* address) {
	struct IoRequestRead request;
	IoResponse response;

	request.virt = address;

	if (!DeviceIoControl(
		hDevice, IOCTL_MMIO_READ_32,
		&request, sizeof(request),
		&response, sizeof(response),
		NULL, NULL
	)) return 0xFFFFFFFF;

	return response.value;
}

bool MmIoWrite(HANDLE hDevice, void* address, unsigned int value) {
	struct IoRequestWrite request;
	request.virt = address;
	request.value = value;

	return DeviceIoControl(hDevice, IOCTL_MMIO_WRITE_32, &request, sizeof(request), NULL, 0, NULL, NULL);
}

void* MmIoMmap(HANDLE hDevice, void* physical, unsigned int size) {
	struct IoRequestMmap request;
	struct IoResponseMmap response;
	request.phys = physical;
	request.size = size;

	if (!DeviceIoControl(
		hDevice, IOCTL_MMAP_MMIO,
		&request, sizeof(request),
		&response, sizeof(response),
		NULL, NULL
	)) return NULL;

	return response.virt;
}

bool MmIoUnmap(HANDLE hDevice, void* virt, unsigned int size) {
	struct IoRequestUnmap request;
	request.virt = virt;
	request.size = size;

	return DeviceIoControl(hDevice, IOCTL_UMAP_MMIO, &request, sizeof(request), NULL, 0, NULL, NULL);
}

#endif
