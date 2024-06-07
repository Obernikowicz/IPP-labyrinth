#!/bin/bash

COLOR_GREEN="\e[32m"
COLOR_RED="\e[31m"
COLOR_YELLOW="\e[33m"
COLOR_RESET="\e[0m"

if [ $# -lt 2 ]
then
    echo -e "${COLOR_RED}Error!${COLOR_RESET} Too few arguments!"
    exit 0
fi

# CORRECTNESS TESTS

echo -e "Starting correctness tests...\n"

FAILED_TESTS_NUMBER=0
PASSED_TESTS_NUMBER=0

time for f in "$2"/*.in
do
    echo -n "Starting test for $f..."
    ./"${1}" < "$f" > output.out 2> output.err

    TEST_FAILED=0
    if ! diff output.out ${f%in}out &>/dev/null
    then
        echo -n -e " ${COLOR_RED}Invalid output!${COLOR_RESET}"
        TEST_FAILED=1
    fi

    if ! diff output.err ${f%in}err &>/dev/null
    then
        echo -n -e " ${COLOR_RED}Invalid error!${COLOR_RESET}"
        TEST_FAILED=1
    fi

    if [ $TEST_FAILED -eq 0 ]
    then
        echo -n -e " ${COLOR_GREEN}Passed!${COLOR_RESET}"
        PASSED_TESTS_NUMBER=$((PASSED_TESTS_NUMBER+1))
    else
        FAILED_TESTS_NUMBER=$((FAILED_TESTS_NUMBER+1))
    fi

    echo -n -e "\n"

    rm output.out
    rm output.err

done

TOTAL_TESTS_NUMBER=$((PASSED_TESTS_NUMBER+FAILED_TESTS_NUMBER))

echo -e "\nSummary:"

if [ $FAILED_TESTS_NUMBER -eq 0 ]
then
    echo -e "Passed tests: ${COLOR_GREEN}${PASSED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
    echo -e "Failed tests: ${COLOR_GREEN}${FAILED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
else
    echo -e "Passed tests: ${COLOR_RED}${PASSED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
    echo -e "Failed tests: ${COLOR_RED}${FAILED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
fi

# MEMORY TESTS

echo -e "\nStarting memory tests...\n"

FAILED_TESTS_NUMBER=0
PASSED_TESTS_NUMBER=0

time for f in "$2"/*.in
do
    echo -n "Starting test for $f..."
    valgrind --log-file=valgrind.log --error-exitcode=123 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all ./"${1}" < "$f" > /dev/null 2>&1
    
    if [ $? -eq 123 ]
    then
        echo -e " ${COLOR_RED}Failed!${COLOR_YELLOW}"
        cat valgrind.log
        echo -n -e "${COLOR_RESET}"
        FAILED_TESTS_NUMBER=$((FAILED_TESTS_NUMBER+1))
    else 
        echo -e " ${COLOR_GREEN}Passed!${COLOR_RESET}"
        PASSED_TESTS_NUMBER=$((PASSED_TESTS_NUMBER+1))
    fi

    rm valgrind.log
done

TOTAL_TESTS_NUMBER=$((PASSED_TESTS_NUMBER+FAILED_TESTS_NUMBER))

echo -e "\nSummary:"

if [ $FAILED_TESTS_NUMBER -eq 0 ]
then
    echo -e "Passed tests: ${COLOR_GREEN}${PASSED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
    echo -e "Failed tests: ${COLOR_GREEN}${FAILED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
else
    echo -e "Passed tests: ${COLOR_RED}${PASSED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
    echo -e "Failed tests: ${COLOR_RED}${FAILED_TESTS_NUMBER}/${TOTAL_TESTS_NUMBER}${COLOR_RESET}"
fi