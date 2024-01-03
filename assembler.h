// Name: Phubeth Mettaprasert
// Version: 1.0
// File: assembler.h
// Date: 23/11/2022
// A file that contains assembler code. This code will read assembly language
// instruction and translate into machine language

#ifndef REDCODE_ASSEMBLER_H
#define REDCODE_ASSEMBLER_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>


using namespace std;

//This is the array for the main memory
static unsigned int core[1024];

//A command list map
std::unordered_map<string, int> command_list = {
        {"MOV", 1},
        {"ADD", 2},
        {"SUB", 3},
        {"JMP", 4},
        {"JMZ", 5},
        {"JMG", 6},
        {"DJZ", 7},
        {"CMP", 8},
        {"SPL", 9},
        {"DAT", 0},
};



/**
 * The main assemble function. Calls other functions to assemble the line of
 * assembly  language.
 * @param line The line of assembly language that needs to be assembled
 * @param int User number
 * @return The line translated to machine language.
 */
int assemble(string, int);

/**
 * This will take in a filename, what user is playing, and the base address
 * of where to put the machine instructions in the main memory. Keeps
 * incrementing down the memory. This function will call other functions in
 * order to assemble the instructions.
 */
void load_program(string, int, int);

/**
 * A method to print out the machine code in memory. This is copied verbatim
 * from the homework assignment.
 * @param start The starting block of where the memory is to be printed
 * @param end The end of the block
 */
void dump_memory(int, int);

/**
 * A function called by the assemble function to split the string and store
 * it as a vector. Splits the string by delemiter and also checks that if
 * anything is ; or anything after (the comments portion) to just stop the
 * operation.
 * @param: stringToBeSplit The string that will be split
 * @param: delim The delimiter to split the string by
 * @param: out The vector that will contain the split string.
 */
void splitString(const string &, const char, vector<string> &);

/**
 * A helper function that will return the mode of addressing base on the
 * string operand.
 * @param operand The string that will be translated to an int mode
 * @return The int mode that will be returned.
 */
int returnMode(string operand);

/**
 * A substring without the mode in front (returns the number for the actual
 * operand).
 * @param operand The original operand with the mode
 * @param mode The mode that that was calculated from the return mode function.
 * @return The operand numbers without the mode in front.
 */
int returnIntSubStringWithoutMode(string, int);



void load_program(string filename, int user, int base_addr) {

    //Reads the file
    ifstream infile(filename);

    //For each line of the file assemble it.
    for (string line; getline(infile, line);) {
        int instr = assemble(line, user);

        //Store it in the core address.
        core[base_addr++] = instr;
    }
}


int assemble(string line, int user) {

    //Create the vector to store the split string
    vector<string> vectorLine;

    //Split it by the delimiter of space
    const char delim = ' ';
    splitString(line, delim, vectorLine);


    //Beginning of the assembling part
    unsigned int instr = 0;
    int opcode = command_list[vectorLine[0]];
    opcode = opcode << 26;
    instr = instr | opcode;


    //This for those without operand A
    if(vectorLine[0] != "DAT") {

        //Figure out the mode and store it
        int modeNum = returnMode(vectorLine[1]);
        int modeA = modeNum << 23;
        instr = instr | modeA;

        //Return the operand without the mode (just the number argument)
        int opA = returnIntSubStringWithoutMode(vectorLine[1], modeNum);

        //For negative cases need to make it to ten bits first before shifting
        if(opA < 0) {
            opA = opA & 0x3FF;
        }

        //Store the operand.
        opA = opA << 13;
        instr = instr | opA;

    }


    //Same principle for operand B
    if(vectorLine[0] != "SPL" && vectorLine[0] != "JMP") {
        if(vectorLine[0] == "DAT") {

            //Operand B with DAT has a zero as the address mode so just store
            // the operand itself
            int opB = returnIntSubStringWithoutMode(vectorLine[1], 0);

            //For negative cases
            if(opB < 0) {
                opB = opB & 0x3FF;
            }

            //Storing operand B
            opB = opB << 0;
            instr = instr | opB;
        } else {

            //Same principle as storing operand A
            int modeNum = returnMode(vectorLine[2]);
            int modeB = modeNum << 10;
            instr = instr | modeB;
            int opB = returnIntSubStringWithoutMode(vectorLine[2], modeNum);
            //For negative cases
            if(opB < 0) {
                opB = opB & 0x3FF;
            }

            opB = opB << 0;
            instr = instr | opB;
        }



    }

    //Store the user's number in the instruction
    int player = user << 30;
    instr = instr | player;


    //Return the instruction
    return instr;

}

//Print out the memory
void dump_memory(int start, int len) {
    for (int i = start; i < start + len; i++) {
        printf("%03x: 0x%08x\n", i, core[i]);
    }
}



void splitString(const string &stringToBeSplit, const char delim,
                 vector<string> & out) {


    stringstream ss(stringToBeSplit);
    string intermediateString;
    while(getline(ss, intermediateString, delim)) {

        //Immediately stops the operation if there is a semicolon so that the
        // comments aren't read.
        if(intermediateString == ";") {
            return;
        }

        //Storing the splitted string in the vector
        out.push_back(intermediateString);
    }
}

int returnMode(string operand) {

    //npos means until the end of the string, returns 2 if there is @ in the
    // operand
    if(operand.find('@') != string::npos) {
        return 2;

    } else if (operand.find('#') != string::npos) {

        //returns zero if # is found in the operand
        return 0;
    }

    //Default mode to return
    return 1;
}

int returnIntSubStringWithoutMode(string operand, int mode) {

    //Truncates the string of the operand of the mode and returns the pure
    // operand (just the number without the mode)
    int returnInt = 0;
    int pos;
    string sub;
    switch(mode) {
        case 1:
            returnInt = stoi(operand);
            break;
        case 2:
            pos = operand.find('@');
            sub = operand.substr(pos + 1);
            returnInt = stoi(sub);
            break;
        case 0:
            pos = operand.find('#');
            sub = operand.substr(pos + 1);
            returnInt = stoi(sub);
            break;
        default:
            break;
    }

    return returnInt;
}


#endif //REDCODE_ASSEMBLER_H
