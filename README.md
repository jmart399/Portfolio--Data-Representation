# CS 3503 Assignment 1 - Number Base Conversion

 ## Author
Jacob Martinez

## Description
My implementation of Owl Tech’s number base conversion utility. This program utilizes division and subtraction base conversion methods, and can display the most common base representations of a given number. 

## What ’s New
 - Direct mapping functions ( oct / hex / bin )
 - Signed number representations
 - Reused test framework from A1

## Build Instructions
‘‘‘bash
gcc -o convert convert .c main .c
./ convert
‘‘‘

## Test Results
49/51 tests passed. 

## Notes (1st test)
My biggest issue was dealing with the array sizes at larger numbers accross different bases. There are a few brute force and patchwork solutions that certainly lack efficiency. As a thought experiment I attempted to find effecient array sizes for each base, but I ran into issues with edge cases. Still, that led me to discover that sizeof() is an excellent go-to.

I also was pleasantly surprised with how user-firendly it was to learn and code in C given its close relationship to the hardware. Perhaps that is a false comparison, but either way it was mostly intuitive.

## Notes (2nd test)
The math for these methods is deceptively easy relative to how difficult the actual coding was. In particular, dealing with the padding in oct_to_hex required a fair amount of trial and error as well as some alternative guide videos, and I'm still not 100% convinced it's fully correct, and certainly not optimal. hex/oct_to_bin were both much easier, and reusing the file reading system from assignment 1 helped a lot.
