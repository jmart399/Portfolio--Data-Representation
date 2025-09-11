# CS 3503 Assignment 1 - Number Base Conversion

 ## Author
Jacob Martinez

## Description
My implementation of Owl Tech’s number base conversion utility. This program utilizes division and subtraction base conversion methods, and can display the most common base representations of a given number. 

## Build Instructions
‘‘‘bash
gcc -o convert convert .c main .c
./ convert
‘‘‘

## Test Results
147/147 tests passed

## Notes
My biggest issue was dealing with the array sizes at larger numbers accross different bases. There are a few brute force and patchwork solutions that certainly lack efficiency. As a thought experiment I attempted to find effecient array sizes for each base, but I ran into issues with edge cases. Still, that led me to discover that sizeof() is an excellent go-to.

I also was pleasantly surprised with how user-firendly it was to learn and code in C given its close relationship to the hardware. Perhaps that is a false comparison, but either way it was mostly intuitive.
