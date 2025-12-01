#include <iostream>
#include <cstdio>

void resetExamFiles()
{
    std::cout << "Resetting all exam files to initial state..." << std::endl;
    
    for (int i = 1; i <= 20; i++)
    {
        char filename[64];
        snprintf(filename, sizeof(filename), "exams/exam_%04d.txt", i);
        
        FILE* file = fopen(filename, "w");
        if (file != NULL)
        {
            if (i == 20)
            {
                fprintf(file, "9999\n");
            }
            else
            {
                fprintf(file, "%04d\n", i);
            }
            fclose(file);
            std::cout << "✓ Reset " << filename << std::endl;
        }
        else
        {
            std::cerr << "✗ Failed to reset " << filename << std::endl;
        }
    }
    
    std::cout << "All exam files have been reset to their initial state." << std::endl;
}

void resetRubricFile()
{
    std::cout << "\nResetting rubric file to initial state..." << std::endl;
    
    FILE* file = fopen("rubric", "w");
    if (file != NULL)
    {
        fprintf(file, "1, A\n");
        fprintf(file, "2, B\n");
        fprintf(file, "3, C\n");
        fprintf(file, "4, D\n");
        fprintf(file, "5, E\n");
        fclose(file);
        std::cout << "✓ Reset rubric file" << std::endl;
    }
    else
    {
        std::cerr << "✗ Failed to reset rubric file" << std::endl;
    }
}

int main()
{
    resetExamFiles();
    resetRubricFile();
    
    std::cout << "\nAll files have been reset to their initial state." << std::endl;
    
    return 0;
}

//Student IDs: 101308485, 101297581