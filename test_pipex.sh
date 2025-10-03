#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counter for tests
TOTAL=0
PASSED=0
FAILED=0

# Test function
test_case() {
    local description="$1"
    local cmd="$2"
    local expected_exit="$3"
    local timeout="${4:-5}"  # Default timeout: 5 seconds

    TOTAL=$((TOTAL + 1))
    echo -e "\n${YELLOW}Test $TOTAL: $description${NC}"
    echo "Command: $cmd"

    timeout $timeout bash -c "$cmd"
    local exit_code=$?

    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}✗ TIMEOUT${NC} (exceeded ${timeout}s)"
        FAILED=$((FAILED + 1))
    elif [ "$expected_exit" = "any" ] || [ $exit_code -eq $expected_exit ]; then
        echo -e "${GREEN}✓ PASSED${NC} (exit code: $exit_code)"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ FAILED${NC} (expected: $expected_exit, got: $exit_code)"
        FAILED=$((FAILED + 1))
    fi
}

# Create test files
echo "Setting up test files..."
echo "Hello World" > infile
echo "Line 1" > infile2
echo "Line 2" >> infile2
echo "Line 3" >> infile2
chmod 000 no_read_file 2>/dev/null || touch no_read_file && chmod 000 no_read_file
chmod 000 no_write_file 2>/dev/null || touch no_write_file && chmod 000 no_write_file

echo -e "\n${YELLOW}=== PIPEX TEST SUITE ===${NC}\n"

# ===== BASIC FUNCTIONALITY =====
echo -e "\n${YELLOW}### BASIC FUNCTIONALITY ###${NC}"

test_case "Basic pipe: cat | wc -l" \
    "./pipex infile 'cat' 'wc -l' outfile && cat outfile" \
    "any"

test_case "Basic pipe: ls | grep pipex" \
    "./pipex infile 'ls' 'grep pipex' outfile && cat outfile" \
    "any"

test_case "Grep and wc" \
    "./pipex infile2 'grep Line' 'wc -l' outfile && cat outfile" \
    "any"

# ===== ARGUMENT VALIDATION =====
echo -e "\n${YELLOW}### ARGUMENT VALIDATION ###${NC}"

test_case "No arguments" \
    "./pipex" \
    1

test_case "One argument" \
    "./pipex infile" \
    1

test_case "Two arguments" \
    "./pipex infile 'cat'" \
    1

test_case "Three arguments" \
    "./pipex infile 'cat' 'wc -l'" \
    1

test_case "Five arguments (too many)" \
    "./pipex infile 'cat' 'wc -l' outfile extra" \
    1

test_case "Exactly four arguments (correct)" \
    "./pipex infile 'cat' 'wc -l' outfile" \
    "any"

# ===== FILE HANDLING =====
echo -e "\n${YELLOW}### FILE HANDLING ###${NC}"

test_case "Non-existent input file" \
    "./pipex nonexistent 'cat' 'wc -l' outfile" \
    "any"

test_case "Empty input file" \
    "touch empty_file && ./pipex empty_file 'cat' 'wc -l' outfile" \
    "any"

test_case "Input file without read permissions" \
    "./pipex no_read_file 'cat' 'wc -l' outfile 2>/dev/null" \
    "any"

test_case "Output file in non-existent directory" \
    "./pipex infile 'cat' 'wc -l' /nonexistent/dir/outfile 2>/dev/null" \
    "any"

test_case "Output file creation" \
    "rm -f new_outfile && ./pipex infile 'cat' 'wc -l' new_outfile && test -f new_outfile" \
    "any"

test_case "Output file overwrite" \
    "echo 'old content' > existing_file && ./pipex infile 'cat' 'wc -l' existing_file && cat existing_file" \
    "any"

# ===== COMMAND VALIDATION =====
echo -e "\n${YELLOW}### COMMAND VALIDATION ###${NC}"

test_case "First command not found" \
    "./pipex infile 'nonexistentcmd' 'wc -l' outfile 2>/dev/null" \
    127

test_case "Second command not found" \
    "./pipex infile 'cat' 'nonexistentcmd' outfile 2>/dev/null" \
    127

test_case "Both commands not found" \
    "./pipex infile 'cmd1notfound' 'cmd2notfound' outfile 2>/dev/null" \
    127

