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

        // Access and process the rubric
        std::cout << "[" << ta_name << "] Reviewing rubric items..." << std::endl;
        for (int i = 0; shm_ptr->rubric[i] != '\0'; i++)
        {
            if (shm_ptr->rubric[i + 1] == '\0')
            {
                break;
            }

            std::cout << "  [" << ta_name << "] Rubric Item #" << shm_ptr->rubric[i] << " - Answer: '" << shm_ptr->rubric[i + 3] << "'" << std::endl;
            std::cout << "  [" << ta_name << "] Evaluating correctness..." << std::endl;

            float randomNumber = randomNumGenerator(0.5, 1);
            delay(randomNumber);
            bool decision = randomBool();

            if (decision == false)
            {
                std::cout << "  [" << ta_name << "] ❌ INCORRECT - Current answer '" << shm_ptr->rubric[i + 3] << "' needs correction" << std::endl;
                std::cout << "  [" << ta_name << "] Updating rubric in shared memory..." << std::endl;

                // Use rubric_mutex for rubric modifications
                sem_wait(&shm_ptr->rubric_mutex);
                shm_ptr->rubric[i + 3] += 1;

                // Convert char array to string for writing to file
                std::string rubricString = charArrayToString(shm_ptr->rubric);

                // Write updated rubric back to rubric file
                FILE *rubric_file = fopen("rubric", "w");
                if (rubric_file != NULL)
                {

                    fputs(rubricString.c_str(), rubric_file);
                    fclose(rubric_file);
                }
                std::cout << "  [" << ta_name << "] ✅ Rubric updated successfully " << "Rubric Item #"<< shm_ptr->rubric[i] << " (New value: '" << shm_ptr->rubric[i + 3] << "')" << std::endl;
                sem_post(&shm_ptr->rubric_mutex);
            }
            else
            {
                std::cout << "  [" << ta_name << "] ✓ CORRECT - Item #" << shm_ptr->rubric[i] << " with answer '" << shm_ptr->rubric[i + 3] << "' is valid" << std::endl;
            }

            i += 4;
        }

        sem_wait(&shm_ptr->exam_mutex);
        std::string path = shm_ptr->exams;
        std::string filename = std::filesystem::path(path).filename().string();
        std::string file_number = filename.substr(5, 4);

        // Mark the current exam file (using local path variable)
        std::cout << "\n[" << ta_name << "] 📝 Grading exam " << filename << "..." << std::endl;
        // Get next exam file in critical section

        FILE *check_file = fopen(shm_ptr->exams, "r");
        char first_line[256];

        if (check_file == NULL)
        {
            std::cout << "\n[" << ta_name << "] File not found: " << filename << " - Terminating." << std::endl;
            sem_post(&shm_ptr->exam_mutex);
            break;
        }

        if (fgets(first_line, 256, check_file) != NULL)
        {
            first_line[strcspn(first_line, "\n")] = '\0';

            if (strcmp(first_line, "9999") == 0)
            {
                std::cout << "\n[" << ta_name << "] *** TERMINATION SIGNAL DETECTED (9999) - Stopping work ***" << std::endl;
                fclose(check_file);
                sem_post(&shm_ptr->exam_mutex);
                exit(0);

                break;
            }
        }

        fclose(check_file);

        float randomMarkNumber = randomNumGenerator(1.0, 2.0);
        delay(randomMarkNumber); // Delay outside any lock

        // Write to exam file (no lock needed - each TA works on different files)
        FILE *file_ptr = fopen(path.c_str(), "a");
        if (file_ptr == NULL)
        {
            perror("Error opening file");
            break;
        }

        char data[50];
        fputs("\n", file_ptr);
        snprintf(data, sizeof(data), "graded by: %s ", ta_name);
        fputs(data, file_ptr);
        fclose(file_ptr);

        // Read back the student ID from the exam file
        FILE *file_to_read = fopen(path.c_str(), "r");
        char file_content[256];
        if (fgets(file_content, 256, file_to_read) != NULL)
        {
            file_content[strcspn(file_content, "\n")] = '\0'; // Remove newline
            std::cout << "[" << ta_name << "] ✅ Completed grading " << filename << " for Student ID: " << file_content << std::endl;
            // Increment to next exam while still holding lock
        }
        else
        {
            std::cout << "[" << ta_name << "] ⚠️  Warning: Could not read student ID from file" << std::endl;
        }
        loadNewExam(ta_name, shm_ptr->exams);
        fclose(file_to_read);
        sem_post(&shm_ptr->exam_mutex);
    } // End of while loop

    // 4. Clean up
    if (munmap(shm_ptr, shm_size) == -1)
    {
        perror("munmap failed");
    }
    close(shm_fd);

    std::cout << "\n[" << ta_name << "] 🏁 Work completed. Exiting gracefully." << std::endl;
    exit(0);

    return 0;
}

// Student IDs: 101308485, 101297581