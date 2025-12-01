#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

#include "part2.hpp"

int main(int argc, char *argv[])
{
    const char *ta_name = argv[1]; // This will be "TA1" or "TA2"

    // 1. Open the shared memory object (O_RDWR for read-write access)
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open failed. Is the Producer running?");
        return 1;
    }

    // We don't need ftruncate since the Producer already sized it.
    size_t shm_size = sizeof(SharedMemory);

    // 2. Map the shared memory object into the process's address space
    // PROT_READ | PROT_WRITE: Allow reading and writing to the memory
    SharedMemory *shm_ptr = (SharedMemory *)mmap(
        0,                      // let the kernel choose the address
        shm_size,               // size of the mapping
        PROT_READ | PROT_WRITE, // protection (read and write)
        MAP_SHARED,             // sharing flags (shared mapping)
        shm_fd,                 // file descriptor of the shm object
        0                       // offset from the start of the object
    );
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap failed");
        close(shm_fd);
        return 1;
    }

    // Main loop: keep processing exams until we encounter exam with content 9999
    while (true)
    {
        // Read the current exam file path from shared memory
        std::string path = shm_ptr->exams;
        std::string filename = std::filesystem::path(path).filename().string();
        
        // Read the content of the exam file to check for termination condition
        FILE *check_file = fopen(shm_ptr->exams, "r");
        char first_line[256];
        bool should_exit = false;
        
        if (check_file != NULL && fgets(first_line, 256, check_file) != NULL)
        {
            // Remove trailing newline if present
            first_line[strcspn(first_line, "\n")] = '\0';
            
            // Check if the content is "9999"
            if (strcmp(first_line, "9999") == 0)
            {
                std::cout << ta_name << " encountered exam with content 9999, stopping." << std::endl;
                should_exit = true;
            }
        }
        
        if (check_file != NULL)
        {
            fclose(check_file);
        }
        
        if (should_exit)
        {
            break;
        }

        std::cout << "\n" << ta_name << " processing " << filename << std::endl;
        
        // Extract file number from filename for display purposes
        std::string file_number = filename.substr(5, 4);

        // Access and process the rubric
        std::cout << ta_name << " accessing rubric" << std::endl;
        for (int i = 0; shm_ptr->rubric[i] != '\0'; i++)
        {
            if (shm_ptr->rubric[i + 1] == '\0')
            {
                break;
            }

            std::cout << ta_name << " looking rubic number: " << shm_ptr->rubric[i] << " " << "rubric content is: " << shm_ptr->rubric[i + 3] << '\n';
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
                if (rubric_file != NULL)
                {
                    std::cout << ta_name << " is writting to the rubric " << '\n';
                    fputs(rubricString.c_str(), rubric_file);
                    fclose(rubric_file);
                }
            }

            i += 4;
        }

        // Open and mark the current exam file
        FILE *file_ptr;
        file_ptr = fopen(shm_ptr->exams, "a");
        if (file_ptr == NULL)
        {
            perror("Error opening file");
            break;  // Exit the loop if we can't open the file
        }
        
        std::cout << ta_name << " picked " << filename << std::endl;
        std::cout << ta_name << " is marking Exam " << file_number << std::endl;
        
        float randomMarkNumber = randomNumGenerator(1.0, 2.0);
        delay(randomMarkNumber);
        
        char data[50];
        fputs("\n", file_ptr);
        snprintf(data, sizeof(data), "graded by: %s ", ta_name);
        fputs(data, file_ptr);

        if (fclose(file_ptr) != 0)
        {
            perror("Error closing file");
        }

        // Read back the student ID from the exam file
        FILE *file_to_read;
        file_to_read = fopen(shm_ptr->exams, "r");
        char file_content[256];
        if (fgets(file_content, 256, file_to_read) != NULL)
        {
            std::cout << ta_name << " marked " << filename << " for student: " << file_content;

            if (file_content[strlen(file_content) - 1] != '\n')
            {
                printf("\n");
            }
        }
        else
        {
            printf("File is empty or could not be read.\n");
        }
        fclose(file_to_read);

        // Load next exam file
        loadNewExam(shm_ptr->exams);
        
    }  // End of while loop

    // 4. Clean up
    if (munmap(shm_ptr, shm_size) == -1)
    {
        perror("munmap failed");
    }
    close(shm_fd);

    std::cout << ta_name << " exiting." << std::endl;

    return 0;
}