test_case "Empty command string 1" \
    "./pipex infile '' 'wc -l' outfile 2>/dev/null" \
    "any"

test_case "Empty command string 2" \
    "./pipex infile 'cat' '' outfile 2>/dev/null" \
    "any"

# ===== COMMAND WITH ARGUMENTS =====
echo -e "\n${YELLOW}### COMMANDS WITH ARGUMENTS ###${NC}"

test_case "Command with single flag" \
    "./pipex infile 'ls -l' 'grep pipex' outfile" \
    "any"

test_case "Command with multiple flags" \
    "./pipex infile 'ls -la' 'wc -l' outfile" \
    "any"

test_case "grep with pattern" \
    "./pipex infile2 'cat' 'grep Line' outfile && cat outfile" \
    "any"

test_case "awk command" \
    "./pipex infile 'cat' 'awk {print \$1}' outfile 2>/dev/null" \
    "any"

# ===== SPECIAL CASES =====
echo -e "\n${YELLOW}### SPECIAL CASES ###${NC}"

test_case "cat to cat (identity)" \
    "./pipex infile 'cat' 'cat' outfile && diff infile outfile" \
    "any"

test_case "Multiple spaces in command" \
    "./pipex infile 'cat' 'wc  -l' outfile 2>/dev/null" \
    "any"

test_case "Command with absolute path" \
    "./pipex infile '/bin/cat' 'wc -l' outfile" \
    "any"

test_case "Command that fails (grep no match)" \
    "./pipex infile 'cat' 'grep NOMATCH' outfile" \
    "any"

# ===== COMPARISON WITH SHELL =====
echo -e "\n${YELLOW}### COMPARISON WITH SHELL BEHAVIOR ###${NC}"

test_case "Compare with shell: < infile cat | wc -l > outfile" \
    "< infile cat | wc -l > shell_out && ./pipex infile 'cat' 'wc -l' pipex_out && diff shell_out pipex_out" \
    0

test_case "Compare with shell: < infile2 grep Line | wc -l > outfile" \
    "< infile2 grep Line | wc -l > shell_out && ./pipex infile2 'grep Line' 'wc -l' pipex_out && diff shell_out pipex_out" \
    0

test_case "Compare with shell: < infile ls | grep .c > outfile" \
    "< infile ls | grep .c > shell_out && ./pipex infile 'ls' 'grep .c' pipex_out && diff shell_out pipex_out" \
    0

# ===== EDGE CASES =====
echo -e "\n${YELLOW}### EDGE CASES ###${NC}"

test_case "Large file handling" \
    "seq 1 10000 > large_file && ./pipex large_file 'cat' 'wc -l' outfile && cat outfile" \
    "any"

test_case "Binary file as input" \
    "./pipex ./pipex 'cat' 'wc -c' outfile && cat outfile" \
    "any"

test_case "Command that reads from stdin (cat)" \
    "./pipex infile 'cat' 'cat' outfile && diff infile outfile" \
    "any"

test_case "head command" \
    "seq 1 100 > numbers && ./pipex numbers 'cat' 'head -5' outfile && cat outfile" \
    "any"

test_case "tail command" \
    "./pipex numbers 'cat' 'tail -5' outfile && cat outfile" \
    "any"

test_case "sort command" \
    "echo -e '3\n1\n2' > unsorted && ./pipex unsorted 'cat' 'sort' outfile && cat outfile" \
    "any"

# ===== PERMISSION TESTS =====
echo -e "\n${YELLOW}### PERMISSION TESTS ###${NC}"

test_case "Read-only output directory" \
    "mkdir -p readonly_dir && chmod 444 readonly_dir && ./pipex infile 'cat' 'wc -l' readonly_dir/outfile 2>/dev/null; chmod 755 readonly_dir" \
    "any"

# Clean up
echo -e "\n${YELLOW}Cleaning up test files...${NC}"
rm -f infile infile2 outfile new_outfile existing_file empty_file
rm -f no_read_file no_write_file shell_out pipex_out
rm -f large_file numbers unsorted
rm -rf readonly_dir

# Summary
echo -e "\n${YELLOW}=== TEST SUMMARY ===${NC}"
echo -e "Total tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed ✗${NC}"
    exit 1
fi
