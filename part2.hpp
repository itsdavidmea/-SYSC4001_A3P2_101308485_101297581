#include <string>

#ifndef PART2_HPP
#define PART2_HPP




// do the data structure for my memory

//struct that represents rubric 

struct rubricItem
{
    unsigned int exerciseNum;
    std::string exerciseText;

    rubricItem(unsigned int _eN, std::string _eT): 
        exerciseNum(_eN), exerciseText(_eT) {}


};

rubricItem rubric[] = {};

//in the memory, have rubric in one side and exam in the other, will use struct for this 

struct sharedMemory
{
    //exam number 

    //rubric 
};


#endif