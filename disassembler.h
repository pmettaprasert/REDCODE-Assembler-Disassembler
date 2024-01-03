// Name: Phubeth Mettaprasert
// Version: 1.0
// File: disassembler.h
// Date: 23/11/2022
// A file that contains disassembler code. This will translate the machine
// code instruction back to string. This will tell what player it is as well.


#ifndef REDCODE_DISASSEMBLER_H
#define REDCODE_DISASSEMBLER_H

//A map that contains the direct translation for op codes
std::unordered_map<int, string> opcodes2{
        {1, "MOV"},
        {2, "ADD"},
        {3, "SUB"},
        {4, "JMP"},
        {5, "JMZ"},
        {6, "JMG"},
        {7, "DJZ"},
        {8, "CMP"},
        {9, "SPL"},
        {0, "DAT"},
};

//A map that contains the direct translation for address mode for A and B
std::unordered_map<int, string> addrMode2 {
        {0, "#"},
        {1, ""},
        {2,"@"},
};


/**
 * A function that will returns the disassembled string. Calls other functions
 * @param instrNum The machine code that is going to be disassembled
 * @return The string in assembly language.
 */
string disassemble(int instrNum);

/**
 * A function to get the operation code from the machine code
 * @param instrNum The machine code instructions
 * @return A string of the operation code
 */
string getOp(int instrNum);

/**
 * Gets the mode address for A
 * @param instrNum The machine code instructions
 * @return A string of the mode for A
 */
string getModeA(int instrNum);

/**
 * Gets the mode address for B
 * @param instrNum The machine code instructions
 * @return A string of the mode for B
 */
string getModeB(int instrNum);

/**
 * Gets the operand number
 * @param instrNum The machine code instructions
 * @param mode Which operand it is for A or B
 * @return A string of the operand number
 */
string getOperand(int instrNum, char mode);

/**
 * Gets the player number
 * @param instrNum The machine code instructions
 * @return A string of the player
 */
string getPlayer(int instrNum);


/**
 * The main function call in order to disassemble the block of memory.
 * @param start the starting memory address
 * @param len How big the block of memory is.
 */
void disassembleFromBlock(int start, int len);

void disassembleFromBlock(int start, int len) {
    printf("\nDisassembling from block %03x to block %03x\n\n", start, start +
                                                                       len -1);
    //Print out the disassembled machine code from the memory.
    for(int i = start; i < start + len; i++) {
        printf("%03x: ", i);
        cout << disassemble(core[i]) << "... Instruction In Memory: ";
        printf("0x%08x\n", core[i]);

    }
}

string disassemble(int instrNum) {

    //Zero check for no instructions
    if(instrNum == 0) {
        return "No instructions      ;";
    }

    //Returns the stringstream when done
    stringstream ss;

    //Gets the operation code
    string opCode = getOp(instrNum);

    ss << opCode << " ";
    string addrModeA;
    string addrModeB;

    //Conditions for operand A
    if(opCode != "DAT") {
        addrModeA = getModeA(instrNum);
        ss << addrModeA;
        ss << getOperand(instrNum, 'A') << " ";

    }

    //Conditions for operand B
    if(opCode != "JMP" && opCode != "SPL") {

        //Special section just for the DAT command
        if(opCode != "DAT") {
            addrModeB = getModeB(instrNum);
            ss << addrModeB;

        }


        ss << getOperand(instrNum, 'B') << " ";

    }
    string player = getPlayer(instrNum);
    ss << "      ; " << player;

    return ss.str();
}

string getOp(int instrNum) {
    int opCode =  (instrNum >> 26) & 0xF;
    return opcodes2[opCode];
}

string getModeA(int instrNum) {
    int modeA = (instrNum >> 23) & 0x3;
    return addrMode2[modeA];
}

string getModeB(int instrNum) {
    int modeB = (instrNum >> 10) & 0x3;
    return addrMode2[modeB];
}

string getOperand(int instrNum, char mode) {

    //The operand checks for whether it is operand A or B
    int copiedInstruNum = instrNum;
    int operandToReturn = 0;
    stringstream ss;

    if(mode == 'A') {

        //check for negative
        if (((copiedInstruNum >> 22) & 0x1) == 1) {
            operandToReturn = (instrNum >> 13) & 0x3ff;
            operandToReturn = ~operandToReturn & 0x3ff;
            operandToReturn += 1;
            ss << "-";
        } else {
            operandToReturn = instrNum >> 13 & 0x3ff;

        }
    } else if (mode == 'B') {

        //Check for negative
        if(((copiedInstruNum >> 9) & 0x1) == 1) {
            operandToReturn = (instrNum >> 0) & 0x3ff;

            operandToReturn = ~operandToReturn & 0x3ff;
            operandToReturn += 1;
            ss << "-";
        } else {
            operandToReturn = instrNum >> 0 & 0x3ff;
        }
    }

    ss << operandToReturn;
    return ss.str();
}

string getPlayer(int instrNum) {
    int player = (instrNum >> 30) & 0x3;
    string playerToReturn;
    if(player == 1) {
        playerToReturn = "Player 1";
    } else if (player == 2) {
        playerToReturn = "Player 2";
    } else if (player == 0) {
        playerToReturn = "Unclaimed";
    }

    return playerToReturn;
}


#endif //REDCODE_DISASSEMBLER_H
