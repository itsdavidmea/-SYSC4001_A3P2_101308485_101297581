# SYSC4001 Assignment 3 Part 2 - Multi-Process Exam Grading System

## Student Information
- Student IDs: 101308485, 101297581
- Course: SYSC4001
- Assignment: Part 2 - Shared Memory and Semaphores

## Program Description
This program simulates a multi-process exam grading system where multiple Teaching Assistants (TAs) concurrently grade exams and update a shared rubric. The program demonstrates:
- **POSIX Shared Memory** for inter-process communication
- **Semaphores** for synchronization (separate mutexes for exam files and rubric)
- **Fork/Exec** for creating TA processes
- **Concurrent file I/O** with proper synchronization

## System Architecture
- **Producer (part2_a)**: Creates shared memory, initializes data, spawns TA processes, and manages cleanup
- **Consumers (teaching_assistant)**: TA processes that read from shared memory, grade exams, and update the rubric
- **Shared Resources**: 
  - Exam file path (protected by `exam_mutex`)
  - Rubric data (protected by `rubric_mutex`)

## File Structure
```
.
├── part2_a.cpp              # Main producer program
├── teaching_assistant.cpp   # TA consumer program
├── part2.hpp                # Shared header with data structures
├── reset_exams.cpp          # Utility to reset exam and rubric files
├── run.sh                   # Build and run script
├── rubric                   # Rubric file (5 items)
├── exams/                   # Directory containing 20 exam files
│   ├── exam_0001.txt        # Student ID: 0001
│   ├── exam_0002.txt        # Student ID: 0002
│   └── ...
│   └── exam_0020.txt        # Termination file (contains "9999")
└── README.md                # This file
```

## Compilation

### Using the Build Script (Recommended)
```bash
./run.sh
```

### Manual Compilation
```bash
# Compile the producer
g++ -std=c++17 -o part2_a part2_a.cpp

# Compile the TA consumer
g++ -std=c++17 -o teaching_assistant teaching_assistant.cpp

# Compile the reset utility
g++ -std=c++17 -o reset_exams reset_exams.cpp
```

### Compilation Requirements
- C++17 standard
- POSIX-compliant system (Linux/Unix)
- Required libraries: pthread, rt (for semaphores and shared memory)

## Running the Program

### Basic Usage
```bash
./part2_a <rubric_file> <exam_file> <num_TAs>
```

**Arguments:**
- `rubric_file`: Path to the rubric file (default: `rubric`)
- `exam_file`: Path to the first exam file (default: `exams/exam_0001.txt`)
- `num_TAs`: Number of TA processes to spawn (default: 2)

### Example Test Cases

#### Test Case 1: Two TAs (Default)
```bash
./part2_a rubric exams/exam_0001.txt 2
```
**Expected Behavior:**
- 2 TA processes are created
- Each TA picks up exams sequentially from shared memory
- TAs concurrently review and update the rubric
- Both TAs stop when they encounter exam_0020.txt (containing "9999")

#### Test Case 2: Single TA
```bash
./part2_a rubric exams/exam_0001.txt 1
```
**Expected Behavior:**
- 1 TA processes all exams sequentially
- No contention for shared resources
- Stops at exam_0020.txt

#### Test Case 3: Multiple TAs (High Concurrency)
```bash
./part2_a rubric exams/exam_0001.txt 4
```
**Expected Behavior:**
- 4 TAs compete for exams
- Higher likelihood of rubric conflicts
- Demonstrates proper semaphore synchronization
- All TAs terminate when one encounters "9999"

#### Test Case 4: Using the Run Script
```bash
# Uses default parameters (2 TAs)
./run.sh
```

## Resetting Test Files

Before running new tests, reset the exam and rubric files to their initial state:

```bash
./reset_exams
```

**What it does:**
- Resets all 20 exam files to contain only their student IDs (0001-0019)
- Sets exam_0020.txt to contain "9999" (termination signal)
- Resets rubric file to original values (1,A / 2,B / 3,C / 4,D / 5,E)
- Removes all "graded by: TA#" annotations

## Program Output

