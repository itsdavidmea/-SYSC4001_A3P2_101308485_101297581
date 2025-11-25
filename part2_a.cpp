//pass the number of process to main function when startign 



#include <cstdlib>
#include "part2.hpp"

//"To run the program, do: ./interrutps <your_trace_file.txt> <your_vector_table.txt> <your_device_table.txt> <your_external_files.txt>"
int main(int argc, char *argv[]) {
    if (argc != 2) {
        // Check if the number of arguments is correct
        return 1; // Return error code if not
    }

    int numProcesses = atoi(argv[1]); // Convert argument to integer

    // Rest of the code to create and manage processes

    // load the rubric and first exam file into shared memeroy -> maybe have a hashmap 

    //all TAs must work with the data in shared memory 

    //take data from main memory and put it in a file 
    SharedMemory memory;
    memory.rubric = RubricItem;

    return 0; 
}