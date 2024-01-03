// Name: Phubeth Mettaprasert
// Version: 1.0
// File: main.cpp
// Date: 23/11/2022
// A file to test the assembler and the disassembler.

#include <iostream>
#include "assembler.h"
#include "disassembler.h"

using namespace std;


//THE CORE MEMORY IS LOCATED AT THE TOP OF THE ASSEMBLER.H
//I have my files at in the cmake-build-debug folder. Wasn't sure how to hand
// it in so I have it as zip file with all the files and t



int main() {

    //Assembling portion
    cout << "Assembling Portion: \n\n";

    //Load the program nulls.rc starting from zero in the memory block
    //My nulls.rc and gemini.rc is located in the cmake-build-debug
    load_program("nulls.rc", 1, 0);

    //Load the program gemini.rc starting from 512 in the memory block
    load_program("gemini.rc", 2, 512);

    //Print out the memory
    printf("Player 1:\n");
    dump_memory(0, 10);
    printf("\nPlayer 2:\n");
    dump_memory(512, 10);


    //Disassembling Part for the second part of the hwk
    cout << "\n\nDisassembling Portion: \n\n";
    disassembleFromBlock(0, 10);
    disassembleFromBlock(512, 10);


    return 0;
}


