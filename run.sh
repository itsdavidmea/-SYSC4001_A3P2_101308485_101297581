#!/bin/bash
# This script compiles and runs the program

# Compile both programs separately
echo "Compiling part2_a.cpp..."
g++ -std=c++17 -o part2_a part2_a.cpp

echo "Compiling teaching_assistant.cpp..."
g++ -std=c++17 -o teaching_assistant teaching_assistant.cpp

echo "Compiling reset_exams.cpp..."
g++ -std=c++17 -o reset_exams reset_exams.cpp

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running program..."
    echo "---"
    
    # Run the program with the provided arguments
    # If no arguments provided, use default files and 2 TAs
    defaultNumTAs=4
    if [ $# -eq 0 ]; then
        ./part2_a rubric exams/exam_0001.txt $defaultNumTAs
    else
        ./part2_a "$@"
    fi
else
    echo "Compilation failed!"
    exit 1
fi

# Student IDs: 101308485, 101297581