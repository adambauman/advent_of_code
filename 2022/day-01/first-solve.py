#!/usr/bin/env python

import sys
import array

"""Solve Advent of Code Day 1, Puzzle 1"""

__author__ = "Adam J. Bauman"

def main():

    elf_totals = array.array('I')

    with open("input.txt", "r") as file:
        calories = 0
        for line in file:
            
            # Strip whitespace, returns false if an empty line.
            if line.strip(): 

                calories += int(line)
            else:
                
                elf_totals.append(calories)
                calories = 0

                continue
    
    elf_totals = sorted(elf_totals)
    print(elf_totals)

if __name__ == '__main__':
    sys.exit(main())