#!/bin/bash
# This script compiles and runs the program

# Compile both programs separately
echo "Compiling part2_a.cpp..."
g++ -std=c++17 -o part2_a part2_a.cpp

echo "Compiling teaching_assistant.cpp..."
g++ -std=c++17 -o teaching_assistant teaching_assistant.cpp



# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running program..."
    echo "---"
    
    # Run the program with the provided arguments
    # If no arguments provided, use default files and 2 TAs
    defaultNumTAs=2
    if [ $# -eq 0 ]; then
        ./part2_a rubric exams/exam_0001.txt $defaultNumTAs
    else
        ./part2_a "$@"
    fi
else
    echo "Compilation failed!"
    exit 1
fi
