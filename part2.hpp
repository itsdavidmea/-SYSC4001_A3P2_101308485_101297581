

#ifndef PART2_HPP
#define PART2_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <chrono>


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
        if (line.empty())
            continue;

        auto studentId = std::stoi(line);
        exams.emplace_back(studentId);
    }

    return exams;
}

//functions to print rubric item 
std::ostream& operator<<(std::ostream& os, const RubricItem& r)
{
    os << "RubricItem { number: " << r.number
       << ", text: \"" << r.text << "\" }";
    return os;
}

//functions to print operator 
std::ostream& operator<<(std::ostream& os, const Exam& e)
{
    os << "Exam { studentId: " << e.studentId << " }";
    return os;
}

//funtions to print shared memory 
std::ostream& operator<<(std::ostream& os, const SharedMemory& sm)
{
    os << "Exams:\n";
    for (const auto& exam : sm.exams)
        os << "  - " << exam << "\n";

    os << "Rubric:\n";
    for (const auto& item : sm.rubric)
        os << "  - " << item << "\n";

    return os;
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