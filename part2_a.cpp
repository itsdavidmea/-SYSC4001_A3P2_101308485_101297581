//pass the number of process to main function when startign 



#include <cstdlib>
#include "part2.hpp"

//"To run the program, do: ./interrutps <your_trace_file.txt> <your_vector_table.txt> <your_device_table.txt> <your_external_files.txt>"
int main(int argc, char **argv) {
    

    auto [rubricFile, examFile] = parse_args(argc, argv);
    
    SharedMemory memory;

    memory.rubric = createRubricItem(rubricFile);
    

    return 0; 
}