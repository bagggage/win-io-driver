# ISA IO Windows Kernel Driver for x86/x86-64 CPUs

This driver is a simple wrapper over `in`/`out` cpu instructions.

It provides 2 operations:
- *Read*: `in` instruction wrapper
- *Write*: `out` instruction wrapper

# Usage

Before, you have to compile or install driver. See [installation guide](/install/INSTAL.md).

Driver registers device and link it to a file named `ISA-IO`.
You might open it and then can use `IOCTL_*` commands to get access to the `in`/`out`. 

Step-by-step:
- Include header file `ISA_IO.h` into your userspace program.
- Open device handle like this:
  
  ```C
  hDevice = CreateFileW(
	  L"\\??\\ISA-IO",
	  GENERIC_READ | GENERIC_WRITE,
	  FILE_SHARE_READ,
	  NULL,
	  OPEN_EXISTING,
	  FILE_ATTRIBUTE_NORMAL,
	  NULL
  );
  ```
- Use write (`IOCTL_ISA_WRITE_32`):
  ```C
  IsaIoRequestWrite request;
  request.port = ...;
  request.value = ...;

  DeviceIoControl(hDevice, IOCTL_ISA_WRITE_32, &request, sizeof(request), NULL, 0, NULL, NULL);
  ```
- Use read (`IOCTL_ISA_READ_32`):
  ```C
  IsaIoRequestRead request;
  request.port = ...;

  IsaIoResponse response = { 0 };
  DeviceIoControl(hDevice, IOCTL_ISA_READ_32, &request, sizeof(request), &response, sizeof(response), NULL, NULL);

  // Access result
  ... = response.value;
  ```
