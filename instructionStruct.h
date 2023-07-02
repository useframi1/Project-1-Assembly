#ifndef __INSTRUCTIONSTRUCT_H__
#define __INSTRUCTIONSTRUCT_H__

#include <iostream>
using namespace std;

struct Instruction
{
    int func7;
    int rd;
    int rs1;
    int rs2;
    int func3;
    int immediate;
    char type;
    bool isCompressed;
    string opcode;
    bool isTranslated;
    string decompressedWord;
};
#endif // __INSTRUCTIONSTRUCT_H__