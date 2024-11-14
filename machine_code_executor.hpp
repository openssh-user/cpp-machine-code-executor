#ifndef H_MACHINE_CODE_EXECUTOR
#define H_MACHINE_CODE_EXECUTOR

#if defined(_WIN32)
#include <Windows.h>

#elif (defined(__linux__) || defined(__APPLE__))
#include <sys/mman.h>
#endif 

#include <vector>
#include <type_traits>

/*
@brief
Custom allocator for ExecutableMemory container
ExecutableMemory has to allocate executable memory.

In the background, 
executable allocator calls platform specific API functions for allocate
or deallocate memory.

*/
template<typename T>
class ExecutableMemoryAllocator
{
public:
    using value_type = T;
    
    T* allocate(std::size_t size)
    {
        #if defined(WIN32)
        return reinterpret_cast<T*>
        (
            VirtualAlloc(nullptr, sizeof(T) * size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
        );
        
        #elif (defined(__linux__) || defined(__APPLE__))
        return reinterpret_cast<T*>
        (
            mmap(nullptr, sizeof(T) * size, PROT_EXEC | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)
        );

        #endif
    }
    
    void deallocate(void* pointer, std::size_t size)
    {
        #if defined(WIN32)
        VirtualFree(pointer, sizeof(T) * size, MEM_DECOMMIT);

        #elif (defined(__linux__) || defined(__APPLE__))
        munmap(pointer, sizeof(T) * size);

        #endif
    }
};

/*
@brief

Class MachineCodeExecutor provides a public interface to execute a random code
*/
class MachineCodeExecutor
{
public:
    using ExecutableMemory = std::vector<unsigned char, ExecutableMemoryAllocator<unsigned char>>;

    template<typename FunctionType, typename... Args>
    auto Execute(const ExecutableMemory& executable_memory, Args&&... args)
    {
        FunctionType* start_address = (FunctionType*)executable_memory.data();
        return start_address(args...);
    }
};


#endif 
