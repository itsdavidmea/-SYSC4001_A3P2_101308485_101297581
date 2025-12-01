#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "part2.hpp"

int main(int argc, char *argv[]) {
    const char* ta_name = argv[1]; // This will be "TA1" or "TA2"
    std::cout << "--- Shared Memory Consumer (Reader) ---" << std::endl;

    // 1. Open the shared memory object (O_RDWR for read-write access)
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed. Is the Producer running?");
        return 1;
    }
    std::cout << "Shared Memory object opened: " << SHM_NAME << std::endl;

    // We don't need ftruncate since the Producer already sized it.
    size_t shm_size = sizeof(SharedMemory);

    // 2. Map the shared memory object into the process's address space
    // PROT_READ | PROT_WRITE: Allow reading and writing to the memory
    SharedMemory* shm_ptr = (SharedMemory*)mmap(
        0,              // let the kernel choose the address
        shm_size,       // size of the mapping
        PROT_READ | PROT_WRITE,  // protection (read and write)
        MAP_SHARED,     // sharing flags (shared mapping)
        shm_fd,         // file descriptor of the shm object
        0               // offset from the start of the object
    );
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        return 1;
    }
    std::cout << "Shared Memory successfully mapped." << std::endl;

    // 3. Read data from the shared memory
    std::cout << "\n--- Data Read from Shared Memory ---" << std::endl;
    std::cout << "Exams Count: " << shm_ptr->exams << std::endl;
    std::cout << "Rubric:      " << shm_ptr->rubric << std::endl;

    for (int i = 0; shm_ptr->rubric[i] != '\0';  i++)
    {
        if (shm_ptr->rubric[i + 1] == '\0')
        {
            break;
        }
        
        std::cout << ta_name << " looking rubic number: " << shm_ptr->rubric[i] << " "<< "rubric content is: " <<  shm_ptr->rubric[i + 3] << '\n';
        i += 4;
    }
    
    std::cout << "------------------------------------" << std::endl;

    // 4. Clean up
    if (munmap(shm_ptr, shm_size) == -1) {
        perror("munmap failed");
    }
    close(shm_fd);
    
    std::cout << "Consumer exiting." << std::endl;

    return 0;
}