### Output Format
The program provides detailed output showing:
- **Critical Section Entry/Exit**: Clearly marked with `>>> ENTERING/EXITING CRITICAL SECTION <<<`
- **Shared Memory Operations**: Explicit statements about reading/writing to shared memory
- **TA Activities**: Processing exams, reviewing rubric items, marking exams
- **Synchronization**: Which semaphore (exam_mutex or rubric_mutex) is being used
- **Termination**: Clear indication when "9999" is detected

### Sample Output
```
[TA1] >>> ENTERING CRITICAL SECTION (exam_mutex) <<<
[TA1] Reading current exam path from shared memory...
[TA1] Incrementing exam path in shared memory...
[TA1] <<< EXITING CRITICAL SECTION (exam_mutex) >>>

========================================
[TA1] Processing exam_0001.txt (Student ID: 0001)
========================================
[TA1] Reviewing rubric items...
  [TA1] Rubric Item #1 - Answer: 'A'
  [TA1] ❌ INCORRECT - Current answer 'A' needs correction
  [TA1] >>> ENTERING CRITICAL SECTION (rubric_mutex) <<<
  [TA1] Writing to shared rubric: incrementing value at position 3
  [TA1] <<< EXITING CRITICAL SECTION (rubric_mutex) >>>
```

## Termination Conditions

The program terminates when:
1. Any TA encounters a file with content "9999" (exam_0020.txt by default)
2. A TA cannot open the next exam file
3. All TAs complete their work and exit

## Important Notes

### Critical Sections
- **exam_mutex**: Protects access to the shared exam file path
- **rubric_mutex**: Protects access to the shared rubric data
- Delays and file I/O operations occur OUTSIDE critical sections for better concurrency

### File Format
- **Exam files**: First line contains 4-digit student ID, subsequent lines added by TAs
- **Rubric file**: Format is "number, letter" (e.g., "1, A")
- **Termination file**: exam_0020.txt contains "9999"

### Synchronization
- Each TA gets a unique exam file (no race condition for individual files)
- Rubric modifications are synchronized via `rubric_mutex`
- Exam path updates are synchronized via `exam_mutex`

## Troubleshooting

### Issue: "shm_open failed"
**Solution**: Make sure no previous shared memory segment exists
```bash
rm /dev/shm/my_sharedmemory
```

### Issue: Programs don't compile
**Solution**: Ensure you have C++17 support and required libraries
```bash
g++ --version  # Check GCC version (should be 7.0 or higher)
```

### Issue: Deadlock or TAs not stopping
**Solution**: Reset and recompile
```bash
./reset_exams
make clean  # If using Makefile
./run.sh
```

### Issue: Permission denied on scripts
**Solution**: Make scripts executable
```bash
chmod +x run.sh reset_exams
```

## Testing Strategy

1. **Reset environment**: `./reset_exams`
2. **Run test case**: `./part2_a rubric exams/exam_0001.txt <num_TAs>`
3. **Observe output**: Check for proper critical section documentation
4. **Verify results**: 
   - Check exam files for "graded by: TA#" annotations
   - Check rubric file for modified values
5. **Repeat**: Reset and test with different numbers of TAs

## Expected Behavior by Test Case

| Test Case | # TAs | Expected Exams Processed | Concurrency Level |
|-----------|-------|-------------------------|-------------------|
| 1         | 1     | Up to 19 (stops at 20)  | None (sequential) |
| 2         | 2     | Up to 19 (distributed)  | Moderate          |
| 3         | 4     | Up to 19 (distributed)  | High              |
| 4         | 8     | Up to 19 (distributed)  | Very High         |

## Known Limitations

- Maximum 20 exam files in current setup
- Rubric size limited to 256 characters
- Exam file path limited to 256 characters
- TAs process exams sequentially from shared counter (not truly parallel exam selection)

## Additional Resources

- POSIX Shared Memory: `man shm_open`
- Semaphores: `man sem_init`
- Process Management: `man fork`, `man exec`

## Contact

For questions or issues, contact:
- Student 1: 101308485
- Student 2: 101297581
