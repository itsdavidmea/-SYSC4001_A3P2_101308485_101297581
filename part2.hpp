#include <string>

#ifndef PART2_HPP
#define PART2_HPP




// do the data structure for my memory

//struct that represents rubric 

#include <string>
#include <vector>

struct RubricItem
{
    unsigned int exerciseNum;
    std::string exerciseText;

    RubricItem(unsigned int n, const std::string& t)
        : exerciseNum(n), exerciseText(t) {}
};

struct Exam
{
    std::string examContent;

    Exam(const std::string& c) : examContent(c) {}
};

struct SharedMemory
{
    std::vector<Exam> allExams;
    std::vector<RubricItem> rubric;
};



#endif