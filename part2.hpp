

#ifndef PART2_HPP
#define PART2_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <chrono>
#include <cstring>

#define RUBRIC_SIZE 256

// do the data structure for my memory

// struct that represents rubric

const char* SHM_NAME = "/my_sharedmemory";
struct TA {
    std::string name;
    
    TA(const std::string &n) 
    : name(n) {}
};

struct SharedMemory
{
    unsigned int exams;
    char rubric[RUBRIC_SIZE];
};

// returns the files rubric

/**
 * \brief parse the CLI arguments
 *
 * This helper function parses command line arguments and checks for errors
 *
 * @param argc number of command line arguments
 * @param argv the command line arguments
 * @return a vector of strings (the parsed vector table), a vector of delays, a vector of external files
 *
 */
std::tuple<std::vector<std::string>, std::vector<std::string>, int> parse_args(int argc, char **argv)
{

    if (argc != 4)
    {
        std::cout << "ERROR!\nExpected 3 arguments, received " << argc - 1 << std::endl;
        std::cout << "To run the program, do: ./part2 <rubric_file> <exam_file> <num_TAs>" << std::endl;
        exit(1);
    }

    std::ifstream file;
    file.open(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << argv[1] << std::endl;
        exit(1);
    }

    // processing the rubric file

    std::string line;
    std::vector<std::string> rubric;
    while (std::getline(file, line))
    {
        rubric.push_back(line);
    }
    file.close();

    file.open(argv[2]);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << argv[2] << std::endl;
        exit(1);
    }

    std::vector<std::string> exams;
    while (std::getline(file, line))
    {
        exams.push_back(line);
    }
    file.close();

    int numTAs = std::stoi(argv[3]);
    if (numTAs <= 0)
    {
        std::cerr << "Error: Number of TAs must be positive" << std::endl;
        exit(1);
    }

    return {rubric, exams, numTAs};
}

std::vector<std::string> split(std::string input, std::string delim)
{
    std::vector<std::string> tokens;
    std::size_t pos = 0;
    std::string token;
    while ((pos = input.find(delim)) != std::string::npos)
    {
        token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + delim.length());
    }
    tokens.push_back(input);

    return tokens;
}



// Convert rubric file lines to a char array
char* rubricToCharArray(const std::vector<std::string>& lines)
{
    // Allocate memory for the char array
    char* rubric = new char[RUBRIC_SIZE];
    rubric[0] = '\0';  // Initialize as empty string
    
    for (const std::string &line : lines)
    {
        // Check if adding this line would overflow the  
        if (strlen(rubric) + line.length() + 2 > RUBRIC_SIZE) // +2 for newline and null terminator
        {
            std::cerr << "Warning: Rubric data too large for buffer" << std::endl;
            break;
        }
        
        // Concatenate the line and a newline character
        
        strcat(rubric, line.c_str());
        strcat(rubric, "\n");
      
    }
    
    return rubric;
}

unsigned int parseExams(std::vector<std::string> lines)
{
    unsigned int exams;
    
    for (const std::string &line : lines)
    {
        if (line.empty())
            continue;

        auto studentId = std::stoi(line);
        exams = studentId;
    }

    return exams;
}

//create TAs list 
std::vector<TA> createTAs(int numTAs) {
    std::vector<TA> allTAs;
    for (int i = 0; i < numTAs; i++)
    {
        TA person("Teaching Assistant" + std::to_string(i + 1));
        allTAs.push_back(person);
    }
    
    return allTAs;
    
}



//function to print TA
std::ostream& operator<<(std::ostream& os, const TA& ta)
{
    os << "TA { name: \"" << ta.name << "\" }";
    return os;
}

//function to print vector of TAs
void printTAs(const std::vector<TA>& tas)
{
    std::cout << "TAs:\n";
    for (const auto& ta : tas)
        std::cout << "  - " << ta << "\n";
}




//create a random number generator with a given range of 2 numbers 
float randomNumGenerator(float low, float high) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(low, high);
    return dist(gen);
}


//create a delay function 
void delay(float seconds) {
    std::this_thread::sleep_for(std::chrono::duration<float>(seconds));

}




#endif