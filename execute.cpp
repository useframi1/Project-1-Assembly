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
};

void execute(Instruction inst, int registers[], int &pc)
{
    if (inst.type == 'r')
    {
    }
    else if (inst.type == 'i')
    {
    }
    else if (inst.type == 's')
    {
    }
    else if (inst.type == 'b')
    {
    }
    else if (inst.type == 'u')
    {
    }
    else if (inst.type == 'j')
    {
    }
}