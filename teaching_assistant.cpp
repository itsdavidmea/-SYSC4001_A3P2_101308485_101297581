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
    
   
    // 1. Open the shared memory object (O_RDWR for read-write access)
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed. Is the Producer running?");
        return 1;
    }


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

    char examFile[50];
    if (shm_ptr->exams > 0 && shm_ptr->exams <= 9)
    {
        snprintf(examFile, sizeof(examFile), "exams/exam_000%d.txt", shm_ptr->exams);
    } else {
        snprintf(examFile, sizeof(examFile), "exams/exam_00%d.txt", shm_ptr->exams);
    }
    

   
 
    std::cout << ta_name << " accessing rubric" << std::endl;
    for (int i = 0; shm_ptr->rubric[i] != '\0';  i++)
    {
        if (shm_ptr->rubric[i + 1] == '\0')
        {
            break;
        }
        
        std::cout << ta_name << " looking rubic number: " << shm_ptr->rubric[i] << " "<< "rubric content is: " <<  shm_ptr->rubric[i + 3] << '\n';
        float randomNumber = randomNumGenerator(0.5, 1);
        std::cout << ta_name << " is rubric correct ? " << '\n';
        delay(randomNumber);
        bool decision;
        decision = true;
        if (decision == true)
        {
            shm_ptr->rubric[i + 3] += 1;
            
            // Convert char array to string for writing to file
            std::string rubricString = charArrayToString(shm_ptr->rubric);
            
            // Write updated rubric back to rubric file
            FILE *rubric_file = fopen("rubric", "w");
            if (rubric_file != NULL) {
                std::cout << ta_name << " is writting to the rubric " << '\n';
                fputs(rubricString.c_str(), rubric_file);
                fclose(rubric_file);
            }
        } else {

        }
        


        i += 4;
    }

    
    

    FILE *file_ptr;
    file_ptr = fopen(examFile, "a");
    // 3. Check for errors
    if (file_ptr == NULL) {
        perror("Error opening file");
        return 0; 
    }
    std::cout << ta_name << " picked Exam #" <<  shm_ptr->exams <<std::endl;
    
    std::cout << ta_name << " is marking Exam #" <<  shm_ptr->exams <<std::endl;

    // 4. Write data
    
    // Write the raw string data and a newline
    float randomNumber = randomNumGenerator(1.0, 2.0);
    delay(randomNumber);
    char data[50];
    fputs("\n", file_ptr);
    
    snprintf(data, sizeof(data), "graded by: %s ", ta_name);
    fputs(data, file_ptr); 
    

    // 5. Close the file
    if (fclose(file_ptr) != 0) {
        perror("Error closing file");
    }


    FILE *file_to_read;
    file_to_read = fopen(examFile, "r");
    char buffer[256];
    if (fgets(buffer, 256, file_to_read) != NULL) {
      
        std::cout << ta_name << " marked Exam #" <<  shm_ptr->exams << " for student: " << buffer <<std::endl;
        
        // If fgets didn't read a full line (i.e., the buffer isn't newline-terminated),
        // print a newline for clean terminal output.
        if (buffer[strlen(buffer) - 1] != '\n') {
            printf("\n");
        }
    } else {
        printf("File is empty or could not be read.\n");
    }

    // 4. Close the file
    fclose(file_to_read);

    
  
    // 4. Clean up
    if (munmap(shm_ptr, shm_size) == -1) {
        perror("munmap failed");
    }
    close(shm_fd);
    
    std::cout << ta_name << " exiting." << std::endl;

    return 0;
}