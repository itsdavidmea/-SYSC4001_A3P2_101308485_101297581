

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
    unsigned int number;
    std::string text;

    RubricItem(unsigned int n, const std::string &t)
        : number(n), text(t) {}
};

struct Exam
{
    int studentId;

    Exam(unsigned int id) : studentId(id) {}
};

struct SharedMemory
{
    std::vector<Exam> exams;
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

    return {rubric, exams};
}

std::vector<RubricItem> parseRubric(std::vector<std::string> lines)
{

    std::vector<RubricItem> rubric;
    for (const std::string &line : lines)
    {
        auto parts = split(line, ",");
        if (parts.size() < 2)
        {
            std::cerr << "Error: Malformed input line: " << line << std::endl;
            return {};
        }

        auto number = std::stoi(parts[0]);
        auto text = parts[1];
        rubric.emplace_back(number, text);
    }

    return rubric;
}

std::vector<Exam> parseExams(std::vector<std::string> lines)
{
    std::vector<Exam> exams;
    for (const std::string &line : lines)
    {
        auto parts = split(line, ",");
        if (parts.size() < 2)
        {
            std::cerr << "Error: Malformed input line: " << line << std::endl;
            return {};
        }

        auto studentId = std::stoi(parts[0]);
        exams.emplace_back(studentId);
    }

    return exams;
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

#endif