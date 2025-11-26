//pass the number of process to main function when startign 



#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "part2.hpp"

//"To run the program, do: ./interrutps <your_trace_file.txt> <your_vector_table.txt> <your_device_table.txt> <your_external_files.txt>"
int main(int argc, char **argv) {
    

    auto [rubricFile, examFile, numTAs] = parse_args(argc, argv);
    
    SharedMemory memory;
    std::vector<TA> allTAs;

    memory.rubric = parseRubric(rubricFile);
    memory.exams = parseExams(examFile);
    allTAs = createTAs(numTAs);

    std::cout << "Number of TAs: " << numTAs << "\n\n";
    printTAs(allTAs);
    std::cout << "\n";
    std::cout << memory;

    return 0; 
}