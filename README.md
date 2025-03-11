# C++ Machine Code Executor
Just done for testing purposes.

Usage sample is simple as that:
```cpp
#include "machine_code_executor.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	MachineCodeExecutor::ExecutableMemory mem  =
	// 48 05 80 add rax,0x80
	// 00 00 00 C3 ret
	{ 0x48, 0x05, 0x80, 0x00, 0x00, 0x00, 0xC3 };

	MachineCodeExecutor executor;
	try
	{
		std::cout << executor.Execute<int(int)>(mem, 128) << std::endl;
	}
	catch(std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
```

Result:
```console
[user@host machine_code_executor]$ ./machine_code_executor
256
```
