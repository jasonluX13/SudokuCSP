# SudokuCSP

## About
This is a sudoku puzzle solver that treats the puzzle as a constraint satisfaction problem. The program stores the board as a
three dimensional vector with the first two dimensions representing the rows and columns and the last dimension represents the constraint list for each tile on the board. The program first uses foward checking to eleminate constraints based on tiles that are already filled in. Once foward checking completes, backtracking search is used to solve the remainder of the puzzle.

## Instructions
To compile the program, run the following command:  
g++ -std=c++11 sudoku.cpp   
To run the program on an input file, use the following command:   
./a.out \<inputfile> \<outputfile>  
\<inputfile> - name of input file  
\<outputfile> - what you want to name the output file  
Example Usage:  
./a.out SUDUKO_Input1.txt Output1.txt  
./a.out SUDUKO_Input2.txt Output2.txt  
./a.out SUDUKO_Input3.txt Output3.txt    

Several test files have been provided
