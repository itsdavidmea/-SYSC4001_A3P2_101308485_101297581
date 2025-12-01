// pass the number of process to main function when startign

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <iostream>
#include <vector>
#include <string>
#include "part2.hpp"

//"To run the program, do: ./interrutps <your_trace_file.txt> <your_vector_table.txt> <your_device_table.txt> <your_external_files.txt>"
int main(int argc, char **argv)
{

    auto [rubricFile, examFile, numTAs] = parse_args(argc, argv);


    std::vector<TA> allTAs;
    

   
    allTAs = createTAs(numTAs);
    
    std::cout << "Number of TAs: " << numTAs << "\n\n";
   
    printTAs(allTAs);
    std::cout << "\n";

    std::cout << "--- Shared Memory Producer (Writer) ---" << std::endl;
    

    // 1. Create and open the shared memory object

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666 );
    if (shm_fd == -1)
    {
        perror("shm_open failed");
        return 1;
    }
    
    std::cout << "Shared Memory object created/opened: " << SHM_NAME << std::endl;

    // 2. Set the size of the shared memory object
    size_t shm_size = sizeof(SharedMemory);
    if (ftruncate(shm_fd, shm_size) == -1 ) 
    {
        perror("ftruncate failed");
        shm_unlink(SHM_NAME);
        return 1;
    }
    
  
    std::cout << "Shared Memory size set to " << shm_size << " bytes." << std::endl;

    // 3. Map the shared memory object into the process's address space
    // PROT_WRITE: Allow writing to the memory
    // MAP_SHARED: Changes are visible to other processes
    SharedMemory *shm_ptr = (SharedMemory*) mmap (
        0,
        shm_size,
        PROT_WRITE,
        MAP_SHARED,
        shm_fd,
        0
    );

    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap failed");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }
    
    
    std::cout << "Shared Memory successfully mapped." << std::endl;

    // 4. Write data to the shared memory
    shm_ptr->exams = parseExams(examFile);


    char *conversion = rubricToCharArray(rubricFile);
    strncpy(shm_ptr->rubric, conversion, RUBRIC_SIZE - 1);
    shm_ptr->rubric[RUBRIC_SIZE - 1] = '\0';
    // Copy the string safely into the fixed-size array
    
    std::cout << "Data written: " 
              << "Exams: " << shm_ptr->exams 
              << ", Rubric: \n" << shm_ptr->rubric << "" << std::endl;
    
    std::cout << "Data is now live in shared memory." << std::endl;

    // Fork child processes to run teaching_assistant
    pid_t pid;
    for (int i = 0; i < numTAs; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            std::cerr << "Fork unsuccessful" << std::endl;
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process - exec to teaching_assistant program
            std::string ta_name = "TA" + std::to_string(i + 1);
            execl("./teaching_assistant", "teaching_assistant", ta_name.c_str(), NULL);
            
            // If exec fails, print error and exit
            perror("execl failed");
            exit(1);
        }
    }

    // Parent process - wait for all children to finish
    for (int i = 0; i < numTAs; i++)
    {
        wait(NULL);
    }
    
    std::cout << "All teaching assistants finished." << std::endl;

    // 5. Clean up
    if (munmap(shm_ptr, shm_size) == -1) {
        perror("munmap failed");
    }
    close(shm_fd);
    
    // Unlink: This removes the shared memory object from the system.
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink failed");
    }
    std::cout << "Shared Memory object unlinked and producer exiting." << std::endl;

    return 0;
}