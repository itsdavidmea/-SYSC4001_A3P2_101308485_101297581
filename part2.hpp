

#ifndef PART2_HPP
#define PART2_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// do the data structure for my memory

// struct that represents rubric

struct RubricItem
{
    unsigned int exerciseNum;
    std::string exerciseText;

    RubricItem(unsigned int n, const std::string &t)
        : exerciseNum(n), exerciseText(t) {}
};

struct Exam
{
    std::string examContent;

    Exam(const std::string &c) : examContent(c) {}
};

struct SharedMemory
{
    std::vector<Exam> allExams;
    std::vector<RubricItem> rubric;
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
std::tuple<std::vector<std::string>, std::vector<std::string>> parse_args(int argc, char **argv)
{

    if (argc != 5)
    {
        std::cout << "ERROR!\nExpected 3 argument, received " << argc - 1 << std::endl;
        std::cout << "To run the program, do: ./part2 <rubric_file.txt> <exam_file.txt> " << std::endl;
        exit(1);
    }

    std::ifstream input_file;
    input_file.open(argv[1]);
    if (!input_file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << argv[1] << std::endl;
        exit(1);
    }

    // processing the rubric file

    std::string rubricLine;
    std::vector<std::string> rubric;
    while (std::getline(input_file, rubricLine))
    {
        rubric.push_back(rubricLine);
    }
    input_file.close();

    input_file.open(argv[2]);
    if (!input_file.is_open())
    {
        std::cerr << "Error: Unable to open file: " << argv[2] << std::endl;
        exit(1);
    }

    std::string examLine;
    std::vector<std::string> exams;
    while (std::getline(input_file, examLine))
    {
        exams.push_back(examLine);
    }
    input_file.close();

    return {rubric, exams};
}

std::vector<RubricItem> createRubricItem(std::vector<std::string> rubricFile)
{

    std::vector<RubricItem> finalRubric; 
    for (const std::string &item : rubricFile)
    {
        auto parts = split_delim(item, ",");
        if (parts.size() < 2)
        {
            std::cerr << "Error: Malformed input line: " << item << std::endl;
            return {};
        }

        auto exerciseNum = std::stoi(parts[0]);

        auto exerciseText = parts[1];
        RubricItem rubricLine(exerciseNum, exerciseText);
        finalRubric.push_back(rubricLine);

        
    }

    return finalRubric;
}

void addExamToMemory()
{
}

std::vector<std::string> split_delim(std::string input, std::string delim)
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

#endif